/*
 * Library notification print functions test program
 *
 * Copyright (C) 2008-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU
#endif

#include "cnotify_test_libcerror.h"
#include "cnotify_test_libcnotify.h"
#include "cnotify_test_macros.h"
#include "cnotify_test_unused.h"

#include "../libcnotify/libcnotify_print.h"

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )

static int (*cnotify_test_real_vfprintf)(FILE *, const char *, va_list) = NULL;

int cnotify_test_vfprintf_attempts_before_fail                          = -1;

/* Custom vfprintf for testing memory error cases
 * Returns a pointer to newly allocated data or NULL
 */
int vfprintf(
     FILE *stream,
     const char *format,
     va_list ap )
{
	int print_count = 0;

	if( cnotify_test_real_vfprintf == NULL )
	{
		cnotify_test_real_vfprintf = dlsym(
		                              RTLD_NEXT,
		                              "vfprintf" );
	}
	if( cnotify_test_vfprintf_attempts_before_fail == 0 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;

		return( -1 );
	}
	else if( cnotify_test_vfprintf_attempts_before_fail > 0 )
	{
		cnotify_test_vfprintf_attempts_before_fail--;
	}
	print_count = cnotify_test_real_vfprintf(
	               stream,
	               format,
	               ap );

	return( print_count );
}

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) */

/* Tests the libcnotify_printf function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_printf(
     void )
{
	libcerror_error_t *error = NULL;
	int print_count          = 0;
	int result               = 0;

	/* Test print with stream
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

	print_count = libcnotify_printf(
	               "Test" );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 4 );

	/* Test error cases
	 */
#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )
	/* Test libcnotify_printf with vfprintf failing
	 */
	cnotify_test_vfprintf_attempts_before_fail = 1;

	print_count = libcnotify_printf(
	               "Test" );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) */

	/* Test print without stream
	 */
	result = libcnotify_stream_set(
	          NULL,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	print_count = libcnotify_printf(
	               "Test" );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

#if defined( __GNUC__ )

/* Tests the libcnotify_print_data_as_character function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_print_data_as_character(
     void )
{
	libcerror_error_t *error = NULL;
	int print_count          = 0;
	int result               = 0;

	/* Test print with stream
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

	/* Test regular cases
	 */
	print_count = libcnotify_print_data_as_character(
	               (uint8_t) 'A' );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 1 );

	print_count = libcnotify_print_data_as_character(
	               (uint8_t) 0x8c );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 1 );

	/* Test error cases
	 */
#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )
	/* Test libcnotify_printf with vfprintf failing
	 */
	cnotify_test_vfprintf_attempts_before_fail = 1;

	print_count = libcnotify_print_data_as_character(
	               (uint8_t) 'A' );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) */

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libcnotify_print_data_as_characters function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_print_data_as_characters(
     void )
{
	uint8_t data[ 60 ];

	libcerror_error_t *error = NULL;
	int index                = 0;
	int print_count          = 0;
	int result               = 0;

	/* Test print with stream
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

	/* Test regular cases
	 */
	for( index = 0;
	     index < 60;
	     index++ )
	{
		data[ index ] = (uint8_t) ( index + index );
	}
	print_count = libcnotify_print_data_as_characters(
	               data,
	               60,
	               0 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 17 );

	print_count = libcnotify_print_data_as_characters(
	               data,
	               60,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 13 );

	print_count = libcnotify_print_data_as_characters(
	               data,
	               56,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 8 );

	print_count = libcnotify_print_data_as_characters(
	               data,
	               54,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 6 );

	print_count = libcnotify_print_data_as_characters(
	               data,
	               60,
	               64 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 0 );

	/* Test error cases
	 */
	print_count = libcnotify_print_data_as_characters(
	               NULL,
	               60,
	               0 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 -1 );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )
	/* Test libcnotify_printf with vfprintf failing
	 */
	cnotify_test_vfprintf_attempts_before_fail = 0;

	print_count = libcnotify_print_data_as_characters(
	               data,
	               16,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
	cnotify_test_vfprintf_attempts_before_fail = 8;

	print_count = libcnotify_print_data_as_characters(
	               data,
	               16,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) */

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libcnotify_print_data_as_hexadecimal function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_print_data_as_hexadecimal(
     void )
{
	uint8_t data[ 60 ];

	libcerror_error_t *error = NULL;
	int index                = 0;
	int print_count          = 0;
	int result               = 0;

	/* Test print with stream
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

	/* Test regular cases
	 */
	for( index = 0;
	     index < 60;
	     index++ )
	{
		data[ index ] = (uint8_t) ( index + index );
	}
	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               60,
	               0 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 49 );

	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               60,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 49 );

	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               56,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 49 );

	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               54,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 49 );

	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               60,
	               64 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 0 );

	/* Test error cases
	 */
	print_count = libcnotify_print_data_as_hexadecimal(
	               NULL,
	               60,
	               0 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 -1 );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )
	/* Test libcnotify_printf with vfprintf failing
	 */
	cnotify_test_vfprintf_attempts_before_fail = 0;

	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               16,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
	cnotify_test_vfprintf_attempts_before_fail = 8;

	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               16,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
	cnotify_test_vfprintf_attempts_before_fail = 7;

	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               7,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
	cnotify_test_vfprintf_attempts_before_fail = 8;

	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               7,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) */

	return( 1 );

