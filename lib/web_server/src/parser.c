#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "parser.h"

typedef struct ParserParam
{
    pthread_t thread;
    bool run;
}ParserParam;

typedef struct ParserGlobalInfo
{
    uint32_t no_parser;
    ParserParam params[];
}ParserGlobalInfo;

static ParserGlobalInfo* global_info;

static void* parser_thread(void* arg);

web_server_err_t parser_boot(uint32_t no_parser_threads)
{
    global_info = malloc(no_parser_threads * sizeof(ParserParam) +
                         sizeof(ParserGlobalInfo));
    if(!global_info)
    {
        return WEB_SERVER_ERR_ALLOC;
    }
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
    (void)arg;
    while(1)
    {

    }
}
