#include "http_header.h"
#include <assert.h>

static web_server_err_t http_header_parse_first_line(http_header_t* header, str_view_t first_line);

web_server_err_t http_header_parse(http_header_t* header, str_view_t data)
{
    str_view_t whitespaces = string_view_create_from_str_literal(" \n\r\t");
    str_view_tokenizer_t lines = string_view_tokenizer_init(data, STRING_VIEW("\n"));
    str_view_t line = string_view_tokenizer_next(&lines);
    http_header_parse_first_line(header, line);
    darray_init(&header->elements);
    while(true)
    {
        line = string_view_tokenizer_next(&lines);
        if(string_view_is_empty(line))
        {
            break;
        }
        str_view_tokenizer_t pair = string_view_tokenizer_init(line, STRING_VIEW(":"));
        str_view_t key = string_view_tokenizer_next(&pair);
        str_view_t value = string_view_tokenizer_get_rest(&pair);
        key = string_view_remove_whitespace(key);
        value = string_view_remove_whitespace(value);
        http_header_element_t element = {.key = key, .value = value};
        darray_push_back(&header->elements, element);
    }
    return WEB_SERVER_ERR_SUCCESS;
}

static web_server_err_t
http_header_parse_first_line(http_header_t* header, str_view_t first_line)
{
    str_view_tokenizer_t tokenizer =
        string_view_tokenizer_init(first_line, STRING_VIEW(" "));
    str_view_t method = string_view_tokenizer_next(&tokenizer);
    str_view_t url = string_view_tokenizer_next(&tokenizer);
    str_view_t version = string_view_tokenizer_next(&tokenizer);
    str_view_t rest = string_view_tokenizer_next(&tokenizer);
    assert(string_view_equal(rest, STRING_VIEW("\n")) || string_view_is_empty(rest));
    /* here should be comparison */
    header->version = HTTP_HEADER_VERSION_1_1;
    header->url = url;
    header->method = HTTP_HEADER_GET;
    return WEB_SERVER_ERR_SUCCESS;
}

