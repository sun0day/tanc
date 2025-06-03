#include "utest.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

static TCList *_mock_data;

inline void *tc_ut_malloc(size_t size) {
  _tc_ut_return((void *)&malloc, NULL, 0);
  _tc_ut_mock((void *)&malloc);

  return malloc(size);
}

inline void tc_ut_free(void *ptr) {
  _tc_ut_return((void *)&free, NULL, 0);
  _tc_ut_mock((void *)&free);

  free(ptr);
}

// run test handlers
void _tc_ut_run(tc_ut_handler *ut_handler, size_t len) {
  UTState *state = (UTState *)malloc(sizeof(UTState));
  if (_mock_data == NULL) _mock_data = tc_list_new();

  state->file = NULL;
  state->name = NULL;
  state->assert_rt = NULL;
  state->passed = 1;

  for (int i = 0; i < len; i++) {
    (**ut_handler)(state);
    ut_handler++;
  }

  // output last file test result
  _tc_ut_out(state);
}

// init file test result
void _tc_ut_fs(UTState *state, char *file) {
  if (state->file == NULL || strcmp(state->file, file) != 0) {
    if (state->file != NULL) {
      // output previous file test result
      _tc_ut_out(state);

    // clear mock data when changing file context
    tc_list_clear(_mock_data, _TCMockData);
    }

    state->file = file;
    state->passed = 1;

    if (state->assert_rt) {
      free(state->assert_rt);
    }
    state->assert_rt = tc_list_new();
  }
}

// run test case
inline void _tc_ut(UTState *state, char *name) { state->name = name; }

// safely abort with error code
void _tc_ut_abort(UTState *state, int err) {
  tc_list_free(state->assert_rt, TCAssertRt);
  tc_list_free(_mock_data, _TCMockData);

  free(state);

  exit(err);
}

// output test result
void _tc_ut_out(UTState *state) {
  if (state->file == NULL || state->assert_rt == NULL ||
      tc_list_empty(state->assert_rt) == 1) {
    fprintf(stderr, "No test cases or assertions found!\n");
    _tc_ut_abort(state, ENODATA);
  }

  fprintf(stdout, "%s %s\n\n", state->passed ? "[PASS]" : "[FAIL]",
          state->file);

  tc_list_each(tc_list_begin(state->assert_rt), tc_list_end(state->assert_rt),
               iter) {
    TCAssertRt *assert_rt = tc_list_at(iter, TCAssertRt);
    TCAssertRt *next_assert_rt = tc_list_at(tc_list_next(iter), TCAssertRt);

    if (!next_assert_rt || assert_rt->name != next_assert_rt->name) {
      fprintf(assert_rt->passed ? stdout : stderr, "       %s %s\n",
              assert_rt->passed ? "✔" : "✘", assert_rt->name);

      if (assert_rt->passed == 0) {
        fprintf(stderr, "\n         Error: line %d\n", assert_rt->lno);
        _tc_ut_abort(state, EIO);
      }
    }
  }
}

_TCMockData *_tc_ut_mock_fd(void *fn_ptr) {
  TCListIter begin = tc_list_begin(_mock_data);
  TCListIter end = tc_list_end(_mock_data);


  tc_list_each(begin, end, cur) {
    _TCMockData *_mp = tc_list_at(cur, _TCMockData);
    if (_mp->fn_ptr == fn_ptr) {
      return _mp;
    }
  }


  return NULL;
}

void *_tc_ut_mock(void *fn_ptr) {
  _TCMockData *_mp = _tc_ut_mock_fd(fn_ptr);
  if (_mp != NULL) {
    _mp->call_num++;

    return tc_list_front(_mp->data, _tc_void_ptr);
  }

  // abort if no mock data provided
  tc_list_free(_mock_data, _TCMockData);

  exit(ENODATA);
}

void _tc_ut_return(void *fn_ptr, void *value, unsigned char reset) {
  _TCMockData *_mp = _tc_ut_mock_fd(fn_ptr);
  if (_mp != NULL) {
    if(reset) 
      _mp->call_num = 0;
    tc_list_push(_mp->data, _tc_void_ptr, value);
    return;
  }

  TCList *data = tc_list_new();
  tc_list_push(data, _tc_void_ptr, value);
  tc_list_push(_mock_data, _TCMockData,
               ((_TCMockData){.fn_ptr = fn_ptr, .data = data, .call_num = 0}));
}

// store assert result
inline void _tc_ut_assert(UTState *state, unsigned int lno,
                          unsigned char passed) {
  state->passed &= passed;

  TCAssertRt rt = {
      .lno = lno,
      .name = state->name,
      .passed = passed,
  };
  tc_list_push(state->assert_rt, TCAssertRt, rt);

  if (state->passed == 0) {
    _tc_ut_out(state);
  }
}

inline unsigned char _tc_ut_assert_called(void *fn_ptr, unsigned int num) {
  _TCMockData *_mp = _tc_ut_mock_fd(fn_ptr);
  if (_mp != NULL) {
    return _mp->call_num == num;
  }

  return 0;
}
