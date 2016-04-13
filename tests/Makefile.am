AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCNOTIFY_DLL_IMPORT@

TESTS = \
	test_api_functions.sh

check_SCRIPTS = \
	test_api_functions.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	cnotify_test_get_version \
	cnotify_test_stream_set

cnotify_test_get_version_SOURCES = \
	cnotify_test_get_version.c
	cnotify_test_libcerror.h \
	cnotify_test_libcnotify.h \
	cnotify_test_libcstring.h \
	cnotify_test_unused.h

cnotify_test_get_version_LDADD = \
	../libcnotify/libcnotify.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

cnotify_test_stream_set_SOURCES = \
	cnotify_test_libcerror.h \
	cnotify_test_libcnotify.h \
	cnotify_test_libcstring.h \
	cnotify_test_stream_set.c

cnotify_test_stream_set_LDADD = \
	../libcnotify/libcnotify.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

