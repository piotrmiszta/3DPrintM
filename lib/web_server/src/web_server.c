#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>

#include "list.h"
#include "logger.h"
#include "types.h"
#include "web_server.h"

#define NO_FILE_DESCRIPTORS 1000
#define NO_EVENTS 100

struct web_server_t
{
    uint16_t port;
    int16_t reserved;
    uint32_t max_client_in_que;
    list_t clients; //TODO: implement
    pthread_t thread;
    connection_params_t connection_params;
};

static web_server_err_t
web_server_create_connection(web_server_t* server,
                             uint16_t port,
                             uint32_t max_clients_in_que);

static void* web_server_thread(void* arg);
static web_server_err_t web_server_set_noblock(int32_t file_descriptor);
static web_server_err_t web_server_add_event(int32_t epoll,
                                             int32_t file_descriptor,
                                             uint32_t state);
static web_server_err_t web_server_handle_events(int32_t epoll,
                                                 struct epoll_event* events,
                                                 int32_t no_ready_events,
                                                 web_server_t* server);
static web_server_err_t web_server_accept_client(int32_t epoll,
                                                 web_server_t* server);
static web_server_err_t web_server_read(client_t* client);

web_server_err_t web_server_alloc(web_server_t** server)
{
    assert(server);
    *server = (web_server_t*)malloc(sizeof(web_server_t));
    if(NULL == *server)
    {
        return WEB_SERVER_ERR_ALLOC;
    }
    return WEB_SERVER_ERR_SUCCESS;
}

web_server_err_t web_server_free(web_server_t* server)
{
    if(NULL == server)
    {
        return WEB_SERVER_ERR_BAD_ARG;
    }
    free(server);
    return WEB_SERVER_ERR_SUCCESS;
}

web_server_err_t web_server_run(web_server_t* server)
{
    web_server_err_t err;

    err = web_server_create_connection(server, 8080, 5); //TODO: add this to param
    if(err)
    {
        return err;
    }
    list_init(&server->clients);
    err = pthread_create(&server->thread, NULL, web_server_thread, server);
    if(err)
    {
        log_err(err);
        return WEB_SERVER_ERR_SYS;
    }
    return WEB_SERVER_ERR_SUCCESS;
}

static web_server_err_t
web_server_create_connection(web_server_t* server,
                             uint16_t port,
                             uint32_t max_client_in_que)
{
    if(NULL == server)
    {
        return WEB_SERVER_ERR_BAD_ARG;
    }
    int32_t fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == fd)
    {
        log_errno();
        return WEB_SERVER_ERR_SYS;
    }

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    socklen_t len = sizeof(addr);

    if(bind(fd,
            (struct sockaddr*)&addr,
            len) < 0)
    {
        log_errno();
        return WEB_SERVER_ERR_SYS;
    }

    if(listen(fd, (int)max_client_in_que) < 0)
    {
        log_errno();
        return WEB_SERVER_ERR_SYS;
    }

    server->connection_params.addr = addr;
    server->connection_params.addr_len = len;
    server->connection_params.file_descriptor = fd;
    server->max_client_in_que = max_client_in_que;
    server->port = port;

    return WEB_SERVER_ERR_SUCCESS;
}

static void* web_server_thread(void* arg)
{
    web_server_err_t error;
    web_server_t* server = (web_server_t*)arg;
    if(!server)
    {
        return NULL;
    }
    int32_t epoll = epoll_create(NO_FILE_DESCRIPTORS);
    struct epoll_event events[NO_EVENTS];

    error = web_server_set_noblock(server->connection_params.file_descriptor);
    if(error)
    {
        return NULL;
    }
    error = web_server_add_event(epoll,
                                 server->connection_params.file_descriptor,
                                 EPOLLIN);
    if(error)
    {
        return NULL;
    }

    LOG_INFO("Server booted! Waiting for incoming events ... ");
    while(1)
    {
        int32_t no_ready_events = epoll_wait(epoll, events, NO_EVENTS, -1);
        web_server_handle_events(epoll, events, no_ready_events, server);
    }
}

static web_server_err_t web_server_set_noblock(int32_t file_descriptor)
{
    int32_t flags = fcntl(file_descriptor, F_GETFL, 0);
    if(flags < 0)
    {
        log_errno();
        return WEB_SERVER_ERR_SYS;
    }
    flags |= O_NONBLOCK;
    if(fcntl(file_descriptor, F_SETFL, flags) < 0)
    {
        log_errno();
        return WEB_SERVER_ERR_SYS;
    }
    return WEB_SERVER_ERR_SUCCESS;
}

static web_server_err_t web_server_add_event(int32_t epoll,
                                             int32_t file_descriptor,
                                             uint32_t state)
{
    struct epoll_event ev;
    ev.data.fd = file_descriptor;
    ev.events = state;

    if(epoll_ctl(epoll, EPOLL_CTL_ADD, file_descriptor, &ev) < 0)
    {
        log_errno();
        return WEB_SERVER_ERR_SYS;
    }
    return WEB_SERVER_ERR_SUCCESS;
}

static web_server_err_t web_server_handle_events(int32_t epoll,
                                                 struct epoll_event* events,
                                                 int32_t no_ready_events,
                                                 web_server_t* server)
{
    for(uint32_t i = 0; i < (uint32_t)no_ready_events; i++)
    {
        int32_t fd = events[i].data.fd;
        if(fd == server->connection_params.file_descriptor)
        {
            /* accept */
            web_server_accept_client(epoll, server);
        }
        else
        {
            list_t* list = list_first(&server->clients);
            client_t* client = NULL;
            while(!list_is_head(&server->clients, list))
            {
                client = (client_t*)(list_get_ptr(list, client_t, list));
                if(client->connection_param.file_descriptor == fd)
                {
                    break;
                }
                list = list_next(list);
            }
            /* iterate for each clients */
            if(!client)
            {
                return WEB_SERVER_UNKNOWN_CLIENT;
            }

            if(events[i].events & EPOLLIN)
            {
                if(web_server_read(client))
                {
                    // TODO: disconnect client
                }
            }
            else if(events[i].events & EPOLLOUT && client->output_valid)
            {
                LOG_DEBUG("Server write");
            }
            else if(!(events[i].events & EPOLLOUT) && client->output_valid)
            {
                LOG_DEBUG("Error ???");
            }
        }
    }
    return WEB_SERVER_ERR_SUCCESS;
}

static web_server_err_t
web_server_accept_client(int32_t epoll, web_server_t* server)
{
    client_t* client = (client_t*)malloc(sizeof(client_t));
    if(!client)
    {
        return WEB_SERVER_ERR_ALLOC;
    }
    client->connection_param.file_descriptor =
        accept(server->connection_params.file_descriptor,
               (struct sockaddr*)&client->connection_param.addr,
               &client->connection_param.addr_len);
    if(client->connection_param.file_descriptor < 0)
    {
        log_errno();
        return WEB_SERVER_ERR_SYS;
    }
    list_add_tail(&client->list, &server->clients);
    web_server_add_event(epoll,
                         client->connection_param.file_descriptor,
                         EPOLLIN);

    return WEB_SERVER_ERR_SUCCESS;
}

static web_server_err_t web_server_read(client_t* client)
{
    char buffer[1024];
    int32_t fd = client->connection_param.file_descriptor;
    ssize_t n = read(fd, buffer, sizeof(buffer));
    if(n <= 0)
    {
        LOG_TRACE("Invalid readed value for client %d",
                  fd);
    }
    LOG_DEBUG("Readed message from clients %d", fd);
    return WEB_SERVER_ERR_SUCCESS;
}
