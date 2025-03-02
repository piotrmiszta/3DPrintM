#ifndef WEB_SERVER_PARSER_H_
#define WEB_SERVER_PARSER_H_
#include "err_codes.h"
#include "str.h"

web_server_err_t parser_boot(uint32_t no_parser_threads);
web_server_err_t parser_add_request(str_t request);
web_server_err_t parser_close(void);

#endif
