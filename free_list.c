#include "free_list.h"
#include <stdio.h>

void insert_head(free_list *elm, free_list *list) {
    // free_list new_list = {&new_list, &new_list};
    if (list->next == list) { // empty list
        elm->next = elm->prev = list;
        list->next = list->prev = elm;
    } else {
        list->next->prev = elm;
        elm->next = list->next;
        elm->prev = list;
        list->next = elm;
    }
}

void insert_tail(free_list *elm, free_list *list) {
    if (list->next == list) {           // empty list
        elm->next = elm->prev = list;
        list->next = list->prev = elm;
    } else {
        elm->next = list;
        elm->prev = list->prev;
        list->prev->next = elm;
        list->prev = elm;
    }
}

void remove_from_list(free_list* elm, free_list* list);

unsigned char is_empty(const free_list* const list) {
    return (list->next == list && list->prev == list);
}

int size(const free_list* const list) {
    int s = 0;
    free_list* tmp = list->next;
    while (tmp != list) {
        s++;
        tmp = tmp->next;
    }
    return s;
}