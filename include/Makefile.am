include_HEADERS = \
	libcnotify.h

pkginclude_HEADERS = \
	libcnotify/definitions.h \
	libcnotify/error.h \
	libcnotify/extern.h \
	libcnotify/features.h \
	libcnotify/types.h

EXTRA_DIST = \
	libcnotify.h.in \
	libcnotify/definitions.h.in \
	libcnotify/features.h.in \
	libcnotify/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libcnotify.h
	-rm -f libcnotify/definitions.h
	-rm -f libcnotify/features.h
	-rm -f libcnotify/types.h
	-rm -f Makefile

