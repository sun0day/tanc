#include "utest.h"

#define TCAllocator tc_ut_malloc, tc_ut_free

#include "list.h"

typedef struct {
  int v;
} ObjA;

typedef struct {
  int v;
  TCListPos x;
} ObjB;

TCListOf(int);
TCListOf(ObjA);
TCListOf(ObjB, x);

void list_test(TCUtState *ut_state) {
  TCListIter iter;
  TCList *list1 = tc_list_new();
  TCList *list2 = tc_list_new();
  TCList *list3 = tc_list_new();

  tc_ut("new list", {
    tc_ut_assert(tc_list_empty(list1));
    tc_ut_assert(tc_list_empty(list2));
    tc_ut_assert(tc_list_empty(list3));
    tc_ut_assert_called(malloc, 3);
  });

  tc_ut("push node", {
    tc_list_push(list1, int, 1);
    tc_ut_assert(!tc_list_empty(list1));
    tc_ut_assert(*tc_list_front(list1, int) == 1);
    tc_ut_assert(*tc_list_back(list1, int) == 1);

    tc_list_push(list2, ObjA, (ObjA){.v = 1});
    tc_ut_assert(!tc_list_empty(list2));
    tc_ut_assert(tc_list_front(list2, ObjA)->v == 1);
    tc_ut_assert(tc_list_back(list2, ObjA)->v == 1);

    ObjB *ob = (ObjB *)malloc(sizeof(ObjB));
    ob->v = 1;
    tc_list_push(list3, ObjB, ob);
    tc_ut_assert(!tc_list_empty(list3));
    tc_ut_assert(tc_list_front(list3, ObjB)->v == 1);
    tc_ut_assert(tc_list_back(list3, ObjB)->v == 1);
  });

  tc_ut("unshift node", {
    tc_list_unshift(list1, int, 2);
    tc_ut_assert(*tc_list_front(list1, int) == 2);
    tc_ut_assert(*tc_list_back(list1, int) == 1);

    tc_list_unshift(list2, ObjA, (ObjA){.v = 2});
    tc_ut_assert(tc_list_front(list2, ObjA)->v == 2);
    tc_ut_assert(tc_list_back(list2, ObjA)->v == 1);

    ObjB *ob = (ObjB *)malloc(sizeof(ObjB));
    ob->v = 2;
    tc_list_unshift(list3, ObjB, ob);
    tc_ut_assert(tc_list_front(list3, ObjB)->v == 2);
    tc_ut_assert(tc_list_back(list3, ObjB)->v == 1);
  });

  tc_ut("insert node", {
    iter = tc_list_next(tc_list_begin(list1));
    iter = tc_list_insert(iter, int, 3);
    tc_ut_assert(*tc_list_at(iter, int) == 3);

    iter = tc_list_next(tc_list_begin(list2));
    iter = tc_list_insert(iter, ObjA, (ObjA){.v = 3});
    tc_ut_assert(tc_list_at(iter, ObjA)->v == 3);

    ObjB *ob = (ObjB *)malloc(sizeof(ObjB));
    ob->v = 3;
    iter = tc_list_next(tc_list_begin(list3));
    iter = tc_list_insert(iter, ObjB, ob);
    tc_ut_assert(tc_list_at(iter, ObjB)->v == 3);
  });

  tc_ut("iterate list from head to tail", {
    int i = 0;
    int expected[] = {2, 3, 1};
    TCListIter begin = tc_list_begin(list1);
    TCListIter end = tc_list_end(list1);

    tc_list_each(begin, end, cur) {
      tc_ut_assert(*tc_list_at(cur, int) == expected[i++]);
    }

    i = 0;
    begin = tc_list_begin(list2);
    end = tc_list_end(list2);

    tc_list_each(begin, end, cur) {
      tc_ut_assert(tc_list_at(cur, ObjA)->v == expected[i++]);
    }

    i = 0;
    begin = tc_list_begin(list3);
    end = tc_list_end(list3);

    tc_list_each(begin, end, cur) {
      tc_ut_assert(tc_list_at(cur, ObjB)->v == expected[i++]);
    }
  });

  tc_ut("iterate list from tail to head", {
    int i = 2;
    int expected[] = {2, 3, 1};
    TCListIter begin = tc_list_rbegin(list1);
    TCListIter end = tc_list_rend(list1);

    tc_list_reach(begin, end, cur) {
      tc_ut_assert(*tc_list_at(cur, int) == expected[i--]);
    }

    i = 2;
    begin = tc_list_rbegin(list2);
    end = tc_list_rend(list2);

    tc_list_reach(begin, end, cur) {
      tc_ut_assert(tc_list_at(cur, ObjA)->v == expected[i--]);
    }

    i = 2;
    begin = tc_list_rbegin(list3);
    end = tc_list_rend(list3);

    tc_list_reach(begin, end, cur) {
      tc_ut_assert(tc_list_at(cur, ObjB)->v == expected[i--]);
    }
  });

  tc_ut("clear list", {
    tc_list_clear(list1, int);
    tc_ut_assert_called(free, 3);

    TCListIter begin = tc_list_begin(list1);
    TCListIter end = tc_list_end(list1);

    tc_ut_assert(tc_list_empty(list1));
    tc_list_each(begin, end, cur) { tc_ut_assert(0); }

    tc_list_clear(list2, ObjA);
    tc_ut_assert_called(free, 6);

    begin = tc_list_begin(list2);
    end = tc_list_end(list2);

    tc_ut_assert(tc_list_empty(list2));
    tc_list_each(begin, end, cur) { tc_ut_assert(0); }

    tc_list_clear(list3, ObjB);
    tc_ut_assert_called(free, 9);

    begin = tc_list_begin(list3);
    end = tc_list_end(list3);

    tc_ut_assert(tc_list_empty(list3));
    tc_list_each(begin, end, cur) { tc_ut_assert(0); }
  });

  tc_ut("free list", {
    tc_ut_mock_clear(free);
    tc_list_free(list1, int);
    list1 = NULL;

    tc_ut_assert_called(free, 1);
    tc_ut_assert(tc_list_empty(list1));

    tc_list_free(list2, ObjA);
    list2 = NULL;

    tc_ut_assert_called(free, 2);
    tc_ut_assert(tc_list_empty(list2));

    tc_list_free(list3, ObjB);
    list3 = NULL;

    tc_ut_assert_called(free, 3);
    tc_ut_assert(tc_list_empty(list3));
  });
}

int main() {
  tc_ut_run(list_test);
  return 0;
}