/*
 * @version 0.1.0
 * @copyright 2025 sun0day
 * @description Linked list
 */

#ifndef TANC_LLIST_H
#define TANC_LLIST_H

#include <stddef.h>
#include <string.h>

typedef struct TCListNode {
  struct TCListNode *prev;
  struct TCListNode *next;
} TCListNode;

typedef TCListNode *TCListIter;

typedef void *(*malloc_f)(size_t);
typedef void (*free_f)(void *);

extern unsigned char tc_list_iter_valid(TCListIter);
extern TCListIter tc_list_iter_next(TCListIter);
extern TCListIter tc_list_iter_prev(TCListIter);

#ifndef TC_ALLOCATOR
#define TC_ALLOCATOR malloc, free
#endif

#ifndef _tc_get_alloc
#define _tc_get_alloc(...) __tc_get_alloc(__VA_ARGS__)
#define __tc_get_alloc(_malloc, _free) _malloc
#endif

#ifndef _tc_get_free
#define _tc_get_free(...) __tc_get_free(__VA_ARGS__)
#define __tc_get_free(_malloc, _free) _free
#endif

#ifndef _tc_list_alloc
#define _tc_list_alloc _tc_get_alloc(TC_ALLOCATOR)
#endif

#ifndef _tc_list_free
#define _tc_list_free _tc_get_free(TC_ALLOCATOR)
#endif

/*
 * Get the parent object pointer from its property,
 * it's type-unsafe!
 */
#define tc_container_of(ptr, type, prop) \
  ((type *)((char *)(ptr) - offsetof(type, prop)))

/*
 * iterate over list elements until
 * iterator reaches the bound or becomes invalid
 */
#define tc_list_each(list, start, end, cur)                            \
  for (TCListIter cur = start; (cur != end) & tc_list_iter_valid(cur); \
       cur = tc_list_iter_next(cur))

/*
 * iterate over list elements in the reverse order until
 * iterator reaches the bound or becomes invalid
 */
#define tc_list_reach(list, start, end, cur)                           \
  for (TCListIter cur = start; (cur != end) & tc_list_iter_valid(cur); \
       cur = tc_list_iter_prev(cur))

#define TCLinkedList(Name, Type)                             \
  typedef struct {                                           \
    Type data;                                               \
    TCListNode pos;                                          \
  } Name##Node;                                              \
                                                             \
  /* clang-format off */                                   \
  TCListWrap(Name, Name##Node, pos)               \
                                                           \
  static void Name##_push_back(Name *list, Type x) { \
    /* clang-format on */                                    \
    Name##Node *node = Name##Node_new();                     \
    node->data = x;                                          \
                                                             \
    node->pos.prev = list->tail->pos.prev;                   \
    node->pos.prev->next = &node->pos;                       \
    node->pos.next = &list->tail->pos;                       \
    list->tail->pos.prev = &node->pos;                       \
  }                                                          \
                                                             \
  static inline Type Name##_get(TCListIter iter) {           \
    return (tc_container_of(iter, Name##Node, pos))->data;   \
  }

#define TCInsvList(Name, Node, Prop)                         \
  /* clang-format off */                                   \
  TCListWrap(Name, Node, Prop) \
                                                             \
  static inline Node *Name##_get(TCListIter iter) { \
    /* clang-format on */                                    \
    return tc_container_of(iter, Node, Prop);                \
  }

#define TCListWrap(Name, Node, Prop)                                           \
  typedef struct {                                                             \
    Node *head;                                                                \
    Node *tail;                                                                \
  } Name;                                                                      \
                                                                               \
  static inline Node *Node##_new() {                                           \
    Node *node = (Node *)_tc_list_alloc(sizeof(Node));                         \
    node->Prop = (TCListNode){.prev = NULL, .next = NULL};                     \
    return node;                                                               \
  }                                                                            \
                                                                               \
  static Name *Name##_new() {                                                  \
    Name *list = (Name *)_tc_list_alloc(sizeof(Name));                         \
                                                                               \
    list->head = Node##_new();                                                 \
    list->tail = Node##_new();                                                 \
                                                                               \
    list->head->Prop.next = &list->tail->Prop;                                 \
    list->tail->Prop.prev = &list->head->Prop;                                 \
                                                                               \
    return list;                                                               \
  }                                                                            \
                                                                               \
  static inline unsigned char Name##_empty(Name *list) {                       \
    return list == NULL | list->head->Prop.next == &list->tail->Prop;          \
  }                                                                            \
                                                                               \
  static inline TCListIter Name##_begin(Name *list) {                          \
    return (list == NULL | list->head == NULL) ? NULL : list->head->Prop.next; \
  }                                                                            \
                                                                               \
  static inline TCListIter Name##_end(Name *list) {                            \
    return (list == NULL | list->tail == NULL) ? NULL : &list->tail->Prop;     \
  }                                                                            \
                                                                               \
  static inline void Name##_free_iter(TCListIter iter) {}                      \
                                                                               \
  static inline void Name##_free(Name *list) {                                 \
    if (list == NULL) {                                                        \
      return;                                                                  \
    }                                                                          \
                                                                               \
    tc_list_each(list, Name##_begin(list), Name##_end(list), iter) {           \
      Node *node = tc_container_of(tc_list_iter_prev(iter), Node, Prop);       \
      if (node != NULL) _tc_list_free(node);                                   \
    }                                                                          \
                                                                               \
    _tc_list_free(list->tail);                                                 \
                                                                               \
    _tc_list_free(list);                                                       \
  }

#endif
