#include "utest.h"

#define TCAllocator tc_ut_malloc, tc_ut_free

#include "slist.h"

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

void list_test(TCUtState *ut_state) {
  TCSlistIter iter;
  TCSlist *list1 = tc_slist_new();
  TCSlist *list2 = tc_slist_new();
  TCSlist *list3 = tc_slist_new();

  tc_ut("new list", {
    tc_ut_assert(tc_slist_empty(list1));
    tc_ut_assert(tc_slist_empty(list2));
    tc_ut_assert(tc_slist_empty(list3));
    tc_ut_assert_called(malloc, 3);
  });

  tc_ut("append node", {
    tc_slist_append(list1, int, 1);
    tc_ut_assert(!tc_slist_empty(list1));
    tc_ut_assert(*tc_slist_front(list1, int) == 1);
    tc_ut_assert(*tc_slist_back(list1, int) == 1);

    tc_slist_append(list2, ObjA, (ObjA){.v = 1});
    tc_ut_assert(!tc_slist_empty(list2));
    tc_ut_assert(tc_slist_front(list2, ObjA)->v == 1);
    tc_ut_assert(tc_slist_back(list2, ObjA)->v == 1);

    ObjB *ob = (ObjB *)malloc(sizeof(ObjB));
    ob->v = 1;
    tc_slist_append(list3, ObjB, ob);
    tc_ut_assert(!tc_slist_empty(list3));
    tc_ut_assert(tc_slist_front(list3, ObjB)->v == 1);
    tc_ut_assert(tc_slist_back(list3, ObjB)->v == 1);
  });

  tc_ut("prepend node", {
    tc_slist_prepend(list1, int, 2);
    tc_ut_assert(*tc_slist_front(list1, int) == 2);
    tc_ut_assert(*tc_slist_back(list1, int) == 1);

    tc_slist_prepend(list2, ObjA, (ObjA){.v = 2});
    tc_ut_assert(tc_slist_front(list2, ObjA)->v == 2);
    tc_ut_assert(tc_slist_back(list2, ObjA)->v == 1);

    ObjB *ob = (ObjB *)malloc(sizeof(ObjB));
    ob->v = 2;
    tc_slist_prepend(list3, ObjB, ob);
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
    tc_ut_assert_called(free, 3);

    TCSlistIter begin = tc_slist_begin(list1);

    tc_ut_assert(tc_slist_empty(list1));
    tc_slist_each(begin, NULL, cur) { tc_ut_assert(0); }

    tc_ut_mock_clear(free);
    tc_slist_clear(list2, ObjA);
    tc_ut_assert_called(free, 3);

    begin = tc_slist_begin(list2);

    tc_ut_assert(tc_slist_empty(list2));
    tc_slist_each(begin, NULL, cur) { tc_ut_assert(0); }

    tc_ut_mock_clear(free);
    tc_slist_clear(list3, ObjB);
    tc_ut_assert_called(free, 3);

    begin = tc_slist_begin(list3);

    tc_ut_assert(tc_slist_empty(list3));
    tc_slist_each(begin, NULL, cur) { tc_ut_assert(0); }
  });

  tc_ut("free list", {
    tc_ut_mock_clear(free);
    tc_slist_free(list1, int);
    tc_ut_assert_called(free, 1);
    tc_ut_assert(tc_slist_empty(list1));

    tc_ut_mock_clear(free);
    tc_slist_free(list2, ObjA);
    tc_ut_assert_called(free, 1);
    tc_ut_assert(tc_slist_empty(list2));

    tc_ut_mock_clear(free);
    tc_slist_free(list3, ObjB);
    tc_ut_assert_called(free, 1);
    tc_ut_assert(tc_slist_empty(list3));
  });
}

int main() {
  tc_ut_run(list_test);
  return 0;
}