/*
 * @version 1.0.0
 * @copyright 2025 sun0day
 * @description Unit test utils for C
 */

#ifndef TANC_UTEST_H
#define TANC_UTEST_H

#include <stddef.h>

typedef void (*tc_utest_handler)();

extern void  _tc_utest_run(tc_utest_handler *, size_t);

#define tc_utest_run(...) do { \
  tc_utest_handler utest_handlers[] = { __VA_ARGS__ }; \
  _tc_utest_run(utest_handlers, sizeof(utest_handlers) / sizeof(tc_utest_handler)); \
} while(0)

#endif
