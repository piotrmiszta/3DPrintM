#include "http_header.h"
#include <assert.h>

static web_server_err_t http_header_parse_first_line(http_header_t* header, str_view_t first_line);

static str_view_t http_methods[] =
{
    CONST_STRING_VIEW("GET"),
    CONST_STRING_VIEW("POST"),
    CONST_STRING_VIEW("HEAD"),
};

static str_view_t http_version[] =
{
    CONST_STRING_VIEW("HTTP/0.9"),
    CONST_STRING_VIEW("HTTP/1.0"),
    CONST_STRING_VIEW("HTTP/1.1"),
    CONST_STRING_VIEW("HTTP/2"),
    CONST_STRING_VIEW("HTTP/3"),
};

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
    if( !string_view_equal(rest, STRING_VIEW("\n")) &&
        !string_view_is_empty(rest))
    {
        return WEB_SERVER_PARSER_ERROR;
    }
    for(uint32_t i = 0; i < sizeof(http_methods) / sizeof(http_methods[0]); i++)
    {
        if(string_view_equal(http_methods[i], method))
        {
            header->method = i;
        }
    }

    for(uint32_t i = 0; i < sizeof(http_version) / sizeof(http_version[0]); i++)
    {
        if(string_view_equal(http_version[i], version))
        {
            header->version = i;
        }
    }
    header->url = url;
    return WEB_SERVER_ERR_SUCCESS;
}

