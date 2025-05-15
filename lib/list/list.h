/*
 * @version 0.1.0
 * @copyright 2025 sun0day
 * @description Doubly linked list
 */

#ifndef TANC_LLIST_H
#define TANC_LLIST_H

#include "macro.h"

typedef struct TCListPos {
  struct TCListPos *prev;
  struct TCListPos *next;
} TCListPos;

typedef TCListPos *TCListIter;

typedef struct TCList {
  TCListPos _st;
} TCList;

extern TCListIter tc_list_end(TCList *);
extern TCListIter tc_list_next(TCListIter);
extern TCListIter tc_list_prev(TCListIter);
extern TCListIter tc_list_insert(TCListIter, TCListIter);
extern TCList *_tc_list_new(malloc_f _malloc);
extern unsigned char tc_list_empty(TCList *);

#ifndef _tc_list_alloc
#define _tc_list_alloc _tc_get_alloc(TC_ALLOCATOR)
#endif

#ifndef _tc_list_free
#define _tc_list_free _tc_get_free(TC_ALLOCATOR)
#endif

/*
 * Create new list
 */
#define tc_list_new() _tc_list_new(_tc_list_alloc)

/*
 * Get the sentinel node's iterator
 */
#define tc_list_rend tc_list_end

/*
 * Get the first node's iterator
 */
#define tc_list_begin(...) tc_list_next(tc_list_end(__VA_ARGS__))

/*
 * Get the last node's iterator
 */
#define tc_list_rbegin(...) tc_list_prev(tc_list_end(__VA_ARGS__))

/*
 * Iterate over list elements until
 * iterator reaches the bound or becomes invalid
 */
#define tc_list_each(start, end, cur)                        \
  for (TCListIter cur = start; (cur != end) & (cur != NULL); \
       cur = tc_list_next(cur))
/*
 * Iterate over list elements in the reverse order until
 * iterator reaches the bound or becomes invalid
 */
#define tc_list_reach(start, end, cur)                       \
  for (TCListIter cur = start; (cur != end) & (cur != NULL); \
       cur = tc_list_prev(cur))

/*
 * Generate doubly linked list bases
 */
#define _TCListWrap(Name, Node, Prop)                               \
  typedef TCList Name;                                              \
                                                                    \
  static inline Node *Node##_new() {                                \
    Node *node = (Node *)_tc_list_alloc(sizeof(Node));              \
    node->Prop.prev = NULL;          \
    node->Prop.next = NULL;          \
    return node;                                                    \
  }                                                                 \
                                                                    \
  static void Name##_clear(Name *list) {                            \
    if (list == NULL) {                                             \
      return;                                                       \
    }                                                               \
                                                                    \
    TCListIter start = tc_list_next(tc_list_begin(list));           \
    tc_list_each(start, tc_list_end(list), iter) {                  \
      Node *node = tc_container_of(tc_list_prev(iter), Node, Prop); \
      _tc_list_free(node);                                          \
    }                                                               \
                                                                    \
    list->_st.prev = &list->_st;                                    \
    list->_st.next = &list->_st;                                    \
  }                                                                 \
                                                                    \
  static void Name##_free(Name *list) {                             \
    if (list == NULL) {                                             \
      return;                                                       \
    }                                                               \
                                                                    \
    Name##_clear(list);                                             \
                                                                    \
    _tc_list_free(list);                                            \
  }                                                                 \
                                                                    \
  static inline Node *_##Name##_at(TCListIter iter) {               \
    /* clang-format on */                                           \
    return iter == NULL ? NULL : tc_container_of(iter, Node, Prop); \
  }

/*
 * Doubly linked list constructor
 */
#define _TCListType(Name, Type)                                     \
  typedef struct {                                                  \
    Type data;                                                      \
    TCListPos pos;                                                  \
  } Name##Node;                                                     \
                                                                    \
  /* clang-format off */                                     \
  _TCListWrap(Name, Name##Node, pos)                         \
                                                             \
  static inline Type *Name##_at(TCListIter iter) {      \
    /* clang-format on */                                           \
    Name##Node *node = _##Name##_at(iter);                          \
    return node == NULL ? NULL : &(node->data);                     \
  }                                                                 \
                                                                    \
  static inline TCListIter Name##_insert(TCListIter iter, Type x) { \
    Name##Node *node = Name##Node_new();                            \
    node->data = x;                                                 \
    return tc_list_insert(iter, &node->pos);                        \
  }                                                                 \
                                                                    \
  static inline Type *Name##_front(Name *list) {                    \
    return Name##_at(list->_st.next);                               \
  }                                                                 \
                                                                    \
  static inline Type *Name##_back(Name *list) {                     \
    return Name##_at(list->_st.prev);                               \
  }                                                                 \
                                                                    \
  static inline void Name##_push(Name *list, Type x) {              \
    Name##_insert(&list->_st, x);                                   \
  }                                                                 \
                                                                    \
  static inline void Name##_unshift(Name *list, Type x) {           \
    Name##_insert(list->_st.next, x);                               \
  }

/*
 * Instrusive doubly linked list constructor
 */
#define _TCListInsv(Name, Node, Prop)                                   \
  /* clang-format off */                                      \
  _TCListWrap(Name, Node, Prop)                               \
                                                              \
  static inline Node *Name##_at(TCListIter iter) {         \
    /* clang-format on */                                               \
    return _##Name##_at(iter);                                          \
  }                                                                     \
                                                                        \
  static inline Node *Name##_front(Name *list) {                        \
    return Name##_at(list->_st.next);                                   \
  }                                                                     \
                                                                        \
  static inline Node *Name##_back(Name *list) {                         \
    return Name##_at(list->_st.prev);                                   \
  }                                                                     \
                                                                        \
  static inline TCListIter Name##_insert(TCListIter iter, Node *node) { \
    return tc_list_insert(iter, &node->Prop);                           \
  }                                                                     \
                                                                        \
  static inline void Name##_push(Name *list, Node *node) {              \
    Name##_insert(&list->_st, node);                                    \
  }                                                                     \
                                                                        \
  static inline void Name##_unshift(Name *list, Node *node) {           \
    Name##_insert(list->_st.next, node);                                \
  }

#define TCListOf(...) \
  tc_args_of(_1, _2, __VA_ARGS__, _TCListInsv, _TCListType)(__VA_ARGS__)

#endif
