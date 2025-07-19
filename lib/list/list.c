#include "list.h"

inline TCList *_tc_list_new(_tc_malloc_ptr _malloc) {
  TCList *list = (TCList *)_malloc(sizeof(TCList));

  list->_st.prev = &list->_st;
  list->_st.next = &list->_st;

  return list;
}

inline unsigned char tc_list_empty(TCList *list) {
  if (list != NULL) {
    return list->_st.next == list->_st.prev & list->_st.next == &list->_st;
  }
  return 1;
}

TCListIter _tc_list_insert(TCListIter iter, TCListIter target) {
  target->next = iter;
  target->prev = iter->prev;

  if (iter->prev) {
    iter->prev->next = target;
  }

  iter->prev = target;
  return target;
}

inline TCListIter tc_list_end(TCList *list) {
  return list != NULL ? &list->_st : NULL;
}

inline TCListIter tc_list_next(TCListIter iter) {
  return iter != NULL ? iter->next : NULL;
}

inline TCListIter tc_list_prev(TCListIter iter) {
  return iter != NULL ? iter->prev : NULL;
}