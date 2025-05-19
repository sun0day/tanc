/*
 * @version 0.1.0
 * @copyright 2025 sun0day
 * @description Singly linked list
 */

#ifndef TANC_SLIST_H
#define TANC_SLIST_H

#include "macro.h"

typedef void *TCSlistPos;

typedef TCSlistPos *TCSlistIter;

typedef struct TCSlist {
  TCSlistPos _st;
  TCSlistIter _back;
} TCSlist;

extern TCSlistIter tc_slist_begin(TCSlist *);
extern TCSlistIter tc_slist_next(TCSlistIter);
extern TCSlistIter _tc_slist_insert(TCSlistIter, TCSlistIter);
extern TCSlist *_tc_slist_new(malloc_f _malloc);
extern unsigned char tc_slist_empty(TCSlist *);

#ifndef _tc_slist_alloc
#define _tc_slist_alloc _tc_get_alloc(TC_ALLOCATOR)
#endif

#ifndef _tc_slist_free
#define _tc_slist_free _tc_get_free(TC_ALLOCATOR)
#endif

/*
 * Create new list
 */
#define tc_slist_new() _tc_slist_new(_tc_slist_alloc)

#define tc_slist_push(list, Type, data) _TCSl##Type##_push(list, data)

#define tc_slist_unshift(list, Type, data) _TCSl##Type##_unshift(list, data)

#define tc_slist_insert(list, Type, data) _TCSl##Type##_insert(list, data)

#define tc_slist_front(list, Type) _TCSl##Type##_front(list)

#define tc_slist_back(list, Type) _TCSl##Type##_back(list)

#define tc_slist_at(iter, Type) _TCSl##Type##_at(iter)

#define tc_slist_clear(list, Type) _TCSl##Type##_clear(list)

#define tc_slist_free(list, Type) _TCSl##Type##_free(list)

/*
 * Iterate over list elements until
 * iterator reaches the bound or becomes invalid
 */
#define tc_slist_each(start, end, cur)                        \
  for (TCSlistIter cur = start; (cur != end) & (cur != NULL); \
       cur = tc_slist_next(cur))

/*
 * Generate singly linked list bases
 */
#define _TCSlistBase(Node, Mem)                                    \
  static inline Node *Node##_new() {                               \
    Node *node = (Node *)_tc_slist_alloc(sizeof(Node));            \
    node->Mem = NULL;                                              \
    return node;                                                   \
  }                                                                \
                                                                   \
  static void Node##_clear(TCSlist *list) {                        \
    if (list != NULL) {                                            \
      TCSlistIter next;                                            \
      TCSlistIter cur = tc_slist_begin(list);                      \
      while (cur != NULL) {                                        \
        next = tc_slist_next(cur);                                 \
                                                                   \
        Node *node = tc_container_of(cur, Node, Mem);              \
        _tc_slist_free(node);                                      \
                                                                   \
        cur = next;                                                \
      }                                                            \
                                                                   \
      list->_st = NULL;                                            \
      list->_back = &list->_st;                                    \
    }                                                              \
  }                                                                \
                                                                   \
  static void Node##_free(TCSlist *list) {                         \
    if (list != NULL) {                                            \
      Node##_clear(list);                                          \
                                                                   \
      _tc_slist_free(list);                                        \
    }                                                              \
  }                                                                \
                                                                   \
  static inline Node *_##Node##_at(TCSlistIter iter) {             \
    return iter != NULL ? tc_container_of(iter, Node, Mem) : NULL; \
  }

/*
 * Singly linked list constructor
 */
#define _TCSlistType(Type)                                                   \
  typedef struct {                                                           \
    Type data;                                                               \
    TCSlistPos pos;                                                          \
  } _TCSl##Type;                                                             \
                                                                             \
  /* clang-format off */                                     \
  _TCSlistBase(_TCSl##Type, pos)                         \
                                                             \
  static inline Type *_TCSl##Type##_at(TCSlistIter iter) {               \
    /* clang-format on */                                                    \
    _TCSl##Type *node = __TCSl##Type##_at(iter);                             \
    return node != NULL ? &(node->data) : NULL;                              \
  }                                                                          \
                                                                             \
  static inline Type *_TCSl##Type##_front(TCSlist *list) {                   \
    return _TCSl##Type##_at(tc_slist_begin(list));                           \
  }                                                                          \
                                                                             \
  static inline TCSlistIter _TCSl##Type##_insert(TCSlistIter iter, Type x) { \
    _TCSl##Type *node = _TCSl##Type##_new();                                 \
    node->data = x;                                                          \
    return _tc_slist_insert(iter, &node->pos);                               \
  }                                                                          \
                                                                             \
  static inline Type *_TCSl##Type##_back(TCSlist *list) {                    \
    return _TCSl##Type##_at(list->_back);                                    \
  }                                                                          \
                                                                             \
  static inline void _TCSl##Type##_push(TCSlist *list, Type x) {             \
    list->_back = _TCSl##Type##_insert(list->_back, x);                      \
  }                                                                          \
                                                                             \
  static inline void _TCSl##Type##_unshift(TCSlist *list, Type x) {          \
    unsigned char is_empty = tc_slist_empty(list);                           \
    _TCSl##Type##_insert(&list->_st, x);                                     \
    if (is_empty) {                                                          \
      list->_back = tc_slist_begin(list);                                    \
    }                                                                        \
  }

/*
 * Instrusive singly linked list constructor
 */
#define _TCSlistNode(Node, Mem)                                         \
  typedef Node _TCSl##Node;                                             \
                                                                        \
  /* clang-format off */                                      \
  _TCSlistBase(_TCSl##Node, Mem)                               \
                                                              \
  static inline Node *_TCSl##Node##_at(TCSlistIter iter) {         \
    /* clang-format on */                                               \
    return __TCSl##Node##_at(iter);                                     \
  }                                                                     \
                                                                        \
  static inline Node *_TCSl##Node##_front(TCSlist *list) {              \
    return _TCSl##Node##_at(tc_slist_begin(list));                      \
  }                                                                     \
                                                                        \
  static inline Node *_TCSl##Node##_back(TCSlist *list) {               \
    return _TCSl##Node##_at(list->_back);                               \
  }                                                                     \
                                                                        \
  static inline TCSlistIter _TCSl##Node##_insert(TCSlistIter iter,      \
                                                 Node *node) {          \
    return _tc_slist_insert(iter, &node->Mem);                          \
  }                                                                     \
                                                                        \
  static inline void _TCSl##Node##_push(TCSlist *list, Node *node) {    \
    list->_back = _TCSl##Node##_insert(list->_back, node);              \
  }                                                                     \
                                                                        \
  static inline void _TCSl##Node##_unshift(TCSlist *list, Node *node) { \
    unsigned char is_empty = tc_slist_empty(list);                      \
    _TCSl##Node##_insert(&list->_st, node);                             \
    if (is_empty) {                                                     \
      list->_back = tc_slist_begin(list);                               \
    }                                                                   \
  }

#define TCSlistOf(...) \
  tc_args_of(_1, _2, _3, __VA_ARGS__, _TCSlistNode, _TCSlistType)(__VA_ARGS__)

#endif
