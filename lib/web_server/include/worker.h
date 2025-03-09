#ifndef WEB_SERVER_WORKER_H_
#define WEB_SERVER_WORKER_H_
#include "err_codes.h"
#include "str.h"

web_server_err_t worker_boot(uint32_t no_worker_threads);
web_server_err_t worker_add_request(str_t request);
web_server_err_t worker_close(void);

#endif
