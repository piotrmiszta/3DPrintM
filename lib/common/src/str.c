#include <assert.h>
#include <stdlib.h>
#include <string.h>


#include "err_codes.h"
#include "str.h"
#include "str_view.h"

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

str_t string_token(str_t str, str_t delims);
