/*
 * @version 0.1.0
 * @copyright 2025 sun0day
 * @description Faster printf
 */

#ifndef TANC_PRINT_H
#define TANC_PRINT_H

#include <stdio.h>
#include <string.h>

#include "common.h"

#ifndef _tc_print_alloc
#define _tc_print_alloc _tc_get_alloc(TCAllocator)
#endif

#ifndef _tc_print_free
#define _tc_print_free _tc_get_free(TCAllocator)
#endif

#define tc_print_set_buf(size) \
  _tc_print_set_buf(size, _tc_print_alloc, _tc_print_free)

#define tc_print(stream, ...)                                                \
  do {                                                                       \
    int writen_size = snprintf(_tc_stdio_buf + _tc_stdio_len,                \
                               _tc_stdio_caps - _tc_stdio_len, __VA_ARGS__); \
    _tc_stdio_len = _tc_stdio_len + writen_size;                             \
    if (_tc_stdio_len >= _tc_stdio_thres) {                                  \
      *(_tc_stdio_buf + _tc_stdio_len - writen_size) = '\0';                 \
      tc_print_flush(stream);                                                \
      fprintf(stream, __VA_ARGS__);                                          \
    }                                                                        \
  } while (0);

#define tc_print_flush(stream)                  \
  do {                                          \
    if (_tc_stdio_len > 0) {                    \
      fprintf(stream, "%s", _tc_stdio_buf);     \
      memset(_tc_stdio_buf, 0, _tc_stdio_caps); \
    }                                           \
    _tc_stdio_len = 0;                          \
  } while (0);

#define tc_print_clean() _tc_print_clean(_tc_print_free)

extern char *_tc_stdio_buf;
extern size_t _tc_stdio_caps;
extern size_t _tc_stdio_len;
extern size_t _tc_stdio_thres;
extern void _tc_print_set_buf(size_t, _tc_malloc_ptr, _tc_free_ptr);
extern void _tc_print_clean(_tc_free_ptr);

#endif