#!/usr/bin/env bash

export TEST_ROOT="./build/lib"

case "$(uname -s)" in
    CYGWIN*|MINGW*|MSYS*) export TEST_BIN="/Release" TEST_SUFFIX=".exe" ;;
    *)          export TEST_BIN="" TEST_SUFFIX="" ;;
esac

chmod +x $TEST_ROOT/list$TEST_BIN/list_test$TEST_SUFFIX

"$TEST_ROOT/list$TEST_BIN/list_test$TEST_SUFFIX"
