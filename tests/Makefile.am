AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCNOTIFY_DLL_IMPORT@

TESTS = \
	test_library.sh

check_SCRIPTS = \
	test_library.sh \
	test_manpage.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	cnotify_test_print \
	cnotify_test_stream \
	cnotify_test_support

cnotify_test_print_SOURCES = \
	cnotify_test_libcerror.h \
	cnotify_test_libcnotify.h \
	cnotify_test_macros.h \
	cnotify_test_print.c \
	cnotify_test_unused.h

cnotify_test_print_LDADD = \
	../libcnotify/libcnotify.la \
	@LIBCERROR_LIBADD@

cnotify_test_stream_SOURCES = \
	cnotify_test_libcerror.h \
	cnotify_test_libcnotify.h \
	cnotify_test_macros.h \
	cnotify_test_stream.c \
	cnotify_test_unused.h

cnotify_test_stream_LDADD = \
	../libcnotify/libcnotify.la \
	@LIBCERROR_LIBADD@

cnotify_test_support_SOURCES = \
	cnotify_test_libcerror.h \
	cnotify_test_libcnotify.h \
	cnotify_test_macros.h \
	cnotify_test_support.c \
	cnotify_test_unused.h

cnotify_test_support_LDADD = \
	../libcnotify/libcnotify.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

