#ifndef WEB_SERVER_H_
#define WEB_SERVER_H_
#include "err_codes.h"

typedef struct web_server_t web_server_t;

web_server_err_t web_server_alloc(web_server_t** server);
web_server_err_t web_server_free(web_server_t* server);
web_server_err_t web_server_run(web_server_t* server);
web_server_err_t web_server_close(web_server_t* server);

#endif
