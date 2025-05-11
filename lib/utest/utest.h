/*
 * @version 1.0.0
 * @copyright 2025 sun0day
 * @description Unit test utils for C
 */

#ifndef TANC_UTEST_H
#define TANC_UTEST_H

#ifndef TANC_UT_ON
#define TANC_UT_ON
#endif

#include <list/list.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct TCAssertResult {
  int lno;
  char *name;
  unsigned char passed;
} TCAssertRt;
TCLinkedList(TCAssertRtList, TCAssertRt);

typedef struct TCTestState {
  char *file;
  char *name;
  TCAssertRtList *assert_rt;
  unsigned char passed;
} UTState;

typedef void (*tc_ut_handler)(UTState *);

extern void _tc_ut_run(tc_ut_handler *, size_t);
extern void _tc_ut_fs(UTState *, char *);
extern void _tc_ut(UTState *, char *);
extern void _tc_ut_assert(UTState *, int, unsigned char);
extern void _tc_ut_out(UTState *);

// run test handlers
#define tc_ut_run(...)                                                    \
  do {                                                                    \
    tc_ut_handler ut_handlers[] = {__VA_ARGS__};                          \
    _tc_ut_run(ut_handlers, sizeof(ut_handlers) / sizeof(tc_ut_handler)); \
  } while (0);

// assert if expression is falsy
// clang-format off
#define tc_ut_assert(expr) do { int lno = __LINE__; \
   _tc_ut_fs(ut_state, __FILE__); \
   _tc_ut_assert(ut_state, lno, expr); \
  } while(0);

// run test case
#define tc_ut(name, block)        \
  do {                             \
    _tc_ut_fs(ut_state, __FILE__); \
    _tc_ut(ut_state, name);       \
    block                          \
  } while (0);

#endif
