/*
 * @version 1.0.0
 * @copyright 2025 sun0day
 * @description Unit test utils for C
 */

#ifndef TANC_UTEST_H
#define TANC_UTEST_H

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "common.h"
#include "list.h"

#ifndef TANC_UT_ON
#define TANC_UT_ON
#endif

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#ifndef ENODATA
#define ENODATA ENOATTR
#endif
#endif

/*
 * Test case assert result
 */
typedef struct _TCAssertResult {
  char *name;
  unsigned int lno;
  unsigned char passed;
} _TCAssertRt;
TCListOf(_TCAssertRt);

/*
 * Function mock data
 */
typedef struct {
  const char *fn;
  TCList *data;
  TCList *args;
  uint32_t call_num;
} _TCMockData;
TCListOf(_TCMockData);

typedef struct {
  char *name;
  void *value;
} _TCMockArg;
TCListOf(_TCMockArg);

/*
 * All test cases assert results in a file
 */
typedef struct TCTestState {
  char *file;
  char *name;
  TCList *assert_rt;
  unsigned char passed;
} TCUtState;

TCListOf(_tc_void_ptr);

/*
 * Test case execution handler
 */
typedef void (*_tc_ut_handler)(TCUtState *);

extern void *tc_ut_malloc(size_t);
extern void tc_ut_free(void *);
extern void _tc_ut_run(_tc_ut_handler *, size_t);
extern void _tc_ut_fs(TCUtState *, char *);
extern void _tc_ut(TCUtState *, char *);
extern void _tc_ut_out(TCUtState *);
extern void *_tc_ut_call(char *);
extern void _tc_ut_mock_clear(char *);
extern void _tc_ut_arg(const char *, char *, void *, size_t);
extern void _tc_ut_return(char *, void *);
extern void _tc_ut_assert(TCUtState *, unsigned int, unsigned char);
extern unsigned char _tc_ut_assert_called(char *, uint32_t);
extern unsigned char _tc_ut_assert_arg_str(const char *, char *, char *);
extern void _tc_ut_mock_arg_clear(_TCMockData *);

/*
 * Run test handlers
 */
#define tc_ut_run(...)                                                     \
  do {                                                                     \
    _tc_ut_handler ut_handlers[] = {__VA_ARGS__};                          \
    _tc_ut_run(ut_handlers, sizeof(ut_handlers) / sizeof(_tc_ut_handler)); \
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

#define tc_ut_mock(returns, fn, ...) \
  returns fn(__VA_ARGS__) { return *(returns *)_tc_ut_call(#fn); }

#define tc_ut_return(fn, data) _tc_ut_return(#fn, (void *)&data)

#define tc_ut_arg_str(value) \
  _tc_ut_arg(__func__, #value, value, value == NULL ? 0 : strlen(value))

/*
 * Detect whether a expression is truthy, abort if falsy
 */
// clang-format off
#define tc_ut_assert(expr) do { unsigned int lno = __LINE__; \
   _tc_ut_fs(ut_state, __FILE__); \
   _tc_ut_assert(ut_state, lno, expr); \
  } while(0);

#define tc_ut_assert_called(fn, num) tc_ut_assert(_tc_ut_assert_called(#fn, num))
#define tc_ut_assert_arg_str(fn, name, value) tc_ut_assert(_tc_ut_assert_arg_str(#fn, #name, (char *)&value))
#define tc_ut_mock_clear(fn) _tc_ut_mock_clear(#fn)

#endif
