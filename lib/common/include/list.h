#ifndef LIB_COMMON_LIST_H_
#define LIB_COMMON_LIST_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct list_t
{
    struct list_t* next;
    struct list_t* prev;
}list_t;

#define LIST_INIT(name) \
        (name) = { &name, &name };

#define list_get_ptr(ptr, type, member)         \
    (type*)((char*)(list_t*)(ptr) - offsetof(type, member))

static inline void list_init(list_t* list)
{
    list->next = list;
    list->prev = list;
}

static inline void list_add_between(list_t* entry, list_t* prev, list_t* next)
{
    entry->next = next;
    entry->prev = prev;
    next->prev = entry;
    prev->next = entry;
}

static inline void list_add_head(list_t* entry, list_t* head)
{
    list_add_between(entry, head, head->next);
}

static inline void list_add_tail(list_t* entry, list_t* head)
{
    list_add_between(entry, head->prev, head);
}

static inline void list_rem_between(list_t* prev, list_t* next)
{
    prev->next = next;
    next->prev = prev;
}

static inline void list_rem_entry(list_t* entry)
{
    list_rem_between(entry->prev, entry->next);
}

static inline bool list_is_empty(list_t* head)
{
    return head == head->next;
}

static inline bool list_is_head(list_t* head, list_t* entry)
{
    return head == entry;
}

static inline list_t* list_first(list_t* head)
{
    return head->next;
}

static inline list_t* list_last(list_t* head)
{
    return head->prev;
}

static inline list_t* list_next(list_t* entry)
{
    return entry->next;
}

static inline list_t* list_prev(list_t* entry)
{
    return entry->prev;
}

#endif
