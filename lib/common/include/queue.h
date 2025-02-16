#ifndef WEB_SERVER_QUEUE_H_
#define WEB_SERVER_QUEUE_H_
#include "list.h"

typedef list_t queue_t;

static inline void queue_init(queue_t* que)
{
    list_init(que);
}

static inline void queue_push(queue_t* entry, queue_t* head)
{
    list_add_tail(entry, head);
}

static inline queue_t* queue_pop(queue_t* head)
{
    queue_t* ret = list_first(head);
    if(ret == head)
    {
        return NULL;
    }
    list_rem_entry(ret);
    return ret;
}

static inline bool queue_empty(const queue_t* head)
{
    return head == head->next;
}

#endif
