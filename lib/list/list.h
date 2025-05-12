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

extern TCListIter tc_list_next(TCListIter);
extern TCListIter tc_list_prev(TCListIter);

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
 * it's type-unsafe and not applied to NULL ptr
 */
#define tc_container_of(ptr, type, prop) \
  ((type *)((char *)(ptr) - offsetof(type, prop)))

/*
 * Match 1 - 5 variadic arguments
 */
#define tc_args_of(_1, _2, _3, _4, _5, Name, ...) Name

/*
 * Get the sentinel node's iterator
 */
#define tc_list_rend tc_list_end
#define tc_list_end(list) (list == NULL ? NULL : &list->_st)

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
 * Generate double linked list bases
 */
#define _TCListWrap(Name, Node, Prop)                                          \
  typedef struct {                                                             \
    TCListNode _st;                                                            \
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
    list->_st = (TCListNode){.prev = &(list->_st), .next = &(list->_st)};      \
                                                                               \
    return list;                                                               \
  }                                                                            \
                                                                               \
  static TCListIter _##Name##_insert(TCListIter iter, Node *node) {            \
    node->Prop.next = iter;                                                    \
    node->Prop.prev = iter->prev;                                              \
                                                                               \
    if (iter->prev) {                                                          \
      iter->prev->next = &node->Prop;                                          \
    }                                                                          \
                                                                               \
    iter->prev = &node->Prop;                                                  \
    return &node->Prop;                                                        \
  }                                                                            \
                                                                               \
  static inline unsigned char Name##_empty(Name *list) {                       \
    return list == NULL ||                                                     \
           (list->_st.next == list->_st.prev && list->_st.next == &list->_st); \
  }                                                                            \
                                                                               \
  static void Name##_clear(Name *list) {                                       \
    if (list == NULL) {                                                        \
      return;                                                                  \
    }                                                                          \
                                                                               \
    tc_list_each(tc_list_begin(list), tc_list_end(list), iter) {               \
      Node *node = tc_container_of(iter, Node, Prop);                          \
      if (node != NULL) _tc_list_free(node);                                   \
    }                                                                          \
                                                                               \
    list->_st.prev = &list->_st;                                               \
    list->_st.next = &list->_st;                                               \
  }                                                                            \
                                                                               \
  static void Name##_free(Name *list) {                                        \
    if (list == NULL) {                                                        \
      return;                                                                  \
    }                                                                          \
                                                                               \
    Name##_clear(list);                                                        \
                                                                               \
    _tc_list_free(list);                                                       \
  }

/*
 * Double linked list
 */
#define TCLinkedList(Name, Type)                                             \
  typedef struct {                                                           \
    Type data;                                                               \
    TCListNode pos;                                                          \
  } Name##Node;                                                              \
                                                                             \
  /* clang-format off */                                     \
  _TCListWrap(Name, Name##Node, pos)                         \
                                                             \
  static inline Type *Name##_at(TCListIter iter) {               \
    /* clang-format on */                                                    \
    return iter == NULL ? NULL                                               \
                        : &((tc_container_of(iter, Name##Node, pos))->data); \
  }                                                                          \
                                                                             \
  static inline TCListIter Name##_insert(TCListIter iter, Type x) {          \
    Name##Node *node = Name##Node_new();                                     \
    node->data = x;                                                          \
    return _##Name##_insert(iter, node);                                     \
  }                                                                          \
                                                                             \
  static inline Type *Name##_front(Name *list) {                             \
    return Name##_at(list->_st.next);                                        \
  }                                                                          \
                                                                             \
  static inline Type *Name##_back(Name *list) {                              \
    return Name##_at(list->_st.prev);                                        \
  }                                                                          \
                                                                             \
  static inline void Name##_push(Name *list, Type x) {                       \
    Name##_insert(&list->_st, x);                                            \
  }                                                                          \
                                                                             \
  static inline void Name##_unshift(Name *list, Type x) {                    \
    Name##_insert(list->_st.next, x);                                        \
  }

/*
 * Instrusive double linked list
 */
#define TCInsvList(Name, Node, Prop)                                    \
  /* clang-format off */                                      \
  _TCListWrap(Name, Node, Prop)                               \
                                                              \
  static inline Node *Name##_at(TCListIter iter) {         \
    /* clang-format on */                                               \
    return iter == NULL ? NULL : tc_container_of(iter, Node, Prop);     \
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
    return _##Name##_insert(iter, node);                                \
  }                                                                     \
                                                                        \
  static inline void Name##_push(Name *list, Node *node) {              \
    Name##_insert(&list->_st, node);                                    \
  }                                                                     \
                                                                        \
  static inline void Name##_unshift(Name *list, Node *node) {           \
    Name##_insert(list->_st.next, node);                                \
  }

#endif
