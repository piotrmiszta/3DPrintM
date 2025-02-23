#ifndef LIB_COMMON_STR_H_
#define LIB_COMMON_STR_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct str_t
{
    uint32_t size;
    char* data;
}str_t;

str_t string_alloc(uint32_t size);
str_t string_create_from_str_literal(const char* string);
str_t string_create(const char* string, uint32_t size);
str_t string_copy(str_t string);
void string_free(str_t string);

bool string_equal(str_t a, str_t b);
str_t string_substr(str_t src, uint32_t first, uint32_t end);
uint32_t string_find(str_t src, char value);

str_t string_token(str_t str, str_t delims);

#endif
