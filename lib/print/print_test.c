#include <stdio.h>
#include <string.h>

#include "utest.h"
#define fprintf(stream, format, str) mock_fprintf(str)
#include "print.h"

char *print_str[2] = {NULL};
int i = 0;
void mock_fprintf(char *str) {
  if (print_str[i]) {
    free(print_str[i]);
  }
  int size = strlen(str) + 1;
  print_str[i] = malloc(size);
  strncpy(print_str[i], str, size);
  i++;
}

void print_test(TCUtState *ut_state) {
  tc_print_set_buf(10);

  tc_ut("initial stdio buffer", {
    tc_ut_assert(_tc_stdio_len == 0);
    tc_ut_assert(_tc_stdio_thres == 8);
    tc_ut_assert(_tc_stdio_caps == 10);
    tc_ut_assert(strlen(_tc_stdio_buf) == 0);
  });
  tc_ut("stdio buffer not reach threshold", {
    char *str = "test";
    tc_print(stdout, "%s", str);
    tc_ut_assert(_tc_stdio_len == 4);
    tc_ut_assert(_tc_stdio_thres == 8);
    tc_ut_assert(_tc_stdio_caps == 10);
    tc_ut_assert(!strcmp(_tc_stdio_buf, "test"));
  });
  tc_ut("stdio buffer reach threshold", {
    char *str = "test";
    tc_print(stdout, "%s", str);
    tc_ut_assert(_tc_stdio_len == 0);
    tc_ut_assert(_tc_stdio_thres == 8);
    tc_ut_assert(_tc_stdio_caps == 10);
    tc_ut_assert(!strcmp(_tc_stdio_buf, ""));
    tc_ut_assert(!strcmp(print_str[0], "test"));
    tc_ut_assert(!strcmp(print_str[1], "test"));
  });
  tc_ut("stdio buffer overflow", {
    i = 0;
    char *str = "tester";
    tc_print(stdout, "%s", str);
    tc_ut_assert(_tc_stdio_len == 6);
    tc_ut_assert(!strcmp(_tc_stdio_buf, "tester"));

    tc_print(stdout, "%s", str);
    tc_ut_assert(_tc_stdio_len == 0);
    tc_ut_assert(_tc_stdio_thres == 8);
    tc_ut_assert(_tc_stdio_caps == 10);
    tc_ut_assert(!strcmp(_tc_stdio_buf, ""));
    tc_ut_assert(!strcmp(print_str[0], "tester"));
    tc_ut_assert(!strcmp(print_str[1], "tester"));
  });

  tc_print_clean();
}

int main() {
  tc_ut_run(print_test);
  if (print_str[0]) free(print_str[0]);
  if (print_str[1]) free(print_str[1]);
  return 0;
}