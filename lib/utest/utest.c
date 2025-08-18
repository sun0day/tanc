#include "utest.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "print.h"

static TCList *_mock_data_list;

/***********************************************************
 *                   Execution utils                       *
 ***********************************************************/

// safely abort with error code
void _tc_ut_abort(TCUtState *state, int err) {
  if (state != NULL) tc_list_free(state->assert_rt, _TCAssertRt);

  tc_list_each(tc_list_begin(_mock_data_list), tc_list_end(_mock_data_list),
               cur) {
    _TCMockData *mock_data = tc_list_at(cur, _TCMockData);
    tc_list_free(mock_data->data, _tc_void_ptr);
    _tc_ut_mock_arg_clear(mock_data);
    tc_list_free(mock_data->args, _TCMockArg);
  }

  tc_list_free(_mock_data_list, _TCMockData);

  free(state);

  tc_print_clean(stdout);
  tc_print_clean(stderr);

  exit(err);
}

// run test handlers
void _tc_ut_run(_tc_ut_handler *ut_handler, size_t len) {
  tc_print_set_buf(256);

  TCUtState *state = (TCUtState *)malloc(sizeof(TCUtState));
  if (_mock_data_list == NULL) _mock_data_list = tc_list_new();

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
  _tc_ut_abort(state, 0);
}

// init file test result
void _tc_ut_fs(TCUtState *state, char *file) {
  if (state->file == NULL || strcmp(state->file, file) != 0) {
    if (state->file != NULL) {
      // output previous file test result
      _tc_ut_out(state);

      // clear mock data when changing file context
      tc_list_clear(_mock_data_list, _TCMockData);
    }

    state->file = file;
    state->passed = 1;

    if (state->assert_rt) {
      tc_list_free(state->assert_rt, _TCAssertRt);
    }
    state->assert_rt = tc_list_new();
  }
}

// run test case
inline void _tc_ut(TCUtState *state, char *name) { state->name = name; }

// output test result
void _tc_ut_out(TCUtState *state) {
  if (state->file == NULL || state->assert_rt == NULL ||
      tc_list_empty(state->assert_rt) == 1) {
    tc_print(stderr, "No test cases or assertions found!\n");
    _tc_ut_abort(state, ENODATA);
  }

  tc_print(stdout, "\n%s %s\n\n", state->passed ? "[PASS]" : "[FAIL]",
           state->file);

  TCListIter end = tc_list_end(state->assert_rt);
  tc_list_each(tc_list_begin(state->assert_rt), end, iter) {
    TCListIter next_iter = tc_list_next(iter);

    _TCAssertRt *assert_rt = tc_list_at(iter, _TCAssertRt);
    _TCAssertRt *next_assert_rt =
        next_iter != end ? tc_list_at(next_iter, _TCAssertRt) : NULL;

    if (!next_assert_rt || assert_rt->name != next_assert_rt->name) {
      tc_print(assert_rt->passed ? stdout : stderr, "       %s %s\n",
               assert_rt->passed ? "✔" : "✘", assert_rt->name);

      if (assert_rt->passed == 0) {
        tc_print(stderr, "\n         Error: line %d\n", assert_rt->lno);
        _tc_ut_abort(state, EIO);
      }
    }
  }
}

/***********************************************************
 *                      Mock utils                         *
 ***********************************************************/

inline void *tc_ut_malloc(size_t size) {
  char *fn = "malloc";
  _tc_ut_return(fn, NULL);
  _tc_ut_call(fn);

  return malloc(size);
}

inline void tc_ut_free(void *ptr) {
  char *fn = "free";
  _tc_ut_return(fn, NULL);
  _tc_ut_call(fn);

  free(ptr);
}

void _tc_ut_mock_arg_clear(_TCMockData *mk) {
  if (mk == NULL) {
    return;
  }
  TCListIter begin = tc_list_begin(mk->args);
  TCListIter end = tc_list_end(mk->args);
  tc_list_each(begin, end, cur) {
    _TCMockArg *arg = tc_list_at(cur, _TCMockArg);
    if (arg && arg->value) {
      free(arg->value);
    }
  }
}

