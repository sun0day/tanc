/*
 * @version 0.1.0
 * @copyright 2025 sun0day
 * @description Linked list
 */

#ifndef TANC_LLIST_H
#define TANC_LLIST_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void *malloc(size_t size);
extern void free(void *);

#ifdef __cplusplus
}
#endif

#define TCLinkedList(Name, Type)                                          \
  struct Name##Node {                                                     \
    Type data;                                                            \
    struct Name##Node *prev;                                              \
    struct Name##Node *next;                                              \
  };                                                                      \
                                                                          \
  struct Name {                                                           \
    struct Name##Node *head;                                              \
    struct Name##Node *tail;                                              \
    size_t size;                                                          \
  };                                                                      \
                                                                          \
  static inline struct Name##Node *Name##Node_create() {                  \
    return (struct Name##Node *)malloc(sizeof(struct Name##Node));        \
  }                                                                       \
                                                                          \
  static struct Name *Name##_init() {                                     \
    struct Name *list = (struct Name *)malloc(sizeof(struct Name));       \
    list->head = Name##Node_create();                                     \
    list->tail = Name##Node_create();                                     \
                                                                          \
    list->head->prev = NULL;                                              \
    list->head->next = list->tail;                                        \
                                                                          \
    list->tail->next = NULL;                                              \
    list->tail->prev = list->head;                                        \
                                                                          \
    list->size = 0;                                                       \
    return list;                                                          \
  }                                                                       \
                                                                          \
  static struct Name##Node *Name##_push_back(struct Name *list, Type x) { \
    struct Name##Node *node = Name##Node_create();                        \
    node->data = x;                                                       \
    node->prev = list->tail->prev;                                        \
    node->prev->next = node;                                              \
    list->tail->prev = node;                                              \
    list->size++;                                                         \
    return node;                                                          \
  }
#endif
