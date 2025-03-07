#ifndef LIB_COMMON_STR_H_
#define LIB_COMMON_STR_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct str_view_t str_view_t;

typedef struct str_t
{
    char* data;
    uint64_t size;
}str_t;

#define STR_EMPTY()     (str_t) { 0 }

/* Constructors destructors */
str_t string_alloc(uint64_t size);
str_t string_create_from_str_literal(const char* string);
str_t string_create(const char* string, uint64_t size);
str_t string_copy(str_t string);
str_t string_from_string_view(str_view_t str_view);
void string_free(str_t string);

/* From string view */
bool string_equal(str_t a, str_t b);
int32_t string_find(str_t src, char value);
int32_t string_get_size(str_t src);
str_t string_substr(str_t src, int32_t first, int32_t end);

/* Modify string */
/**
 * @brief   add multiple string together,
 *          last element must be empty string
 */
str_t string_concatenate(str_t src, ...);
#endif
