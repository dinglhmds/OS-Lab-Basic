#ifndef LIST_H
#define LIST_H

#include "common.h"

/* Generic doubly-linked list node */
typedef struct ListNode {
    void *data;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

typedef struct List {
    ListNode *head;
    ListNode *tail;
    int size;
} List;

/* List lifecycle */
List *list_create(void);
void list_destroy(List *list, void (*free_fn)(void *));

/* Insertion */
void list_push_front(List *list, void *data);
void list_push_back(List *list, void *data);

/* Removal */
void *list_pop_front(List *list);
void *list_pop_back(List *list);
void list_remove_node(List *list, ListNode *node, void (*free_fn)(void *));

/* Access */
void *list_front(const List *list);
void *list_back(const List *list);
ListNode *list_find(List *list, int (*cmp)(const void *, const void *), const void *key);

/* Utility */
void list_sort(List *list, int (*cmp)(const void *, const void *));

#endif /* LIST_H */
