/*
 * Library notification print functions test program
 *
 * Copyright (C) 2008-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "cnotify_test_libcerror.h"
#include "cnotify_test_libcnotify.h"
#include "cnotify_test_macros.h"
#include "cnotify_test_unused.h"

/* Tests the libcnotify_printf function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_printf(
     void )
{
	int print_count = 0;

	/* Test print without libcnotify_stream set
	 */
	print_count = libcnotify_printf(
	               "Test" );

	CNOTIFY_TEST_ASSERT_EQUAL(
	 "print_count",
	 print_count,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libcnotify_print_data function
 * Returns 1 if successful or 0 if not
 */
int cnotify_test_print_data(
     void )
{
	uint8_t data[ 8 ];

	int print_count = 0;

	/* Test print without libcnotify_stream set
	 */
	print_count = libcnotify_print_data(
	               data,
	               8,
	               0 );

	CNOTIFY_TEST_ASSERT_EQUAL(
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

	/* Test print without libcnotify_stream set
	 */
	print_count = libcnotify_print_error_backtrace(
	               error );

	CNOTIFY_TEST_ASSERT_EQUAL(
	 "print_count",
	 print_count,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	CNOTIFY_TEST_UNREFERENCED_PARAMETER( argc )
	CNOTIFY_TEST_UNREFERENCED_PARAMETER( argv )

	CNOTIFY_TEST_RUN(
	 "libcnotify_printf",
	 cnotify_test_printf() )

	CNOTIFY_TEST_RUN(
	 "libcnotify_print_data",
	 cnotify_test_print_data() )

	CNOTIFY_TEST_RUN(
	 "libcnotify_print_error_backtrace",
	 cnotify_test_print_error_backtrace() )

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

