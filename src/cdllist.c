#include <stdio.h>
#include "cdllist.h"
#include "talloc.h"
#include "helpercode.h"

int insert_head(cdllist *elm, cdllist *list) {
    if (list == NULL) {
        perr("[insert_head] === Critical: List is uninitialized and cannot insert element to list\n");
        return 1;
    }
    if (list->next == list) { // empty list
        elm->next = elm->prev = list;
        list->next = list->prev = elm;
    } else {
        elm->next = list->next;
        elm->prev = list;
        list->next->prev = elm;
        list->next = elm;
    }
    return 0;
}

int insert_tail(cdllist *elm, cdllist *list) {
    if (list == NULL) {
        perr("[insert_tail] === Critical: List is uninitialized and cannot insert element to list\n");
        return 1;
    }
    if (list->next == list) {           // empty list
        elm->next = elm->prev = list;
        list->next = list->prev = elm;
    } else {
        elm->next = list;
        elm->prev = list->prev;
        list->prev->next = elm;
        list->prev = elm;
    }
    return 0;
}

cdllist* remove_from_head(cdllist* list) {
    if (is_empty(list)) {
        perr("[remove_from_head] === Critical: Cannot remove from empty list\n");
        return NULL;
    }

    cdllist* ret_item = list->next;
    list->next = list->next->next;
    list->next->prev = list;
    return ret_item;
}

void remove_from_list(cdllist* elm) {
    cdllist* before = elm->prev;
    cdllist* after = elm->next;

    before->next = after;
    after->prev = before;
    elm->next = elm->prev = elm;
}

int size(const cdllist* const list) {
    if (list == NULL) return -1;
    int s = 0;
    cdllist* tmp = list->next;
    while (tmp != list) {
        s++;
        tmp = tmp->next;
    }
    return s;
}