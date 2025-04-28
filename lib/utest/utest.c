#include <stdlib.h>
#include <list/list.h>
#include "utest.h"

typedef struct TCExpectedResult {
  int line_no;
  unsigned char passed; 
} EXRT;
TCLinkedList(EXRTList, EXRT);

typedef struct TCCaseResult {
  char* title;
  struct EXRTList *expected_rt;
} CART;
TCLinkedList(CARTList, CART);

typedef struct TCFileResult {
  char* file;
  struct CARTList *case_result;
} FIRT;
TCLinkedList(FIRTList, FIRT);

typedef struct TCTestSummary {
  struct FIRTList *file_result; 
} TSUM;

void _tc_utest_run(tc_utest_handler *utest_handler, size_t len) {
  for(int i = 0; i < len; i++) {
    (**utest_handler)();
    utest_handler++;
  }
}
