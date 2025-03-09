#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdbool.h>

#include "err_codes.h"
#include "parser.h"
#include "worker.h"
#include "queue.h"

#define PARSER_MAX_NUMBER_OF_REQUESTS   10

typedef struct ParserParam
{
    pthread_t thread;
    bool run;
}ParserParam;

typedef struct ParserGlobalInfo
{
    struct {
        uint32_t last_element_index;
        uint32_t first_element_index;
        str_t requests[PARSER_MAX_NUMBER_OF_REQUESTS];
        pthread_mutex_t mutex;
        sem_t queue_full;
        sem_t queue_empty;
    }request_param;
    uint32_t no_parser;
    ParserParam params[];
}ParserGlobalInfo;

static ParserGlobalInfo* global_info;

static void* parser_thread(void* arg);
static str_t parser_get_request(void);

web_server_err_t parser_add_request(str_t request)
{
    sem_wait(&global_info->request_param.queue_empty);
    pthread_mutex_lock(&global_info->request_param.mutex);
    uint32_t* last_element = &global_info->request_param.last_element_index;
    (*last_element)++;
    if(*last_element == PARSER_MAX_NUMBER_OF_REQUESTS)
    {
        *last_element = 0;
    }
    global_info->request_param.requests[*last_element] = request;
    pthread_mutex_unlock(&global_info->request_param.mutex);
    sem_post(&global_info->request_param.queue_full);
    return WEB_SERVER_ERR_SUCCESS;
}

web_server_err_t parser_boot(uint32_t no_parser_threads)
{
    global_info = calloc(1, no_parser_threads * sizeof(ParserParam) +
                            sizeof(ParserGlobalInfo));
    if(!global_info)
    {
        return WEB_SERVER_ERR_ALLOC;
    }
    pthread_mutex_init(&global_info->request_param.mutex, NULL);
    sem_init(&global_info->request_param.queue_empty, 0, PARSER_MAX_NUMBER_OF_REQUESTS);
    sem_init(&global_info->request_param.queue_full, 0, 0);
    global_info->request_param.first_element_index = 0;
    global_info->request_param.last_element_index = 0;
    global_info->no_parser = no_parser_threads;
    ParserParam* params = global_info->params;
    for(uint32_t i = 0; i < no_parser_threads; i++)
    {
        params[i].run = true;
        int32_t ret = pthread_create(&params[i].thread,
                                     NULL,
                                     parser_thread,
                                     &params[i]);
        if(ret)
        {
            log_err(ret);
            return WEB_SERVER_ERR_SYS;
        }
    }
    return WEB_SERVER_ERR_SUCCESS;
}

static void* parser_thread(void* arg)
{
    ParserParam* param = arg;
    while(true)
    {
        /* get request */
        sem_wait(&global_info->request_param.queue_full);
        if(!param->run)
        {
            return NULL;
        }
        pthread_mutex_lock(&global_info->request_param.mutex);
        str_t request = parser_get_request();
        pthread_mutex_unlock(&global_info->request_param.mutex);
        sem_post(&global_info->request_param.queue_empty);
        LOG_DEBUG("Received request: parser!");
        string_free(request);
        str_t temporary = string_create_from_str_literal("Req to worker");
        worker_add_request(temporary);
    }
    return NULL;
}

static str_t parser_get_request(void)
{

    uint32_t* next_element = &global_info->request_param.first_element_index;
    (*next_element)++;
    if(*next_element == PARSER_MAX_NUMBER_OF_REQUESTS)
    {
        *next_element = 0;
    }
    str_t request = global_info->request_param.requests[*next_element];

    return request;
}

web_server_err_t parser_close(void)
{
    uint32_t no_parser_threads = global_info->no_parser;

    ParserParam* params = global_info->params;
    for(uint32_t i = 0; i < no_parser_threads; i++)
    {
        params[i].run = false;
    }
    for(uint32_t i = 0; i < no_parser_threads; i++)
    {
        sem_post(&global_info->request_param.queue_full);
    }
    for(uint32_t i = 0; i < no_parser_threads; i++)
    {
        pthread_join(params[i].thread, NULL);
    }
    pthread_mutex_destroy(&global_info->request_param.mutex);
    sem_destroy(&global_info->request_param.queue_empty);
    sem_destroy(&global_info->request_param.queue_full);
    free(global_info);
    return WEB_SERVER_ERR_SUCCESS;
}
