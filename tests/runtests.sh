#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libcnotify/.libs/libcnotify.1.dylib" ] && [ -f ./pycnotify/.libs/pycnotify.so ]
then
    install_name_tool -change /usr/local/lib/libcnotify.1.dylib "${PWD}/libcnotify/.libs/libcnotify.1.dylib" ./pycnotify/.libs/pycnotify.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

