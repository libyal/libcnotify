dnl Checks for required headers and functions
dnl
dnl Version: 20190308

dnl Function to detect if libcnotify dependencies are available
AC_DEFUN([AX_LIBCNOTIFY_CHECK_LOCAL],
  [dnl Headers included in libcnotify/libcnotify_print.c
  AC_CHECK_HEADERS([stdarg.h varargs.h])

  AS_IF(
    [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
    [AC_MSG_FAILURE(
      [Missing headers: stdarg.h and varargs.h],
      [1])
    ])

  dnl Headers included in libcnotify/libcnotify_stream.c
  AC_CHECK_HEADERS([errno.h])
  ])

dnl Function to check if DLL support is needed
AC_DEFUN([AX_LIBCNOTIFY_CHECK_DLL_SUPPORT],
  [AS_IF(
    [test "x$enable_shared" = xyes],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw* | *msys*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])

      AC_SUBST(
        [LIBCNOTIFY_DLL_EXPORT],
        ["-DLIBCNOTIFY_DLL_EXPORT"])

      AC_SUBST(
        [LIBCNOTIFY_DLL_IMPORT],
        ["-DLIBCNOTIFY_DLL_IMPORT"])
      ])
    ])
  ])

