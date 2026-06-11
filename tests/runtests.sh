#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libcnotify/.libs/libcnotify.1.dylib && test -f ./pycnotify/.libs/pycnotify.so
then
	install_name_tool -change /usr/local/lib/libcnotify.1.dylib ${PWD}/libcnotify/.libs/libcnotify.1.dylib ./pycnotify/.libs/pycnotify.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