_TCMockData *_tc_ut_mock_fd(const char *fn) {
  TCListIter begin = tc_list_begin(_mock_data_list);
  TCListIter end = tc_list_end(_mock_data_list);

  tc_list_each(begin, end, cur) {
    _TCMockData *mk = tc_list_at(cur, _TCMockData);
    if (strcmp(mk->fn, fn) == 0) {
      return mk;
    }
  }

  return NULL;
}

_TCMockArg *_tc_ut_mock_arg_fd(const char *fn, char *name) {
  _TCMockData *mk = _tc_ut_mock_fd(fn);
  if (mk) {
    TCListIter begin = tc_list_begin(mk->args);
    TCListIter end = tc_list_end(mk->args);

    tc_list_each(begin, end, cur) {
      _TCMockArg *arg = tc_list_at(cur, _TCMockArg);

      if (strcmp(arg->name, name) == 0) {
        return arg;
      }
    }
  }

  return NULL;
}

void *_tc_ut_call(char *fn) {
  _TCMockData *mk = _tc_ut_mock_fd(fn);

  if (mk != NULL) {
    mk->call_num++;

    // TODO: replace with tc_list_shift
    return tc_list_front(mk->data, _tc_void_ptr);
  }

  // abort if no mock data provided
  _tc_ut_abort(NULL, ENODATA);

  return NULL;
}

void _tc_ut_mock_clear(char *fn) {
  _TCMockData *mk = _tc_ut_mock_fd(fn);

  if (mk != NULL) {
    mk->call_num = 0;
    tc_list_clear(mk->data, _tc_void_ptr);
    _tc_ut_mock_arg_clear(mk);
    tc_list_clear(mk->args, _TCMockArg);
  }
}

void _tc_ut_return(char *fn, void *value) {
  _TCMockData *mk = _tc_ut_mock_fd(fn);

  if (mk != NULL) {
    tc_list_append(mk->data, _tc_void_ptr, value);
    return;
  }

  TCList *data = tc_list_new();
  tc_list_append(data, _tc_void_ptr, value);
  tc_list_append(_mock_data_list, _TCMockData,
                 ((_TCMockData){.fn = fn, .data = data, .call_num = 0}));
}

void _tc_ut_arg(const char *fn, char *name, void *val_ptr, size_t size) {
  _TCMockData *mk = _tc_ut_mock_fd(fn);
  _TCMockArg arg =
      ((_TCMockArg){.name = name, .value = (char *)(malloc(size))});

  strncpy(arg.value, val_ptr, size);

  if (mk) {
    tc_list_append(mk->args, _TCMockArg, arg);
    return;
  }

  TCList *args = tc_list_new();
  tc_list_append(args, _TCMockArg, arg);
  tc_list_append(_mock_data_list, _TCMockData,
                 ((_TCMockData){.fn = fn, .args = args, .call_num = 0}));
}

/***********************************************************
 *                     Assert utils                        *
 ***********************************************************/

// store assert result
void _tc_ut_assert(TCUtState *state, unsigned int lno, unsigned char passed) {
  state->passed &= passed;

  tc_list_append(
      state->assert_rt, _TCAssertRt,
      ((_TCAssertRt){.lno = lno, .name = state->name, .passed = passed}));

  if (state->passed == 0) {
    _tc_ut_out(state);
  }
}

inline unsigned char _tc_ut_assert_called(char *fn, uint32_t num) {
  _TCMockData *mk = _tc_ut_mock_fd(fn);

  if (mk != NULL) {
    return mk->call_num == num;
  }

  return 0;
}

unsigned char _tc_ut_assert_arg_str(const char *fn, char *name,
                                    char *expected) {
  _TCMockArg *arg = _tc_ut_mock_arg_fd(fn, name);

  if (arg && arg->value) {
    return strcmp(expected, (char *)arg->value) == 0;
  }

  return 0;
}
