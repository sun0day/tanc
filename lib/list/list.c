#include "list.h"

#include <stddef.h>

inline _TCListNode *_tc_list_node_new(malloc_f _malloc) {
  return (_TCListNode *)_malloc(sizeof(_TCListNode));
}

_TCList *_tc_list_new(malloc_f _malloc) {
  _TCList *list = (_TCList *)_malloc(sizeof(_TCList));
  list->head = _tc_list_node_new(_malloc);
  list->tail = _tc_list_node_new(_malloc);

  list->head->prev = NULL;
  list->head->next = list->tail;

  list->tail->next = NULL;
  list->tail->prev = list->head;

  list->size = 0;

  return list;
}

// push data to the end of a list
void _tc_list_push_back(_TCList *list, void *x, malloc_f _malloc) {
  _TCListNode *node = _tc_list_node_new(_malloc);
  node->data = x;

  node->prev = list->tail->prev;
  node->prev->next = node;
  node->next = list->tail;
  list->tail->prev = node;

  list->size++;
}

inline TCListIter _tc_list_begin(_TCList *list) {
  return (TCListIter){.cur = list->head->next};
}

// detect whether a iterator is still valid
inline unsigned char tc_list_iter_valid(TCListIter iter) {
  return iter.cur != NULL & iter.cur->next != NULL & iter.cur->prev != NULL;
}

inline void *tc_list_iter_get(TCListIter iter) { return iter.cur->data; }

inline void tc_list_iter_next(TCListIter *iter) {
  if (iter->cur != NULL) iter->cur = iter->cur->next;
}

#ifdef TANC_UT_RUN

#include <utest/utest.h>

void list_test(UTState *ut_state) {
  tc_ut("ooooo", { tc_ut_assert(1); })
}

int main() {
  tc_ut_run(list_test);
  return 0;
}
#endif
