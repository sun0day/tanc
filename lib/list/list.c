#include "list.h"

#include <stddef.h>

// detect whether a iterator is still valid
inline unsigned char tc_list_iter_valid(TCListIter iter) {
  return (iter != NULL) & (iter->next != NULL) & (iter->prev != NULL);
}

inline TCListIter tc_list_iter_next(TCListIter iter) {
  return iter == NULL ? NULL : iter->next;
}

inline TCListIter tc_list_iter_prev(TCListIter iter) {
  return iter == NULL ? NULL : iter->prev;
}

#ifdef TANC_UT_RUN

#include <utest/utest.h>

void list_test(UTState *ut_state) {
  tc_ut("ooooo", { tc_ut_assert(1); })
}

int main() {
  tc_ut_run(list_test);
  return 0;
}
#endif
