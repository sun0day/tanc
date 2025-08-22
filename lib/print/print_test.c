#include <stdio.h>
#include <string.h>

#include "utest.h"
#define fprintf(stream, format, str) mock_fprintf(str)
#include "print.h"

int i = 0;
void mock_fprintf(char *str) { tc_ut_arg_str(str); }

void print_test(TCUtState *ut_state) {
  tc_print_set_buf(10);

  tc_ut("initial stdio buffer", {
    tc_ut_assert(_tc_stdio_len == 0);
    tc_ut_assert(_tc_stdio_caps == 10);
    memset(_tc_stdio_buf, 0, _tc_stdio_caps);
    tc_ut_assert(strlen(_tc_stdio_buf) == 0);
  });
  tc_ut("stdio buffer not overflow", {
    char *str = "test";
    tc_print(stdout, "%s", str);
    tc_ut_assert(_tc_stdio_len == 4);
    tc_ut_assert(_tc_stdio_caps == 10);
    tc_ut_assert(!strcmp(_tc_stdio_buf, "test"));
  });
  tc_ut("stdio buffer overflow", {
    char *str = "hello tanc";

    tc_print(stdout, "%s", str);
    tc_ut_assert(_tc_stdio_len == 0);
    tc_ut_assert(_tc_stdio_caps == 10);
    tc_ut_assert(!strcmp(_tc_stdio_buf, ""));
    tc_ut_assert_arg_str(mock_fprintf, str, "test");
  });
}

int main() {
  tc_ut_run(print_test);
  return 0;
}