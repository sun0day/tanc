#include "slist.h"

inline TCSlist *_tc_slist_new(malloc_f _malloc) {
  TCSlist *list = (TCSlist *)_malloc(sizeof(TCSlist));

  list->_st.next = NULL;
  list->_back = &list->_st;

  return list;
}

inline unsigned char tc_slist_empty(TCSlist *list) {
  return list == NULL || list->_st.next == NULL;
}

TCSlistIter _tc_slist_insert(TCSlistIter iter, TCSlistIter target) {
  if (iter != NULL && target != NULL) {
    target->next = iter->next;
    iter->next = target;

    return target;
  }
  return NULL;
}

inline TCSlistIter tc_slist_begin(TCSlist *list) {
  return list != NULL ? tc_slist_next(&list->_st) : NULL;
}

inline TCSlistIter tc_slist_next(TCSlistIter iter) {
  return iter != NULL ? iter->next : NULL;
}

#ifdef TANC_UT_ON

#include "utest.h"

typedef struct {
  int v;
} Obj;

typedef struct {
  int v;
  TCSlistPos x;
} InsvObj;

TCSlistOf(IntList, int);
TCSlistOf(ObjList, Obj);
TCSlistOf(InsvObjs, InsvObj, x);

void list_test(UTState *ut_state) {
  TCSlistIter iter;
  IntList *list1 = tc_slist_new();
  ObjList *list2 = tc_slist_new();
  InsvObjs *list3 = tc_slist_new();

  tc_ut("new list", {
    tc_ut_assert(tc_slist_empty(list1));
    tc_ut_assert(tc_slist_empty(list2));
    tc_ut_assert(tc_slist_empty(list3));
  });

  tc_ut("push node", {
    IntList_push(list1, 1);
    tc_ut_assert(!tc_slist_empty(list1));
    tc_ut_assert(*IntList_front(list1) == 1);
    tc_ut_assert(*IntList_back(list1) == 1);

    ObjList_push(list2, (Obj){.v = 1});
    tc_ut_assert(!tc_slist_empty(list2));
    tc_ut_assert(ObjList_front(list2)->v == 1);
    tc_ut_assert(ObjList_back(list2)->v == 1);

    InsvObj *insv_obj = InsvObj_new();
    insv_obj->v = 1;
    InsvObjs_push(list3, insv_obj);
    tc_ut_assert(!tc_slist_empty(list3));
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
    iter = tc_slist_next(tc_slist_begin(list1));
    iter = IntList_insert(iter, 3);
    tc_ut_assert(*IntList_at(iter) == 3);

    iter = tc_slist_next(tc_slist_begin(list2));
    iter = ObjList_insert(iter, (Obj){.v = 3});
    tc_ut_assert(ObjList_at(iter)->v == 3);

    InsvObj *insv_obj = InsvObj_new();
    insv_obj->v = 3;
    iter = tc_slist_next(tc_slist_begin(list3));
    iter = InsvObjs_insert(iter, insv_obj);
    tc_ut_assert(InsvObjs_at(iter)->v == 3);
  });

  tc_ut("iterate list from head to tail", {
    int i = 0;
    int expected[] = {2, 1, 3};
    TCSlistIter begin = tc_slist_begin(list1);

    tc_slist_each(begin, NULL, cur) {
      tc_ut_assert(*IntList_at(cur) == expected[i++]);
    }

    i = 0;
    begin = tc_slist_begin(list2);

    tc_slist_each(begin, NULL, cur) {
      tc_ut_assert(ObjList_at(cur)->v == expected[i++]);
    }

    i = 0;
    begin = tc_slist_begin(list3);

    tc_slist_each(begin, NULL, cur) {
      tc_ut_assert(InsvObjs_at(cur)->v == expected[i++]);
    }
  });

  tc_ut("clear list", {
    IntList_clear(list1);
    TCSlistIter begin = tc_slist_begin(list1);

    tc_ut_assert(tc_slist_empty(list1));
    tc_slist_each(begin, NULL, cur) { tc_ut_assert(0); }

    ObjList_clear(list2);
    begin = tc_slist_begin(list2);

    tc_ut_assert(tc_slist_empty(list2));
    tc_slist_each(begin, NULL, cur) { tc_ut_assert(0); }

    InsvObjs_clear(list3);
    begin = tc_slist_begin(list3);

    tc_ut_assert(tc_slist_empty(list3));
    tc_slist_each(begin, NULL, cur) { tc_ut_assert(0); }
  });

  tc_ut("free list", {
    IntList_free(list1);
    list1 = NULL;
    tc_ut_assert(tc_slist_empty(list1));

    ObjList_free(list2);
    list2 = NULL;
    tc_ut_assert(tc_slist_empty(list2));

    InsvObjs_free(list3);
    list3 = NULL;
    tc_ut_assert(tc_slist_empty(list3));
  });
}

int main() {
  tc_ut_run(list_test);
  return 0;
}
#endif
