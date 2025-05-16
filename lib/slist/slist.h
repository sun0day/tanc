/*
 * @version 0.1.0
 * @copyright 2025 sun0day
 * @description Singly linked list
 */

#ifndef TANC_SLIST_H
#define TANC_SLIST_H

#include <stdint.h>

#include "macro.h"

typedef uintptr_t TCSlistPos;

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
#define _TCSlistWrap(Name, Node, Prop)                              \
  typedef TCSlist Name;                                             \
                                                                    \
  static inline Node *Node##_new() {                                \
    Node *node = (Node *)_tc_slist_alloc(sizeof(Node));             \
    node->Prop = (uintptr_t)NULL;                                   \
    return node;                                                    \
  }                                                                 \
                                                                    \
  static void Name##_clear(Name *list) {                            \
    if (list != NULL) {                                             \
      TCSlistIter next;                                             \
      TCSlistIter cur = tc_slist_begin(list);                       \
      while (cur != NULL) {                                         \
        next = tc_slist_next(cur);                                  \
                                                                    \
        Node *node = tc_container_of(cur, Node, Prop);              \
        _tc_slist_free(node);                                       \
                                                                    \
        cur = next;                                                 \
      }                                                             \
                                                                    \
      list->_st = (uintptr_t)NULL;                                  \
      list->_back = &list->_st;                                     \
    }                                                               \
  }                                                                 \
                                                                    \
  static void Name##_free(Name *list) {                             \
    if (list != NULL) {                                             \
      Name##_clear(list);                                           \
                                                                    \
      _tc_slist_free(list);                                         \
    }                                                               \
  }                                                                 \
                                                                    \
  static inline Node *_##Name##_at(TCSlistIter iter) {              \
    return iter != NULL ? tc_container_of(iter, Node, Prop) : NULL; \
  }

/*
 * Singly linked list constructor
 */
#define _TCSlistType(Name, Type)                                      \
  typedef struct {                                                    \
    Type data;                                                        \
    TCSlistPos pos;                                                   \
  } Name##Node;                                                       \
                                                                      \
  /* clang-format off */                                     \
   _TCSlistWrap(Name, Name##Node, pos)                         \
                                                              \
   static inline Type *Name##_at(TCSlistIter iter) {        \
    /* clang-format on */                                             \
    Name##Node *node = _##Name##_at(iter);                            \
    return node != NULL ? &(node->data) : NULL;                       \
  }                                                                   \
                                                                      \
  static inline TCSlistIter Name##_insert(TCSlistIter iter, Type x) { \
    Name##Node *node = Name##Node_new();                              \
    node->data = x;                                                   \
    return _tc_slist_insert(iter, &node->pos);                        \
  }                                                                   \
                                                                      \
  static inline Type *Name##_front(Name *list) {                      \
    return Name##_at(tc_slist_begin(list));                           \
  }                                                                   \
                                                                      \
  static inline Type *Name##_back(Name *list) {                       \
    return Name##_at(list->_back);                                    \
  }                                                                   \
                                                                      \
  static inline void Name##_push(Name *list, Type x) {                \
    list->_back = Name##_insert(list->_back, x);                      \
  }                                                                   \
                                                                      \
  static inline void Name##_unshift(Name *list, Type x) {             \
    unsigned char is_empty = tc_slist_empty(list);                    \
    Name##_insert(&list->_st, x);                                     \
    if (is_empty) {                                                   \
      list->_back = tc_slist_begin(list);                             \
    }                                                                 \
  }

/*
 * Instrusive singly linked list constructor
 */
#define _TCSlistInsv(Name, Node, Prop)                                    \
  /* clang-format off */                                      \
   _TCSlistWrap(Name, Node, Prop)                               \
                                                               \
   static inline Node *Name##_at(TCSlistIter iter) {           \
    /* clang-format on */                                                 \
    return _##Name##_at(iter);                                            \
  }                                                                       \
                                                                          \
  static inline Node *Name##_front(Name *list) {                          \
    return Name##_at(tc_slist_begin(list));                               \
  }                                                                       \
                                                                          \
  static inline Node *Name##_back(Name *list) {                           \
    return Name##_at(list->_back);                                        \
  }                                                                       \
                                                                          \
  static inline TCSlistIter Name##_insert(TCSlistIter iter, Node *node) { \
    return _tc_slist_insert(iter, &node->Prop);                           \
  }                                                                       \
                                                                          \
  static inline void Name##_push(Name *list, Node *node) {                \
    list->_back = Name##_insert(list->_back, node);                       \
  }                                                                       \
                                                                          \
  static inline void Name##_unshift(Name *list, Node *node) {             \
    unsigned char is_empty = tc_slist_empty(list);                        \
    Name##_insert(&list->_st, node);                                      \
    if (is_empty) {                                                       \
      list->_back = tc_slist_begin(list);                                 \
    }                                                                     \
  }

#define TCSlistOf(...) \
  tc_args_of(_1, _2, __VA_ARGS__, _TCSlistInsv, _TCSlistType)(__VA_ARGS__)

#endif
