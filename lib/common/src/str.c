#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include "err_codes.h"
#include "str.h"
#include "str_view.h"

/* for compatilibilty for string concatenation */
static_assert(sizeof(str_t) == sizeof(str_view_t));
static_assert(offsetof(str_t, size) == offsetof(str_view_t, size));
static_assert(offsetof(str_t, data) == offsetof(str_view_t, data));

str_t string_alloc(uint64_t size)
{
    str_t string;
    string.data = malloc(sizeof(*string.data) * size);
    if(!string.data)
    {
        return (str_t) { 0 };
    }
    string.size = size;
    return string;
}

str_t string_create_from_str_literal(const char* string)
{
    return string_create(string, strlen(string));
}

str_t string_create(const char* string, uint64_t size)
{
    str_t str = string_alloc(size);
    if(str.data)
    {
        memcpy(str.data, string, size);
    }
    return str;
}

str_t string_from_string_view(str_view_t str_view)
{
    return string_create(str_view.data, str_view.size);
}

void string_free(str_t string)
{
    free(string.data);
}

str_t string_copy(str_t string)
{
    str_t str = string_alloc(string.size);
    if(str.data)
    {
        memcpy(str.data, string.data, string.size);
    }
    return str;
}

bool string_equal(str_t a, str_t b)
{
    return string_view_equal(string_view_create(a), string_view_create(b));
}

int32_t string_find(str_t src, char value)
{
    return string_view_find(string_view_create(src), value);
}

int32_t string_get_size(str_t src)
{
    return src.size;
}

str_t string_substr(str_t src, int32_t first, int32_t end)
{
    return string_from_string_view(string_view_substr(string_view_create(src), first, end));
}

str_t string_concatenate(str_t src, ...)
{
    va_list args;
    uint64_t size = src.size;
    uint32_t no_args = 0;
    va_start(args, src);
    while(true)
    {
        str_view_t string = va_arg(args, str_view_t);
        if(string.data == NULL && string.size == 0)
        {
            break;
        }
        size += string.size;
        no_args++;
    }
    char* data = realloc(src.data, size);
    if(!data)
    {
        return (str_t) { 0 };
    }
    uint64_t last_pos = src.size;
    va_start(args, src);
    for(uint32_t i = 0; i < no_args; i++)
    {
        str_view_t string = va_arg(args, str_view_t);
        memcpy(&data[last_pos], string.data, string.size);
        last_pos += string.size;
    }
    return (str_t) { .data = data, .size = size };
}

