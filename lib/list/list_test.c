#ifdef TANC_UT_RUN

#include <list/list.h>
#include <utest/utest.h>

void list_test(UTState *ut_state) { tc_ut("ooooo", {}) }

int main() {
  tc_ut_run(list_test);
  return 0;
}
#endif
