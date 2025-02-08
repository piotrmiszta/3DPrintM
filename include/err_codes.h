#ifndef WEB_SERVER_ERR_CODES_H_
#define WEB_SERVER_ERR_CODES_H_

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef int32_t web_server_err_t;

#define WEB_SERVER_ERR_SUCCESS      0
#define WEB_SERVER_ERR_GENERAL      1
#define WEB_SERVER_ERR_ALLOC        2
#define WEB_SERVER_ERR_BAD_ARG      3
#define WEB_SERVER_ERR_SYS          4

#define WEB_SERVER_UNKNOWN_CLIENT   10

#define log_errno()          \
        fprintf(stderr, "ERROR: %s", strerror(errno))

#define log_err(err)           \
        fprintf(stderr, "ERROR: %s", strerror(err))

#endif
