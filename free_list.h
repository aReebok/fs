#include "buffer.h"
#include <stddef.h>
#ifndef _FREE_LIST_H
#define _FREE_LIST_H

// Returns the start of the Struct of a given member of that struct
// ptr: pointer to the member (char* - cast it as byte pointer)
// type: struct name
// member: name of the member field
#define container_of(ptr, type, member) \
    (type *) ((char*) (ptr) - offsetof(type, member))

// Its actually a generic Circular Doubly Linked List
// could be used both for both
//  Free List
//  Hash Queue - list is Circular DLL
typedef struct free_list free_list; 

struct free_list { 
    free_list *next;
    free_list *prev;
};

void insert_head(free_list *elm, free_list *list);
void insert_tail(free_list *elm, free_list *list);
void remove_from_list(free_list* elm, free_list* list);
unsigned char is_empty(const free_list* const list);

#endif