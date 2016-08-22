MSVSCPP_FILES = \
	cnotify_test_get_version/cnotify_test_get_version.vcproj \
	cnotify_test_print/cnotify_test_print.vcproj \
	cnotify_test_stream/cnotify_test_stream.vcproj \
	libcerror/libcerror.vcproj \
	libcnotify/libcnotify.vcproj \
	libcstring/libcstring.vcproj \
	libcnotify.sln

SCRIPT_FILES = \
	scripts/vs2008_x64.sh \
	scripts/vs2008_x64_sln.sed \
	scripts/vs2008_x64_vcproj.sed \
	scripts/vs2010_x64.sh \
	scripts/vs2010_x64_sln.sed \
	scripts/vs2010_x64_vcxproj.sed

EXTRA_DIST = \
	$(MSVSCPP_FILES) \
	$(SCRIPT_FILES)

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

