#!/usr/bin/env bash

alias tcconfig="echo '\n-------> Configure cmake <-------\n' && mkdir build 2>/dev/null || true && cd build && cmake .. && cd .."
alias tcbuild="tcconfig && echo '\n-------> Build libs <-------\n' && cmake --build build"
alias tctest="export TEST_RUN=1 && tcbuild && echo '\n-------> Run tests <-------\n' && ./build/test/test"
alias tcexpand="gcc -E ./test/test.c -I lib" 
alias tcclean="rm -rf build **/{CMakeFiles,cmake_install.cmake,CMakeCache.txt,Makefile} && echo '\nTmp files cleaned!\n'"
alias tclint="clang-format -i **/*.c **/*.h"
alias tccmd="TEST_RUN=1 cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1"

echo "\nTanc dev cmds preset!\n"
echo "tcconfig     \"configure cmake\""
echo "tcbuild      \"build libs\""
echo "tctest       \"run tests\""
echo "tcexpand     \"expand code macro\""
echo "tcclean      \"clean tmp files\""
echo "tclint       \"lint source code\""
echo "tccmd        \"generate compile_commands.json\""
