#ifndef LIB_COMMON_STR_VIEW_H_
#define LIB_COMMON_STR_VIEW_H_
#include <stdint.h>

typedef struct str_t str_t;

typedef struct str_view_t
{
    const char* data;
    uint64_t size;
}str_view_t;

typedef struct str_view_tokenizer_t
{
    str_view_t data;
    str_view_t delims;
    uint64_t curr_pos;
}str_view_tokenizer_t;

#define STRING_VIEW(str)    string_view_create_from_str_literal((str))

str_view_t string_view_create(str_t string);
str_view_t string_view_create_from_str_literal(const char* string);
str_view_tokenizer_t
string_view_tokenizer_init(str_view_t string, str_view_t delims);
bool string_view_equal(str_view_t a, str_view_t b);
int32_t string_view_find(str_view_t a, char element);
str_view_t string_view_tokenizer_next(str_view_tokenizer_t* tokenizer);
void string_view_tokenizer_reset(str_view_tokenizer_t* tokenizer);

#endif
