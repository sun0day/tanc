#include "list.h"

#include <stddef.h>

// detect whether a iterator is still valid
inline unsigned char tc_list_iter_valid(TCListIter iter) {
  return (iter != NULL) & (iter->next != NULL) & (iter->prev != NULL);
}

inline TCListIter tc_list_next(TCListIter iter) {
  return iter == NULL ? NULL : iter->next;
}

inline TCListIter tc_list_prev(TCListIter iter) {
  return iter == NULL ? NULL : iter->prev;
}

#ifdef TANC_UT_ON

#include <utest/utest.h>

typedef struct {
  int v;
} Obj;

typedef struct {
  int v;
  TCListNode x;
} InsvObj;

TCLinkedList(IntList, int);
TCLinkedList(ObjList, Obj);
TCInsvList(InsvObjs, InsvObj, x);

void list_test(UTState *ut_state) {
  TCListIter iter;
  IntList *list1 = IntList_new();
  ObjList *list2 = ObjList_new();
  InsvObjs *list3 = InsvObjs_new();

  tc_ut("new list", {
    tc_ut_assert(IntList_empty(list1));
    tc_ut_assert(ObjList_empty(list2));
    tc_ut_assert(InsvObjs_empty(list3));
  });

  tc_ut("push node", {
    IntList_push(list1, 1);
    tc_ut_assert(!IntList_empty(list1));
    tc_ut_assert(*IntList_front(list1) == 1);
    tc_ut_assert(*IntList_back(list1) == 1);

    ObjList_push(list2, (Obj){.v = 1});
    tc_ut_assert(!ObjList_empty(list2));
    tc_ut_assert(ObjList_front(list2)->v == 1);
    tc_ut_assert(ObjList_back(list2)->v == 1);

    InsvObj *insv_obj = InsvObj_new();
    insv_obj->v = 1;
    InsvObjs_push(list3, insv_obj);
    tc_ut_assert(!InsvObjs_empty(list3));
    tc_ut_assert(InsvObjs_front(list3)->v == 1);
    tc_ut_assert(InsvObjs_back(list3)->v == 1);
  });

  tc_ut("unshift node", {
    IntList_unshift(list1, 2);
    tc_ut_assert(*IntList_front(list1) == 2);
    tc_ut_assert(*IntList_back(list1) == 1);

    ObjList_unshift(list2, (Obj){.v = 2});
    tc_ut_assert(ObjList_front(list2)->v == 2);
    tc_ut_assert(ObjList_back(list2)->v == 1);

    InsvObj *insv_obj = InsvObj_new();
    insv_obj->v = 2;
    InsvObjs_unshift(list3, insv_obj);
    tc_ut_assert(InsvObjs_front(list3)->v == 2);
    tc_ut_assert(InsvObjs_back(list3)->v == 1);
  });

  tc_ut("insert node", {
    iter = tc_list_next(tc_list_begin(list1));
    iter = IntList_insert(iter, 3);
    tc_ut_assert(*IntList_at(iter) == 3);

    iter = tc_list_next(tc_list_begin(list2));
    iter = ObjList_insert(iter, (Obj){.v = 3});
    tc_ut_assert(ObjList_at(iter)->v == 3);

    InsvObj *insv_obj = InsvObj_new();
    insv_obj->v = 3;
    iter = tc_list_next(tc_list_begin(list2));
    iter = InsvObjs_insert(iter, insv_obj);
    tc_ut_assert(InsvObjs_at(iter)->v == 3);
  });
}

int main() {
  tc_ut_run(list_test);
  return 0;
}
#endif
