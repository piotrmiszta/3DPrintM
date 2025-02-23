#include <stdlib.h>
#include <string.h>

#include "err_codes.h"
#include "str.h"

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
    if(a.size != b.size)
    {
        return false;
    }
    const char* a_data = a.data;
    const char* b_data = b.data;
    uint32_t size = a.size;
    while(size--)
    {
        if(*a_data++ != *b_data++)
        {
            return false;
        }
    }
    return true;
}

int32_t string_find(str_t src, char value)
{
    if(!src.data)
    {
        return LIB_COMMON_ERR_PTR;
    }
    const char* src_data = src.data;

    for(uint32_t i = 0; i < src.size; i++)
    {
        if(*src_data++ == value)
        {
            return i;
        }
    }
    return LIB_COMMON_NOT_FOUND;
}


str_t string_substr(str_t src, uint32_t first, uint32_t end);

str_t string_token(str_t str, str_t delims);