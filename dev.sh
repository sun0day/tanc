#!/usr/bin/env bash

alias tclist="echo ''"
alias tcconfig="echo '\n-------> Configure cmake <-------\n' && mkdir build 2>/dev/null || true && cd build && TEST_RUN=1 cmake .. && cd .."
alias tcbuild="echo '\n-------> Build libs <-------\n' && cmake --build build"
alias tctest="tcconfig && tcbuild && echo '\n-------> Run tests <-------\n' && ./build/test/test"
alias tcclean="rm -rf build && echo '\nTmp files cleaned!\n'"
alias tclint="clang-format -i **/*.c **/*.h"

echo "\nTanc dev cmds preset!\n"
echo "tcconfig   \"configure cmake\""
echo "tcbuild    \"build libs\""
echo "tctest     \"run tests\""
echo "tcclean    \"clean tmp files\""
echo "tclint     \"lint source code\""