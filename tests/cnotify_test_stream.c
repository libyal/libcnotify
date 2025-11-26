/*
 * Library notification stream functions test program
 *
 * Copyright (C) 2008-2025, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU
#endif

#include "cnotify_test_libcerror.h"
#include "cnotify_test_libcnotify.h"
#include "cnotify_test_macros.h"
#include "cnotify_test_unused.h"

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

static FILE *(*cnotify_test_real_fopen)(const char *, const char *) = NULL;
static int (*cnotify_test_real_fclose)(FILE *)                      = NULL;

int cnotify_test_fopen_attempts_before_fail                         = -1;
int cnotify_test_fclose_attempts_before_fail                        = -1;

/* Custom fopen for testing memory error cases
 * Returns a pointer to newly allocated data or NULL
 */
FILE *fopen(
       const char *path,
       const char *mode )
{
	FILE *stream = NULL;

	if( cnotify_test_real_fopen == NULL )
	{
		cnotify_test_real_fopen = dlsym(
		                           RTLD_NEXT,
		                           "fopen" );
	}
	if( cnotify_test_fopen_attempts_before_fail == 0 )
	{
		cnotify_test_fopen_attempts_before_fail = -1;

		return( NULL );
	}
	else if( cnotify_test_fopen_attempts_before_fail > 0 )
	{
		cnotify_test_fopen_attempts_before_fail--;
	}
	stream = cnotify_test_real_fopen(
	          path,
	          mode );

	return( stream );
}

/* Custom fclose for testing memory error cases
 * Returns a pointer to newly allocated data or NULL
 */
int fclose(
     FILE *stream )
{
	int print_count = 0;

	if( cnotify_test_real_fclose == NULL )
	{
		cnotify_test_real_fclose = dlsym(
		                            RTLD_NEXT,
		                            "fclose" );
	}
	if( cnotify_test_fclose_attempts_before_fail == 0 )
	{
		cnotify_test_fclose_attempts_before_fail = -1;

		return( -1 );
	}
	else if( cnotify_test_fclose_attempts_before_fail > 0 )
	{
		cnotify_test_fclose_attempts_before_fail--;
	}
	print_count = cnotify_test_real_fclose(
	               stream );

	return( print_count );
}

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

