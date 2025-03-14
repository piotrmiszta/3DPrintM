#ifndef LIB_COMMON_ERR_CODES_H_
#define LIB_COMMON_ERR_CODES_H_

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "logger.h"

typedef int32_t web_server_err_t;

#define WEB_SERVER_ERR_SUCCESS      0
#define WEB_SERVER_ERR_GENERAL      1
#define WEB_SERVER_ERR_ALLOC        2
#define WEB_SERVER_ERR_BAD_ARG      3
#define WEB_SERVER_ERR_SYS          4

#define WEB_SERVER_UNKNOWN_CLIENT   10
#define WEB_SERVER_PARSER_ERROR     101

#define LIB_COMMON_ERR_PTR          (-1)
#define LIB_COMMON_NOT_FOUND        (-2)
#define LIB_COMMON_ERR_RANGE        (-3)

#define log_errno()          \
        LOG_ERROR("System error, errno = %d, %s", errno, strerror(errno))

#define log_err(err)           \
        LOG_ERROR("System error, errno = %d, %s", (err), strerror((err)))

#endif
