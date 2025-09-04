#include "buffer.h"
#include <stddef.h>
#ifndef _CDLLIST_H
#define _CDLLIST_H

// Returns the start of the Struct via a ptr to a member of that struct
// Usage: Buffer* buf = container_of(curr_ele_on_list, Buffer, fl_hook);
// Returns the Buffer where fl_hook == curr_ele_on_list (cdllist*).
#define container_of(ptr, type, member) \
    (type *) ((char*) (ptr) - offsetof(type, member))

#define is_empty(lst) \
    (lst == NULL || (lst->next == lst && lst->prev == lst))

typedef struct cdllist cdllist; 

// Circular Doubly Linked List
struct cdllist { 
    cdllist *next;
    cdllist *prev;
};

// Insert an element at the front of the list.
void insert_head(cdllist *elm, cdllist *list);

// Insert an element at the back of the list.
void insert_tail(cdllist *elm, cdllist *list);

// Remove and return the first element from the list.
// Returns NULL if the list is empty.
cdllist *remove_from_head(cdllist *list);

// Remove a specific element from its surrounding.
void remove_from_list(cdllist *elm);

// Count the number of elements in the list.
// Returns -1 if list is uninitialized.
int size(const cdllist *const list);

#endif // _CDLLIST_H
