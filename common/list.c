#include "list.h"

List *list_create(void) {
    List *list = safe_malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void list_destroy(List *list, void (*free_fn)(void *)) {
    if (!list) return;
    ListNode *cur = list->head;
    while (cur) {
        ListNode *next = cur->next;
        if (free_fn) free_fn(cur->data);
        free(cur);
        cur = next;
    }
    free(list);
}

void list_push_front(List *list, void *data) {
    ListNode *node = safe_malloc(sizeof(ListNode));
    node->data = data;
    node->prev = NULL;
    node->next = list->head;
    if (list->head) list->head->prev = node;
    list->head = node;
    if (!list->tail) list->tail = node;
    list->size++;
}

void list_push_back(List *list, void *data) {
    ListNode *node = safe_malloc(sizeof(ListNode));
    node->data = data;
    node->next = NULL;
    node->prev = list->tail;
    if (list->tail) list->tail->next = node;
    list->tail = node;
    if (!list->head) list->head = node;
    list->size++;
}

void *list_pop_front(List *list) {
    if (!list || !list->head) return NULL;
    ListNode *node = list->head;
    void *data = node->data;
    list->head = node->next;
    if (list->head) list->head->prev = NULL;
    else list->tail = NULL;
    free(node);
    list->size--;
    return data;
}

void *list_pop_back(List *list) {
    if (!list || !list->tail) return NULL;
    ListNode *node = list->tail;
    void *data = node->data;
    list->tail = node->prev;
    if (list->tail) list->tail->next = NULL;
    else list->head = NULL;
    free(node);
    list->size--;
    return data;
}

void list_remove_node(List *list, ListNode *node, void (*free_fn)(void *)) {
    if (!list || !node) return;
    if (node->prev) node->prev->next = node->next;
    else list->head = node->next;
    if (node->next) node->next->prev = node->prev;
    else list->tail = node->prev;
    if (free_fn) free_fn(node->data);
    free(node);
    list->size--;
}

void *list_front(const List *list) {
    return list && list->head ? list->head->data : NULL;
}

void *list_back(const List *list) {
    return list && list->tail ? list->tail->data : NULL;
}

ListNode *list_find(List *list, int (*cmp)(const void *, const void *), const void *key) {
    if (!list || !cmp) return NULL;
    for (ListNode *n = list->head; n; n = n->next) {
        if (cmp(n->data, key) == 0) return n;
    }
    return NULL;
}

void list_sort(List *list, int (*cmp)(const void *, const void *)) {
    if (!list || list->size < 2) return;
    /* Simple insertion sort for linked list */
    ListNode *sorted = NULL;
    ListNode *cur = list->head;
    while (cur) {
        ListNode *next = cur->next;
        if (!sorted || cmp(cur->data, sorted->data) < 0) {
            cur->prev = NULL;
            cur->next = sorted;
            if (sorted) sorted->prev = cur;
            sorted = cur;
        } else {
            ListNode *s = sorted;
            while (s->next && cmp(s->next->data, cur->data) <= 0) s = s->next;
            cur->prev = s;
            cur->next = s->next;
            if (s->next) s->next->prev = cur;
            s->next = cur;
        }
        cur = next;
    }
    list->head = sorted;
    list->tail = sorted;
    while (list->tail && list->tail->next) list->tail = list->tail->next;
}
