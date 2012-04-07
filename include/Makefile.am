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
	/bin/rm -f libcnotify.h
	/bin/rm -f libcnotify/definitions.h
	/bin/rm -f libcnotify/features.h
	/bin/rm -f libcnotify/types.h
	/bin/rm -f Makefile

