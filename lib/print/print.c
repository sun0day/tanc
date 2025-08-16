#include "print.h"

#include <stdio.h>
#include <string.h>

#include "common.h"

char *_tc_stdio_buf = NULL;
size_t _tc_stdio_caps = 0;
size_t _tc_stdio_len = 0;
size_t _tc_stdio_thres = 0;

void _tc_print_set_buf(size_t size, _tc_malloc_ptr malloc, _tc_free_ptr free) {
  _tc_stdio_caps = size;
  _tc_stdio_len = 0;
  _tc_stdio_thres = 7 * size / 8;

  if (_tc_stdio_buf) {
    free(_tc_stdio_buf);
  }

  _tc_stdio_buf = (char *)malloc(size);
}

inline void _tc_print_clean(_tc_free_ptr free) {
  if(_tc_stdio_buf)
    free(_tc_stdio_buf);
}