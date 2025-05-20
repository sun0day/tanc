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
extern TCListIter _tc_list_insert(TCListIter, TCListIter);
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

#define tc_list_push(list, Type, data) _TCLl##Type##_push(list, data)

#define tc_list_unshift(list, Type, data) _TCLl##Type##_unshift(list, data)

#define tc_list_insert(list, Type, data) _TCLl##Type##_insert(list, data)

#define tc_list_front(list, Type) _TCLl##Type##_front(list)

#define tc_list_back(list, Type) _TCLl##Type##_back(list)

#define tc_list_at(iter, Type) _TCLl##Type##_at(iter)

#define tc_list_clear(list, Type) _TCLl##Type##_clear(list)

#define tc_list_free(list, Type) _TCLl##Type##_free(list)

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
#define _TCListBase(Node, Prop)                                     \
  static inline Node *Node##_new() {                                \
    Node *node = (Node *)_tc_list_alloc(sizeof(Node));              \
    node->Prop.prev = NULL;                                         \
    node->Prop.next = NULL;                                         \
    return node;                                                    \
  }                                                                 \
                                                                    \
  static void Node##_clear(TCList *list) {                          \
    if (list != NULL) {                                             \
      TCListIter next;                                              \
      TCListIter cur = tc_list_begin(list);                         \
      TCListIter end = tc_list_end(list);                           \
      while ((cur != end) & (cur != NULL)) {                        \
        next = tc_list_next(cur);                                   \
                                                                    \
        Node *node = tc_container_of(cur, Node, Prop);              \
        _tc_list_free(node);                                        \
                                                                    \
        cur = next;                                                 \
      }                                                             \
                                                                    \
      list->_st.prev = &list->_st;                                  \
      list->_st.next = &list->_st;                                  \
    }                                                               \
  }                                                                 \
                                                                    \
  static void Node##_free(TCList *list) {                           \
    if (list != NULL) {                                             \
      Node##_clear(list);                                           \
                                                                    \
      _tc_list_free(list);                                          \
    }                                                               \
  }                                                                 \
                                                                    \
  static inline Node *_##Node##_at(TCListIter iter) {               \
    return iter != NULL ? tc_container_of(iter, Node, Prop) : NULL; \
  }

/*
 * Doubly linked list constructor with type
 */
#define _TCListType(Type)                                                  \
  typedef struct {                                                         \
    Type data;                                                             \
    TCListPos pos;                                                         \
  } _TCLl##Type;                                                           \
                                                                           \
  /* clang-format off */                                     \
  _TCListBase(_TCLl##Type, pos)                         \
                                                             \
  static inline Type *_TCLl##Type##_at(TCListIter iter) {             \
    /* clang-format on */                                                  \
    _TCLl##Type *node = __TCLl##Type##_at(iter);                           \
    return node != NULL ? &(node->data) : NULL;                            \
  }                                                                        \
                                                                           \
  static inline Type *_TCLl##Type##_front(TCList *list) {                  \
    return _TCLl##Type##_at(list->_st.next);                               \
  }                                                                        \
                                                                           \
  static inline Type *_TCLl##Type##_back(TCList *list) {                   \
    return _TCLl##Type##_at(list->_st.prev);                               \
  }                                                                        \
                                                                           \
  static inline TCListIter _TCLl##Type##_insert(TCListIter iter, Type x) { \
    _TCLl##Type *node = _TCLl##Type##_new();                               \
    node->data = x;                                                        \
    return _tc_list_insert(iter, &node->pos);                              \
  }                                                                        \
                                                                           \
  static inline void _TCLl##Type##_push(TCList *list, Type x) {            \
    _TCLl##Type##_insert(&list->_st, x);                                   \
  }                                                                        \
                                                                           \
  static inline void _TCLl##Type##_unshift(TCList *list, Type x) {         \
    _TCLl##Type##_insert(list->_st.next, x);                               \
  }

/*
 * Doubly linked list constructor with custom node
 */
#define _TCListNode(Node, Prop)                                                \
  typedef Node _TCLl##Node;                                                    \
                                                                               \
  /* clang-format off */                                      \
  _TCListBase(_TCLl##Node, Prop)                               \
                                                              \
  static inline Node *_TCLl##Node##_at(TCListIter iter) {                \
    /* clang-format on */                                                      \
    return __TCLl##Node##_at(iter);                                            \
  }                                                                            \
                                                                               \
  static inline Node *_TCLl##Node##_front(TCList *list) {                      \
    return _TCLl##Node##_at(list->_st.next);                                   \
  }                                                                            \
                                                                               \
  static inline Node *_TCLl##Node##_back(TCList *list) {                       \
    return _TCLl##Node##_at(list->_st.prev);                                   \
  }                                                                            \
                                                                               \
  static inline TCListIter _TCLl##Node##_insert(TCListIter iter, Node *node) { \
    return _tc_list_insert(iter, &node->Prop);                                 \
  }                                                                            \
                                                                               \
  static inline void _TCLl##Node##_push(TCList *list, Node *node) {            \
    _TCLl##Node##_insert(&list->_st, node);                                    \
  }                                                                            \
                                                                               \
  static inline void _TCLl##Node##_unshift(TCList *list, Node *node) {         \
    _TCLl##Node##_insert(list->_st.next, node);                                \
  }

#define TCListOf(...) \
  tc_args_of(_1, _2, _3, __VA_ARGS__, _TCListNode, _TCListType)(__VA_ARGS__)

#endif
