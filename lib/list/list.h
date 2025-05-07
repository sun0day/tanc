/*
 * @version 0.1.0
 * @copyright 2025 sun0day
 * @description Linked list
 */

#ifndef TANC_LLIST_H
#define TANC_LLIST_H

#include <stddef.h>
#include <string.h>

typedef struct _TCListNode {
  struct _TCListNode *prev;
  struct _TCListNode *next;
} _TCListNode;

typedef _TCListNode *TCListIter;

typedef void *(*malloc_f)(size_t);
typedef void (*free_f)(void *);

extern unsigned char tc_list_iter_valid(TCListIter);
extern TCListIter tc_list_iter_next(TCListIter);

#ifndef tc_allocator
#define tc_allocator malloc, free
#endif

#define _tc_get_alloc(...) __tc_get_alloc(__VA_ARGS__)
#define __tc_get_alloc(_malloc, _free) _malloc

#define _tc_get_free(...) __tc_get_free(__VA_ARGS__)
#define __tc_get_free(_malloc, _free) _free

#ifndef _tc_list_alloc
#define _tc_list_alloc _tc_get_alloc(tc_allocator)
#endif

#ifndef _tc_list_free
#define _tc_list_free _tc_get_free(tc_allocator)
#endif

#define container_of(ptr, type, member) \
  ((type *)((char *)(ptr) - offsetof(type, member)))

#define TCLinkedList(Name, Type)                                         \
  typedef struct {                                                       \
    Type data;                                                           \
    _TCListNode pos;                                                     \
  } Name##Node;                                                          \
                                                                         \
  typedef struct {                                                       \
    Name##Node *head;                                                    \
    Name##Node *tail;                                                    \
  } Name;                                                                \
                                                                         \
  static inline Name##Node *Name##Node_new() {                           \
    Name##Node *node = (Name##Node *)_tc_list_alloc(sizeof(Name##Node)); \
    node->pos = (_TCListNode){.prev = NULL, .next = NULL};               \
    return node;                                                         \
  }                                                                      \
                                                                         \
  static Name *Name##_new() {                                            \
    Name *list = (Name *)_tc_list_alloc(sizeof(Name));                   \
                                                                         \
    list->head = Name##Node_new();                                       \
    list->tail = Name##Node_new();                                       \
                                                                         \
    list->head->pos.next = &list->tail->pos;                             \
    list->tail->pos.prev = &list->head->pos;                             \
                                                                         \
    return list;                                                         \
  }                                                                      \
                                                                         \
  static void Name##_push_back(Name *list, Type x) {                     \
    Name##Node *node = Name##Node_new();                                 \
    node->data = x;                                                      \
                                                                         \
    node->pos.prev = list->tail->pos.prev;                               \
    node->pos.prev->next = &node->pos;                                   \
    node->pos.next = &list->tail->pos;                                   \
    list->tail->pos.prev = &node->pos;                                   \
  }                                                                      \
                                                                         \
  static inline unsigned char Name##_empty(Name *list) {                 \
    return list == NULL | list->head->pos.next == &list->tail->pos;      \
  }                                                                      \
                                                                         \
  static inline TCListIter Name##_begin(Name *list) {                    \
    return list == NULL ? NULL : list->head->pos.next;                   \
  }                                                                      \
                                                                         \
  static inline Type Name##_get(TCListIter iter) {                       \
    return (container_of(iter, Name##Node, pos))->data;                  \
  }

#endif
