#include "slist.h"

inline TCSlist *_tc_slist_new(_tc_malloc_ptr _malloc) {
  TCSlist *list = (TCSlist *)_malloc(sizeof(TCSlist));

  list->_st = NULL;
  list->_back = &list->_st;

  return list;
}

inline unsigned char tc_slist_empty(TCSlist *list) {
  return list == NULL || list->_st == NULL;
}

TCSlistIter _tc_slist_insert(TCSlistIter iter, TCSlistIter target) {
  if (iter != NULL && target != NULL) {
    *target = *iter;
    *iter = target;

    return target;
  }
  return NULL;
}

inline TCSlistIter tc_slist_begin(TCSlist *list) {
  return list != NULL ? list->_st : NULL;
}

inline TCSlistIter tc_slist_next(TCSlistIter iter) {
  return iter != NULL ? *iter : NULL;
}