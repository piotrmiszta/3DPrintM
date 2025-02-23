#include <stdlib.h>
#include <string.h>

#include "str.h"

str_t string_alloc(uint32_t size)
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

str_t string_create(const char* string, uint32_t size)
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

str_t string_substr(str_t src, uint32_t first, uint32_t end);
uint32_t string_find(str_t src, char value);

str_t string_token(str_t str, str_t delims);