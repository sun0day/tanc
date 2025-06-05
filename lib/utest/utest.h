/*
 * @version 1.0.0
 * @copyright 2025 sun0day
 * @description Unit test utils for C
 */

#ifndef TANC_UTEST_H
#define TANC_UTEST_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "list.h"
#include "common.h"

#ifndef TANC_UT_ON
#define TANC_UT_ON
#endif

/*
 * Test case assert result
 */
typedef struct TCAssertResult {
  unsigned int lno;
  char *name;
  unsigned char passed;
} TCAssertRt;
TCListOf(TCAssertRt);

/*
 * Function mock data
 */
typedef struct {
  void *fn_ptr;
  TCList *data;
  TCList *param;
  unsigned int call_num;
} _TCMockData;
TCListOf(_TCMockData);

/*
 * All test cases assert results in a file
 */
typedef struct TCTestState {
  char *file;
  char *name;
  TCList *assert_rt;
  unsigned char passed;
} UTState;

TCListOf(_tc_void_ptr);

/*
 * Test case execution handler
 */
typedef void (*tc_ut_handler)(UTState *);

extern void *tc_ut_malloc(size_t);
extern void tc_ut_free(void *);
extern void _tc_ut_run(tc_ut_handler *, size_t);
extern void _tc_ut_fs(UTState *, char *);
extern void _tc_ut(UTState *, char *);
extern void _tc_ut_out(UTState *);
extern void *_tc_ut_mock(void *);
extern void _tc_ut_return(void *, void *, unsigned char);
extern void _tc_ut_assert(UTState *, unsigned int, unsigned char);
extern unsigned char _tc_ut_assert_called(void *, unsigned int num);

/*
 * Run test handlers
 */
#define tc_ut_run(...)                                                    \
  do {                                                                    \
    tc_ut_handler ut_handlers[] = {__VA_ARGS__};                          \
    _tc_ut_run(ut_handlers, sizeof(ut_handlers) / sizeof(tc_ut_handler)); \
  } while (0);

/*
 * Run test case
 */
#define tc_ut(name, ...)           \
  do {                             \
    _tc_ut_fs(ut_state, __FILE__); \
    _tc_ut(ut_state, name);        \
    __VA_ARGS__                    \
  } while (0);

#define tc_ut_mock(returns, fn, ...)           \
  typedef returns (*_##fn##_ptr)(__VA_ARGS__); \
                                               \
  returns fn(__VA_ARGS__) { return *(returns *)_tc_ut_mock((void *)&fn); }

#define tc_ut_return(fn, data) _tc_ut_return((void *)&fn, data, 1)

/*
 * Detect whether a expression is truthy, abort if falsy
 */
// clang-format off
#define tc_ut_assert(expr) do { unsigned int lno = __LINE__; \
   _tc_ut_fs(ut_state, __FILE__); \
   _tc_ut_assert(ut_state, lno, expr); \
  } while(0);

#define tc_ut_assert_called(fn, num) tc_ut_assert(_tc_ut_assert_called((void *)&fn, num))

#endif