on_error:
	return( 0 );
}

#endif /* defined( __GNUC__ ) */

/* Tests the libcnotify_print_data function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_print_data(
     void )
{
	uint8_t data[ 128 ];

	libcerror_error_t *error = NULL;
	int index                = 0;
	int print_count          = 0;
	int result               = 0;

	/* Test print with stream
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

	/* Test without grouping
	 */
	for( index = 0;
	     index < 60;
	     index++ )
	{
		data[ index ] = (uint8_t) ( index + index );
	}
	print_count = libcnotify_print_data(
	               data,
	               60,
	               0 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 313 );

	/* Test error cases
	 */
	print_count = libcnotify_print_data(
	               NULL,
	               60,
	               0 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 -1 );

	print_count = libcnotify_print_data(
	               data,
	               (size_t) SSIZE_MAX + 1,
	               0 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 -1 );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )
	/* Test libcnotify_printf with vfprintf failing in libcnotify_printf
	 */
	cnotify_test_vfprintf_attempts_before_fail = 0;

	print_count = libcnotify_print_data(
	               data,
	               16,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
	/* Test libcnotify_printf with vfprintf failing in libcnotify_print_data_as_hexadecimal
	 */
	cnotify_test_vfprintf_attempts_before_fail = 1;

	print_count = libcnotify_print_data(
	               data,
	               16,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
	/* Test libcnotify_printf with vfprintf failing in libcnotify_printf
	 */
	cnotify_test_vfprintf_attempts_before_fail = 18;

	print_count = libcnotify_print_data(
	               data,
	               16,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
	/* Test libcnotify_printf with vfprintf failing in libcnotify_print_data_as_characters
	 */
	cnotify_test_vfprintf_attempts_before_fail = 19;

	print_count = libcnotify_print_data(
	               data,
	               16,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
	/* Test libcnotify_printf with vfprintf failing in libcnotify_printf
	 */
	cnotify_test_vfprintf_attempts_before_fail = 36;

	print_count = libcnotify_print_data(
	               data,
	               16,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
	/* Test libcnotify_printf with vfprintf failing in libcnotify_printf
	 */
	cnotify_test_vfprintf_attempts_before_fail = 37;

	print_count = libcnotify_print_data(
	               data,
	               16,
	               0 );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) */

	/* Test with grouping
	 */
	memory_set(
	 data,
	 'X',
	 108 );

	memory_set(
	 &( data[ 108 ] ),
	 'Y',
	 120 - 108 );

	print_count = libcnotify_print_data(
	               data,
	               120,
	               LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 312 );

	/* Test error cases
	 */
#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )
	/* Test libcnotify_printf with vfprintf failing
	 */
	cnotify_test_vfprintf_attempts_before_fail = 1;

	print_count = libcnotify_print_data(
	               data,
	               120,
	               LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

	if( cnotify_test_vfprintf_attempts_before_fail != -1 )
	{
		cnotify_test_vfprintf_attempts_before_fail = -1;
	}
	else
	{
		CNOTIFY_TEST_ASSERT_EQUAL_INT(
		 "print_count",
		 print_count,
		 -1 );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) */

	/* Test print without stream
	 */
	result = libcnotify_stream_set(
	          NULL,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	print_count = libcnotify_print_data(
	               data,
	               60,
	               0 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libcnotify_print_error_backtrace function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_print_error_backtrace(
     void )
{
	libcerror_error_t *error = NULL;
	int print_count          = 0;
	int result               = 0;

	/* Test print with stream
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

	print_count = libcnotify_print_error_backtrace(
	               error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 -1 );

	/* Test print without stream
	 */
	result = libcnotify_stream_set(
	          NULL,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	print_count = libcnotify_print_error_backtrace(
	               error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 0 );

	return( 1 );

on_error:
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
	 "libcnotify_printf",
	 cnotify_test_printf );

#if defined( __GNUC__ )

	CNOTIFY_TEST_RUN(
	 "libcnotify_print_data_as_character",
	 cnotify_test_print_data_as_character );

	CNOTIFY_TEST_RUN(
	 "libcnotify_print_data_as_characters",
	 cnotify_test_print_data_as_characters );

	CNOTIFY_TEST_RUN(
	 "libcnotify_print_data_as_hexadecimal",
	 cnotify_test_print_data_as_hexadecimal );

#endif /* defined( __GNUC__ ) */

	CNOTIFY_TEST_RUN(
	 "libcnotify_print_data",
	 cnotify_test_print_data );

	CNOTIFY_TEST_RUN(
	 "libcnotify_print_error_backtrace",
	 cnotify_test_print_error_backtrace );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