/* Tests the libcnotify_stream_set function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_stream_set(
     void )
{
	libcerror_error_t *error = NULL;
	char *filename           = NULL;
	int result               = 0;

	/* Test set of stderr
	 */
	result = libcnotify_stream_set(
	          stderr,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test set stderr after open
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	filename = _tempnam(
	            "C:\\Windows\\Temp",
	            "cnotify" );
#else
	filename = tempnam(
	            "/tmp",
	            "cnotify" );
#endif
	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "filename",
	 filename );

	result = libcnotify_stream_open(
	          filename,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if !defined( WINAPI ) || defined( __CYGWIN__ )
	/* tempnam() will return a newly allocated string
	 * _tempnam() will return a static string
	 */
	memory_free(
	 filename );
#endif

	filename = NULL;

	result = libcnotify_stream_set(
	          stderr,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	filename = _tempnam(
	            "C:\\Windows\\Temp",
	            "cnotify" );
#else
	filename = tempnam(
	            "/tmp",
	            "cnotify" );
#endif
	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "filename",
	 filename );

	result = libcnotify_stream_open(
	          filename,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if !defined( WINAPI ) || defined( __CYGWIN__ )
	/* tempnam() will return a newly allocated string
	 * _tempnam() will return a static string
	 */
	memory_free(
	 filename );
#endif

	filename = NULL;

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
	/* Test libcnotify_stream_close with fclose failing
	 */
	cnotify_test_fclose_attempts_before_fail = 0;

	result = libcnotify_stream_set(
	          stderr,
	          &error );

	if( cnotify_test_fclose_attempts_before_fail != -1 )
	{
		cnotify_test_fclose_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	/* Clean up
	 */
	result = libcnotify_stream_close(
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
#if !defined( WINAPI ) || defined( __CYGWIN__ )
	/* tempnam() will return a newly allocated string
	 * _tempnam() will return a static string
	 */
	if( filename != NULL )
	{
		memory_free(
		 filename );
	}
#endif
	return( 0 );
}

/* Tests the libcnotify_stream_open function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_stream_open(
     void )
{
	libcerror_error_t *error = NULL;
	char *filename           = NULL;
	int result               = 0;

	/* Test open
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	filename = _tempnam(
	            "C:\\Windows\\Temp",
	            "cnotify" );
#else
	filename = tempnam(
	            "/tmp",
	            "cnotify" );
#endif
	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "filename",
	 filename );

	result = libcnotify_stream_open(
	          filename,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if !defined( WINAPI ) || defined( __CYGWIN__ )
	/* tempnam() will return a newly allocated string
	 * _tempnam() will return a static string
	 */
	memory_free(
	 filename );
#endif

	filename = NULL;

	/* Test open after open
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	filename = _tempnam(
	            "C:\\Windows\\Temp",
	            "cnotify" );
#else
	filename = tempnam(
	            "/tmp",
	            "cnotify" );
#endif
	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "filename",
	 filename );

	result = libcnotify_stream_open(
	          filename,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if !defined( WINAPI ) || defined( __CYGWIN__ )
	/* tempnam() will return a newly allocated string
	 * _tempnam() will return a static string
	 */
	memory_free(
	 filename );
#endif

	filename = NULL;

	/* Test error cases
	 */
	result = libcnotify_stream_open(
	          NULL,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	filename = _tempnam(
	            "C:\\Windows\\Temp",
	            "cnotify" );
#else
	filename = tempnam(
	            "/tmp",
	            "cnotify" );
#endif
	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "filename",
	 filename );

	/* Test libcnotify_stream_open with fclose failing
	 */
	cnotify_test_fclose_attempts_before_fail = 0;

	result = libcnotify_stream_open(
	          filename,
	          &error );

	if( cnotify_test_fclose_attempts_before_fail != -1 )
	{
		cnotify_test_fclose_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
	/* Test libcnotify_stream_open with fopen failing
	 */
	cnotify_test_fopen_attempts_before_fail = 0;

	result = libcnotify_stream_open(
	          filename,
	          &error );

	if( cnotify_test_fopen_attempts_before_fail != -1 )
	{
		cnotify_test_fopen_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}

/* TODO test fail with different errno values */

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

#if !defined( WINAPI ) || defined( __CYGWIN__ )
	/* tempnam() will return a newly allocated string
	 * _tempnam() will return a static string
	 */
	memory_free(
	 filename );
#endif

	filename = NULL;

	/* Clean up
	 */
	result = libcnotify_stream_close(
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
#if !defined( WINAPI ) || defined( __CYGWIN__ )
	/* tempnam() will return a newly allocated string
	 * _tempnam() will return a static string
	 */
	if( filename != NULL )
	{
		memory_free(
		 filename );
	}
#endif
	return( 0 );
}

/* Tests the libcnotify_stream_close function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_stream_close(
     void )
{
	libcerror_error_t *error = NULL;
	char *filename           = NULL;
	int result               = 0;

	/* Test close after open
	 */
	result = libcnotify_stream_close(
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test close after open
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	filename = _tempnam(
	            "C:\\Windows\\Temp",
	            "cnotify" );
#else
	filename = tempnam(
	            "/tmp",
	            "cnotify" );
#endif
	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "filename",
	 filename );

	result = libcnotify_stream_open(
	          filename,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if !defined( WINAPI ) || defined( __CYGWIN__ )
	/* tempnam() will return a newly allocated string
	 * _tempnam() will return a static string
	 */
	memory_free(
	 filename );
#endif

	filename = NULL;

	result = libcnotify_stream_close(
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	filename = _tempnam(
	            "C:\\Windows\\Temp",
	            "cnotify" );
#else
	filename = tempnam(
	            "/tmp",
	            "cnotify" );
#endif
	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "filename",
	 filename );

	result = libcnotify_stream_open(
	          filename,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if !defined( WINAPI ) || defined( __CYGWIN__ )
	/* tempnam() will return a newly allocated string
	 * _tempnam() will return a static string
	 */
	memory_free(
	 filename );
#endif

	filename = NULL;

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
	/* Test libcnotify_stream_close with fclose failing
	 */
	cnotify_test_fclose_attempts_before_fail = 0;

	result = libcnotify_stream_close(
	          &error );

	if( cnotify_test_fclose_attempts_before_fail != -1 )
	{
		cnotify_test_fclose_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
#if !defined( WINAPI ) || defined( __CYGWIN__ )
	/* tempnam() will return a newly allocated string
	 * _tempnam() will return a static string
	 */
	if( filename != NULL )
	{
		memory_free(
		 filename );
	}
#endif
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc CNOTIFY_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] CNOTIFY_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc CNOTIFY_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] CNOTIFY_TEST_ATTRIBUTE_UNUSED )
#endif
{
	CNOTIFY_TEST_UNREFERENCED_PARAMETER( argc )
	CNOTIFY_TEST_UNREFERENCED_PARAMETER( argv )

	CNOTIFY_TEST_RUN(
	 "libcnotify_stream_set",
	 cnotify_test_stream_set );

	CNOTIFY_TEST_RUN(
	 "libcnotify_stream_open",
	 cnotify_test_stream_open );

	CNOTIFY_TEST_RUN(
	 "libcnotify_stream_close",
	 cnotify_test_stream_close );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

