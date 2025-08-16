SET(LIB_HEADER common.h)
SET(LIB_NAME common)

ADD_LIBRARY(${LIB_NAME} INTERFACE)

INSTALL_HEADER("list;slist;print;utest")