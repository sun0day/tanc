#include "print.h"

#include <stdio.h>
#include <string.h>

#include "common.h"

char *_tc_stdio_buf = NULL;
size_t _tc_stdio_caps = 0;
size_t _tc_stdio_len = 0;
size_t _tc_stdio_thres = 0;

void _tc_print_set_buf(size_t size, _tc_malloc_ptr _malloc,
                       _tc_free_ptr _free) {
  _tc_stdio_caps = size;
  _tc_stdio_len = 0;

  if (_tc_stdio_buf) {
    _free(_tc_stdio_buf);
  }

  _tc_stdio_buf = (char *)_malloc(size);
}

inline void _tc_print_clean(FILE *stream, _tc_free_ptr _free) {
  tc_print_flush(stream);
  if (_tc_stdio_buf) {
    _free(_tc_stdio_buf);
    _tc_stdio_buf = NULL;
  }
}