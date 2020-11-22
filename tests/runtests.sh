#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libcnotify/.libs/libcnotify.1.dylib && test -f ./pycnotify/.libs/pycnotify.so;
then
	install_name_tool -change /usr/local/lib/libcnotify.1.dylib ${PWD}/libcnotify/.libs/libcnotify.1.dylib ./pycnotify/.libs/pycnotify.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

