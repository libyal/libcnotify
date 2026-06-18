/*
 * Library notification stream functions test program
 *
 * Copyright (C) 2008-2026, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_FCNTL_H ) || defined( WINAPI )
#include <fcntl.h>
#endif

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU
#endif

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
#define HAVE_CNOTIFY_TEST_FUNCTION_HOOK	1
#endif

#include "cnotify_test_libcerror.h"
#include "cnotify_test_libcnotify.h"
#include "cnotify_test_macros.h"
#include "cnotify_test_unused.h"

#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )

static FILE *(*cnotify_test_real_fopen)(const char *, const char *) = NULL;
int cnotify_test_fopen_attempts_before_fail                         = -1;

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

static int (*cnotify_test_real_fclose)(FILE *) = NULL;
int cnotify_test_fclose_attempts_before_fail   = -1;

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

#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

/* Tests the libcnotify_stream_set function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_stream_set(
     void )
{
	char filename[ 16 ]      = "cnotifyXXXXXX";

	libcerror_error_t *error = NULL;
	int result               = 0;

#if !defined( _MSC_VER ) && !defined( __MINGW32__ )
	int test_file_descriptor = 0;
#endif

	/* Initialize test
	 */
#if defined( _MSC_VER ) || defined( __MINGW32__ )
	result = _mktemp_s(
	          filename,
	          16 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );
#else
	test_file_descriptor = mkstemp(
	                        filename );

	CNOTIFY_TEST_ASSERT_NOT_EQUAL_INT(
	 "test_file_descriptor",
	 test_file_descriptor,
	 -1 );

	result = close(
	          test_file_descriptor );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );
#endif
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

#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )
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
#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

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
	return( 0 );
}

/* Tests the libcnotify_stream_open function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_stream_open(
     void )
{
	char filename[ 16 ]      = "cnotifyXXXXXX";

	libcerror_error_t *error = NULL;
	int result               = 0;

#if !defined( _MSC_VER ) && !defined( __MINGW32__ )
	int test_file_descriptor = 0;
#endif

	/* Initialize test
	 */
#if defined( _MSC_VER ) || defined( __MINGW32__ )
	result = _mktemp_s(
	          filename,
	          16 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );
#else
	test_file_descriptor = mkstemp(
	                        filename );

	CNOTIFY_TEST_ASSERT_NOT_EQUAL_INT(
	 "test_file_descriptor",
	 test_file_descriptor,
	 -1 );

	result = close(
	          test_file_descriptor );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );
#endif
	/* Test open
	 */
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

	/* Test open after open
	 */
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

#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )
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

#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )
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

#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

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
	return( 0 );
}

/* Tests the libcnotify_stream_close function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_stream_close(
     void )
{
	char filename[ 16 ]      = "cnotifyXXXXXX";

	libcerror_error_t *error = NULL;
	int result               = 0;

#if !defined( _MSC_VER ) && !defined( __MINGW32__ )
	int test_file_descriptor = 0;
#endif

	/* Initialize test
	 */
#if defined( _MSC_VER ) || defined( __MINGW32__ )
	result = _mktemp_s(
	          filename,
	          16 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );
#else
	test_file_descriptor = mkstemp(
	                        filename );

	CNOTIFY_TEST_ASSERT_NOT_EQUAL_INT(
	 "test_file_descriptor",
	 test_file_descriptor,
	 -1 );

	result = close(
	          test_file_descriptor );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );
#endif
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

#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )
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
#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
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

