#include <assert.h>

#include "str.h"
#include "str_view.h"
#include "err_codes.h"

str_view_t string_view_create(str_t string)
{
    return (str_view_t) { .data = string.data, .size = string.size };
}

str_view_t string_view_create_from_str_literal(const char* string)
{
    return (str_view_t) { .data = string, .size = strlen(string) };
}

str_view_tokenizer_t
string_view_tokenizer_init(str_view_t string, str_view_t delims)
{
    return (str_view_tokenizer_t) { .data = string, delims = delims, .curr_pos = 0 };
}

bool string_view_equal(str_view_t a, str_view_t b)
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

int32_t string_view_find(str_view_t src, char value)
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

str_view_t string_view_tokenizer_next(str_view_tokenizer_t* tokenizer)
{
    assert(tokenizer);
    assert(tokenizer->data.data);
    const char* data = tokenizer->data.data + tokenizer->curr_pos;
    uint64_t size = tokenizer->data.size - tokenizer->curr_pos;
    const char* delims = tokenizer->delims.data;
    uint64_t no_delims = tokenizer->delims.size;
    for(uint64_t i = 0; i < size; i++)
    {
        for(uint64_t j = 0; j < no_delims; j++)
        {
            if(data[i] == delims[j])
            {
                str_view_t view =
                    {   .data = (data),
                        .size = (i)
                    };
                tokenizer->curr_pos += (i + 1);
                return view;
            }
        }
    }

    if(tokenizer->curr_pos != tokenizer->data.size)
    {
        str_view_t view = {
            .data = (tokenizer->data.data + tokenizer->curr_pos),
            .size = (tokenizer->data.size - tokenizer->curr_pos)
            };
        tokenizer->curr_pos = tokenizer->data.size;
        return view;
    }
    return (str_view_t) { 0 };
}

str_view_t string_view_substr(str_view_t src, int32_t first, int32_t end)
{
    str_t string;
    assert(first < end);
    if(end < src.size)
    {
        return (str_view_t) { .data = (src.data + first),
                              .size = (end - first + 1) };
    }
    return (str_view_t) { 0 };
}

void string_view_tokenizer_reset(str_view_tokenizer_t* tokenizer)
{
    tokenizer->curr_pos = 0;
}