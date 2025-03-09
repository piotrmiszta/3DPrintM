#ifndef WEB_SERVER_WORKER_H_
#define WEB_SERVER_WORKER_H_

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>

#include "err_codes.h"
#include "str.h"

#define WORKER_MAX_NUMBER_OF_REQ    10

typedef struct WorkerParam
{
    pthread_t thread;
    bool run;
}WorkerParam;

typedef struct WorkerGlobalInfo
{
    struct {
        uint32_t next_element_index;
        uint32_t next_avail_element_index;
        str_t requests[WORKER_MAX_NUMBER_OF_REQ];
        pthread_mutex_t mutex;
        sem_t queue_full;
        sem_t queue_empty;
    }request_param;
    uint32_t no_workers;
    WorkerParam params[];
}WorkerGlobalInfo;

static WorkerGlobalInfo* global_info;
static void* worker_thread(void* arg);
static str_t worker_get_request(void);

web_server_err_t worker_boot(uint32_t no_worker_threads)
{
    global_info = calloc(1, no_worker_threads * sizeof(WorkerParam) +
                            sizeof(WorkerGlobalInfo));
    if(!global_info)
    {
        return WEB_SERVER_ERR_ALLOC;
    }

    pthread_mutex_init(&global_info->request_param.mutex, NULL);
    sem_init(&global_info->request_param.queue_empty, 0, WORKER_MAX_NUMBER_OF_REQ);
    sem_init(&global_info->request_param.queue_full, 0, 0);
    global_info->request_param.next_avail_element_index = 0;
    global_info->request_param.next_element_index = 0;
    global_info->no_workers = no_worker_threads;
    WorkerParam* params = global_info->params;
    for(uint32_t i = 0; i < no_worker_threads; i++)
    {
        params[i].run = true;
        int32_t ret = pthread_create(&params[i].thread,
                                     NULL,
                                     worker_thread,
                                     &params[i]);
        if(ret)
        {
            log_err(ret);
            return WEB_SERVER_ERR_SYS;
        }
    }
    return WEB_SERVER_ERR_SUCCESS;
}

static void *worker_thread(void *arg)
{
    WorkerParam* param = arg;
    while(true)
    {
        /* get request */
        sem_wait(&global_info->request_param.queue_full);
        if(!param->run)
        {
            return NULL;
        }
        pthread_mutex_lock(&global_info->request_param.mutex);
        str_t request = worker_get_request();
        pthread_mutex_unlock(&global_info->request_param.mutex);
        sem_post(&global_info->request_param.queue_empty);
        LOG_DEBUG("Received request in worker!");
        string_free(request);
    }
    return NULL;
}

web_server_err_t worker_add_request(str_t request)
{
    sem_wait(&global_info->request_param.queue_empty);
    pthread_mutex_lock(&global_info->request_param.mutex);
    uint32_t* next_avail =
        &global_info->request_param.next_avail_element_index;
    (*next_avail)++;
    if(*next_avail == WORKER_MAX_NUMBER_OF_REQ)
    {
        *next_avail = 0;
    }
    global_info->request_param.requests[*next_avail] = request;
    pthread_mutex_unlock(&global_info->request_param.mutex);
    sem_post(&global_info->request_param.queue_full);
    return WEB_SERVER_ERR_SUCCESS;
}

web_server_err_t worker_close(void)
{
    uint32_t no_worker_threads = global_info->no_workers;

    WorkerParam* params = global_info->params;
    for(uint32_t i = 0; i < no_worker_threads; i++)
    {
        params[i].run = false;
    }
    for(uint32_t i = 0; i < no_worker_threads; i++)
    {
        sem_post(&global_info->request_param.queue_full);
    }
    for(uint32_t i = 0; i < no_worker_threads; i++)
    {
        pthread_join(params[i].thread, NULL);
    }
    pthread_mutex_destroy(&global_info->request_param.mutex);
    sem_destroy(&global_info->request_param.queue_empty);
    sem_destroy(&global_info->request_param.queue_full);
    free(global_info);
    return WEB_SERVER_ERR_SUCCESS;
}

static str_t worker_get_request(void)
{

    uint32_t* next_element = &global_info->request_param.next_element_index;
    (*next_element)++;
    if(*next_element == WORKER_MAX_NUMBER_OF_REQ)
    {
        *next_element = 0;
    }
    str_t request = global_info->request_param.requests[*next_element];

    return request;
}

#endif
