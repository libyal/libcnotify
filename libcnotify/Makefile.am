AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCNOTIFY_DLL_EXPORT@

lib_LTLIBRARIES = libcnotify.la

libcnotify_la_SOURCES = \
	libcnotify.c \
	libcnotify_definitions.h \
	libcnotify_extern.h \
	libcnotify_libcerror.h \
	libcnotify_print.c libcnotify_print.h \
	libcnotify_stream.c libcnotify_stream.h \
	libcnotify_support.c libcnotify_support.h \
	libcnotify_unused.h \
	libcnotify_verbose.c libcnotify_verbose.h

libcnotify_la_LIBADD = \
	@LIBCERROR_LIBADD@

libcnotify_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libcnotify_definitions.h.in \
	libcnotify.rc \
	libcnotify.rc.in

DISTCLEANFILES = \
	libcnotify_definitions.h \
	libcnotify.rc \
	Makefile \
	Makefile.in

sources-local: $(BUILT_SOURCES)

splint-local:
	@echo "Running splint on libcnotify ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libcnotify_la_SOURCES)

