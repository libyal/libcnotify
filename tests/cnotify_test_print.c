/*
 * Library notification print functions test program
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
#include <narrow_string.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
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

#include "../libcnotify/libcnotify_print.h"

#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )

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

		if( cnotify_test_real_vfprintf == NULL )
		{
			return( -1 );
		}
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

#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

/* Compares the contents of the stream with the expected data and rewinds the stream
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_compare_stream(
     FILE *stream,
     const char *expected_data,
     size_t expected_data_size )
{
	char buffer[ 512 ];

	ssize_t read_count = 0;
	int result         = 0;

	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "stream",
	 stream );

	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "expected_data",
	 expected_data );

	if( expected_data_size > 512 )
	{
		return( 0 );
	}
	result = fflush(
	          stream );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	result = fseek(
	          stream,
	          0,
	          SEEK_SET );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_set(
	 buffer,
	 0,
	 512 );

	read_count = fread(
	              buffer,
		      1,
		      expected_data_size,
	              stream );

	CNOTIFY_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) expected_data_size );

	result = fseek(
	          stream,
	          0,
	          SEEK_SET );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	clearerr(
	 stream );

	result = narrow_string_compare(
	          buffer,
	          expected_data,
	          expected_data_size );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libcnotify_printf function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_printf(
     void )
{
	FILE *stream             = NULL;
	libcerror_error_t *error = NULL;
	int print_count          = 0;
	int result               = 0;

	/* Initialize test
	 */
	stream = tmpfile();

	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "stream",
	 stream );

	/* Test print with stream
	 */
	result = libcnotify_stream_set(
	          stream,
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

	result = cnotify_test_compare_stream(
	          stream,
	          "Test",
	          4 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	/* Test error cases
	 */
#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )
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
#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

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

	/* Clean up
	 */
	result = fclose(
	          stream );

	stream = NULL;

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( stream != NULL )
	{
		fclose(
		 stream );
	}
	return( 0 );
}

#if defined( __GNUC__ ) && !defined( LIBCNOTIFY_DLL_IMPORT )

/* Tests the libcnotify_print_data_as_character function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_print_data_as_character(
     void )
{
	FILE *stream             = NULL;
	libcerror_error_t *error = NULL;
	int print_count          = 0;
	int result               = 0;

	/* Initialize test
	 */
	stream = tmpfile();

	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "stream",
	 stream );

	/* Test print with stream
	 */
	result = libcnotify_stream_set(
	          stream,
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

	result = cnotify_test_compare_stream(
	          stream,
	          "A",
	          1 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	print_count = libcnotify_print_data_as_character(
	               (uint8_t) 0x8c );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 1 );

	result = cnotify_test_compare_stream(
	          stream,
	          ".",
	          1 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	/* Test error cases
	 */
#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )
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
#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

	/* Clean up
	 */
	result = fclose(
	          stream );

	stream = NULL;

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( stream != NULL )
	{
		fclose(
		 stream );
	}
	return( 0 );
}

/* Tests the libcnotify_print_data_as_characters function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_print_data_as_characters(
     void )
{
	uint8_t data[ 60 ];

	FILE *stream             = NULL;
	libcerror_error_t *error = NULL;
	int index                = 0;
	int print_count          = 0;
	int result               = 0;

	/* Initialize test
	 */
	stream = tmpfile();

	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "stream",
	 stream );

	/* Test print with stream
	 */
	result = libcnotify_stream_set(
	          stream,
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

	result = cnotify_test_compare_stream(
	          stream,
	          "........ ........",
	          17 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	print_count = libcnotify_print_data_as_characters(
	               data,
	               60,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 13 );

	result = cnotify_test_compare_stream(
	          stream,
	          "`bdfhjln prtv",
	          13 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	print_count = libcnotify_print_data_as_characters(
	               data,
	               56,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 8 );

	result = cnotify_test_compare_stream(
	          stream,
	          "`bdfhjln",
	          8 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	print_count = libcnotify_print_data_as_characters(
	               data,
	               54,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 6 );

	result = cnotify_test_compare_stream(
	          stream,
	          "`bdfhjl",
	          6 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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

#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )
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
#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

	/* Clean up
	 */
	result = fclose(
	          stream );

	stream = NULL;

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( stream != NULL )
	{
		fclose(
		 stream );
	}
	return( 0 );
}

/* Tests the libcnotify_print_data_as_hexadecimal function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_print_data_as_hexadecimal(
     void )
{
	uint8_t data[ 60 ];

	FILE *stream             = NULL;
	libcerror_error_t *error = NULL;
	int index                = 0;
	int print_count          = 0;
	int result               = 0;

	/* Initialize test
	 */
	stream = tmpfile();

	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "stream",
	 stream );

	/* Test print with stream
	 */
	result = libcnotify_stream_set(
	          stream,
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

	result = cnotify_test_compare_stream(
	          stream,
	          "00 02 04 06 08 0a 0c 0e  10 12 14 16 18 1a 1c 1e ",
	          49 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               60,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 49 );

	result = cnotify_test_compare_stream(
	          stream,
	          "60 62 64 66 68 6a 6c 6e  70 72 74 76             ",
	          49 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               56,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 49 );

	result = cnotify_test_compare_stream(
	          stream,
	          "60 62 64 66 68 6a 6c 6e                          ",
	          49 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	print_count = libcnotify_print_data_as_hexadecimal(
	               data,
	               54,
	               48 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 49 );

	result = cnotify_test_compare_stream(
	          stream,
	          "60 62 64 66 68 6a                                ",
	          49 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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

#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )
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
#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

	/* Clean up
	 */
	result = fclose(
	          stream );

	stream = NULL;

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( stream != NULL )
	{
		fclose(
		 stream );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBCNOTIFY_DLL_IMPORT ) */

/* Tests the libcnotify_print_data function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_print_data(
     void )
{
	uint8_t data[ 128 ];

	FILE *stream                = NULL;
	libcerror_error_t *error    = NULL;
	const char *expected_buffer = NULL;
	int expected_buffer_length  = 0;
	int index                   = 0;
	int print_count             = 0;
	int result                  = 0;

	/* Initialize test
	 */
	stream = tmpfile();

	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "stream",
	 stream );

	/* Test print with stream
	 */
	result = libcnotify_stream_set(
	          stream,
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
	expected_buffer = \
		"00000000: 00 02 04 06 08 0a 0c 0e  10 12 14 16 18 1a 1c 1e   ........ ........\n" \
		"00000010: 20 22 24 26 28 2a 2c 2e  30 32 34 36 38 3a 3c 3e    \"$&(*,. 02468:<>\n" \
		"00000020: 40 42 44 46 48 4a 4c 4e  50 52 54 56 58 5a 5c 5e   @BDFHJLN PRTVXZ\\^\n" \
		"00000030: 60 62 64 66 68 6a 6c 6e  70 72 74 76               `bdfhjln prtv\n\n";

	expected_buffer_length = 313;

	print_count = libcnotify_print_data(
	               data,
	               60,
	               0 );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 expected_buffer_length );

	result = cnotify_test_compare_stream(
	          stream,
	          expected_buffer,
	          expected_buffer_length );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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

#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )
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
#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

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
#if defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK )
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
#endif /* defined( HAVE_CNOTIFY_TEST_FUNCTION_HOOK ) */

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

	/* Clean up
	 */
	result = fclose(
	          stream );

	stream = NULL;

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( stream != NULL )
	{
		fclose(
		 stream );
	}
	return( 0 );
}

/* Tests the libcnotify_print_error_backtrace function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_print_error_backtrace(
     void )
{
	FILE *stream                  = NULL;
	libcerror_error_t *error      = NULL;
	libcerror_error_t *test_error = NULL;
	const char *expected_buffer   = NULL;
	int expected_buffer_length    = 0;
	int print_count               = 0;
	int result                    = 0;

	/* Initialize test
	 */
	stream = tmpfile();

	CNOTIFY_TEST_ASSERT_IS_NOT_NULL(
	 "stream",
	 stream );

	libcerror_error_set(
	 &test_error,
	 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
	 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
	 "%s: invalid filename.",
	 "cnotify_test_print_error_backtrace" );

	/* Test print with stream
	 */
	result = libcnotify_stream_set(
	          stream,
	          &error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CNOTIFY_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if defined( WINAPI ) || defined( __MINGW32__ )
	expected_buffer = \
		"cnotify_test_print_error_backtrace: invalid filename.\r\n";

	expected_buffer_length = 55;
#else
	expected_buffer = \
		"cnotify_test_print_error_backtrace: invalid filename.\n";

	expected_buffer_length = 54;
#endif
	print_count = libcnotify_print_error_backtrace(
	               test_error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 expected_buffer_length );

	result = cnotify_test_compare_stream(
	          stream,
	          expected_buffer,
	          expected_buffer_length );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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
	               test_error );

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "print_count",
	 print_count,
	 0 );

	/* Clean up
	 */
	libcerror_error_free(
	 &test_error );

	result = fclose(
	          stream );

	stream = NULL;

	CNOTIFY_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( test_error != NULL )
	{
		libcerror_error_free(
		 &test_error );
	}
	if( stream != NULL )
	{
		fclose(
		 stream );
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
	 "libcnotify_printf",
	 cnotify_test_printf );

#if defined( __GNUC__ ) && !defined( LIBCNOTIFY_DLL_IMPORT )

	CNOTIFY_TEST_RUN(
	 "libcnotify_print_data_as_character",
	 cnotify_test_print_data_as_character );

	CNOTIFY_TEST_RUN(
	 "libcnotify_print_data_as_characters",
	 cnotify_test_print_data_as_characters );

	CNOTIFY_TEST_RUN(
	 "libcnotify_print_data_as_hexadecimal",
	 cnotify_test_print_data_as_hexadecimal );

#endif /* defined( __GNUC__ ) && !defined( LIBCNOTIFY_DLL_IMPORT ) */

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

