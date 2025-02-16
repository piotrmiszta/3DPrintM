#ifndef WEB_SERVER_TYPES_H_
#define WEB_SERVER_TYPES_H_
#include <netinet/in.h>
#include <stdint.h>
#include <stdbool.h>

#include "list.h"

typedef struct connection_params_t
{
    int32_t file_descriptor;
    struct sockaddr_in addr;
    socklen_t addr_len;
}connection_params_t;

typedef struct client_t
{
    connection_params_t connection_param;
    list_t list;
    bool output_valid;
}client_t;

#endif
