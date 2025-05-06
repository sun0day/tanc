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
  void *data;
  struct _TCListNode *prev;
  struct _TCListNode *next;
} _TCListNode;

typedef struct {
  _TCListNode *head;
  _TCListNode *tail;
  size_t size;
} _TCList;

typedef struct TCListIterator {
  _TCListNode *cur;
} TCListIter;

typedef void *(*malloc_f)(size_t);
typedef void (*free_f)(void *);

extern _TCListNode *_tc_list_node_new(malloc_f);
extern _TCList *_tc_list_new(malloc_f);
extern void _tc_list_push_back(_TCList *, void *, malloc_f);
extern TCListIter _tc_list_begin(_TCList *);
extern unsigned char tc_list_iter_valid(TCListIter);
extern void *tc_list_iter_get(TCListIter);
extern void tc_list_iter_next(TCListIter *);

#define TCLinkedList(Name, Type, _malloc, _free)                      \
  typedef _TCList Name;                                               \
                                                                      \
  static inline Name *Name##_new() { return _tc_list_new(_malloc); }  \
                                                                      \
  static inline void Name##_push_back(Name *list, Type x) {           \
    Type *ptr = (Type *)_malloc(sizeof(Type));                        \
    memcpy(ptr, &x, sizeof(Type));                                    \
    _tc_list_push_back(list, (void *)ptr, _malloc);                   \
  }                                                                   \
                                                                      \
  static inline size_t Name##_size(Name *list) { return list->size; } \
  static inline TCListIter Name##_begin(Name *list) {                 \
    return _tc_list_begin(list);                                      \
  }

#endif
