#include "slist.h"

inline TCSlist *_tc_slist_new(malloc_f _malloc) {
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

#ifdef TANC_UT_ON

#include "utest.h"

typedef struct {
  int v;
} ObjA;

typedef struct {
  int v;
  TCSlistPos x;
} ObjB;

TCSlistOf(int);
TCSlistOf(ObjA);
TCSlistOf(ObjB, x);

void list_test(UTState *ut_state) {
  TCSlistIter iter;
  TCSlist *list1 = tc_slist_new();
  TCSlist *list2 = tc_slist_new();
  TCSlist *list3 = tc_slist_new();

  tc_ut("new list", {
    tc_ut_assert(tc_slist_empty(list1));
    tc_ut_assert(tc_slist_empty(list2));
    tc_ut_assert(tc_slist_empty(list3));
  });

  tc_ut("push node", {
    tc_slist_push(list1, int, 1);
    tc_ut_assert(!tc_slist_empty(list1));
    tc_ut_assert(*tc_slist_front(list1, int) == 1);
    tc_ut_assert(*tc_slist_back(list1, int) == 1);

    tc_slist_push(list2, ObjA, (ObjA){.v = 1});
    tc_ut_assert(!tc_slist_empty(list2));
    tc_ut_assert(tc_slist_front(list2, ObjA)->v == 1);
    tc_ut_assert(tc_slist_back(list2, ObjA)->v == 1);

    ObjB *ob = (ObjB *)malloc(sizeof(ObjB));
    ob->v = 1;
    tc_slist_push(list3, ObjB, ob);
    tc_ut_assert(!tc_slist_empty(list3));
    tc_ut_assert(tc_slist_front(list3, ObjB)->v == 1);
    tc_ut_assert(tc_slist_back(list3, ObjB)->v == 1);
  });

  tc_ut("unshift node", {
    tc_slist_unshift(list1, int, 2);
    tc_ut_assert(*tc_slist_front(list1, int) == 2);
    tc_ut_assert(*tc_slist_back(list1, int) == 1);

    tc_slist_unshift(list2, ObjA, (ObjA){.v = 2});
    tc_ut_assert(tc_slist_front(list2, ObjA)->v == 2);
    tc_ut_assert(tc_slist_back(list2, ObjA)->v == 1);

    ObjB *ob = (ObjB *)malloc(sizeof(ObjB));
    ob->v = 2;
    tc_slist_unshift(list3, ObjB, ob);
    tc_ut_assert(tc_slist_front(list3, ObjB)->v == 2);
    tc_ut_assert(tc_slist_back(list3, ObjB)->v == 1);
  });

  tc_ut("insert node", {
    iter = tc_slist_next(tc_slist_begin(list1));
    iter = tc_slist_insert(iter, int, 3);
    tc_ut_assert(*tc_slist_at(iter, int) == 3);

    iter = tc_slist_next(tc_slist_begin(list2));
    iter = tc_slist_insert(iter, ObjA, (ObjA){.v = 3});
    tc_ut_assert(tc_slist_at(iter, ObjA)->v == 3);

    ObjB *ob = (ObjB *)malloc(sizeof(ObjB));
    ob->v = 3;
    iter = tc_slist_next(tc_slist_begin(list3));
    iter = tc_slist_insert(iter, ObjB, ob);
    tc_ut_assert(tc_slist_at(iter, ObjB)->v == 3);
  });

  tc_ut("iterate list from head to tail", {
    int i = 0;
    int expected[] = {2, 1, 3};
    TCSlistIter begin = tc_slist_begin(list1);

    tc_slist_each(begin, NULL, cur) {
      tc_ut_assert(*tc_slist_at(cur, int) == expected[i++]);
    }

    i = 0;
    begin = tc_slist_begin(list2);

    tc_slist_each(begin, NULL, cur) {
      tc_ut_assert(tc_slist_at(cur, ObjA)->v == expected[i++]);
    }

    i = 0;
    begin = tc_slist_begin(list3);

    tc_slist_each(begin, NULL, cur) {
      tc_ut_assert(tc_slist_at(cur, ObjB)->v == expected[i++]);
    }
  });

  tc_ut("clear list", {
    tc_slist_clear(list1, int);
    TCSlistIter begin = tc_slist_begin(list1);

    tc_ut_assert(tc_slist_empty(list1));
    tc_slist_each(begin, NULL, cur) { tc_ut_assert(0); }

    tc_slist_clear(list2, ObjA);
    begin = tc_slist_begin(list2);

    tc_ut_assert(tc_slist_empty(list2));
    tc_slist_each(begin, NULL, cur) { tc_ut_assert(0); }

    tc_slist_clear(list3, ObjB);
    begin = tc_slist_begin(list3);

    tc_ut_assert(tc_slist_empty(list3));
    tc_slist_each(begin, NULL, cur) { tc_ut_assert(0); }
  });

  tc_ut("free list", {
    tc_slist_free(list1, int);
    list1 = NULL;
    tc_ut_assert(tc_slist_empty(list1));

    tc_slist_free(list2, ObjA);
    list2 = NULL;
    tc_ut_assert(tc_slist_empty(list2));

    tc_slist_free(list3, ObjB);
    list3 = NULL;
    tc_ut_assert(tc_slist_empty(list3));
  });
}

int main() {
  tc_ut_run(list_test);
  return 0;
}
#endif
