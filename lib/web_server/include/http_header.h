#ifndef WEB_SERVER_HTTP_HEADER_H_
#define WEB_SERVER_HTTP_HEADER_H_
#include "str.h"
#include "str_view.h"
#include "list.h"
#include "err_codes.h"
#include "darray.h"

enum HttpHeaderMethodE {
  HTTP_HEADER_GET = 0,
  HTTP_HEADER_POST,
  HTTP_HEADER_HEAD
};

enum HttpHeaderVersionE {
    HTTP_HEADER_VERSION_0_9 = 0,
    HTTP_HEADER_VERSION_1_0,
    HTTP_HEADER_VERSION_1_1,
    HTTP_HEADER_VERSION_2,
    HTTP_HEADER_VERSION_3,
};

typedef struct http_header_element_t
{
    str_view_t key;
    str_view_t value;
}http_header_element_t;

typedef struct http_header_t
{
    str_t data;
    str_view_t url;
    enum HttpHeaderMethodE method;
    enum HttpHeaderVersionE version;
    darray(http_header_element_t) elements;
}http_header_t;

web_server_err_t http_header_parse(http_header_t* header, str_view_t data);

#endif
