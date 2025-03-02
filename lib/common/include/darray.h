#ifndef LIB_COMMON_DARRAY_H_
#define LIB_COMMON_DARRAY_H_
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#define darray(type)            \
        struct                  \
        {                       \
            type* data;         \
            uint32_t size;      \
            uint32_t capacity;  \
        }

#define darray_init(array)      \
    do {                        \
        (array)->data = NULL;   \
        (array)->size = 0;      \
        (array)->capacity = 0;  \
    }while(0)

#define darray_push_back(array, value)                                      \
    do {                                                                    \
        if((array)->size == (array)->capacity)                              \
        {                                                                   \
            (array)->capacity += 5;                                         \
            void* ptr = realloc((array)->data, (array)->capacity * sizeof((value))); \
            assert(ptr);                                                    \
            (array)->data = ptr;                                                     \
        }                                                                   \
        (array)->data[(array)->size] = (value);                             \
        (array)->size++;                                                     \
    }while(0)

#define darray_at(array, index) \
        (array)->data[index]

#define darray_erase(array, index)                                  \
        do {                                                        \
            for(uint32_t i = index; i < (array)->size - 1; i++)     \
            {                                                       \
                (array)->data[i] = (array)->data[i + 1];            \
            }                                                       \
            (array)->size--;                                        \
        }while(0)

#define darray_deinit(array)        \
    do{                             \
        free((array)->data);        \
        darray_init(array);         \
    }while(0)

#endif