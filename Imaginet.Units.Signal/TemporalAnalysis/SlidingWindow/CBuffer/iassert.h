/*
* Imagimob AB CONFIDENTIAL
* Unpublished Copyright (c) 2019- [Imagimob AB], All Rights Reserved.
* NOTICE: All information contained herein is, and remains the property of Imagimob AB.
*
* File created: 2019-04-29 Albert Seward (albert.seward@imagimob.com)
*/

#ifndef _IMAI_IASSERT_H_
#define _IMAI_IASSERT_H_

#pragma IMAGINET_INCLUDES_BEGIN
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "iassert"

#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif

static void _print_memory(const char* ptr, size_t size) {
	const int columns = 8;
	const char* indent = "    ";

	printf(indent);
	for (int i = 0; i < size; i++) {
		unsigned char value = (unsigned char)*ptr++;
		printf("%02x ", value);
		if (i != size - 1 && i != 0 && (i + 1) % columns == 0) {
			printf("\n%s", indent);
		}
	}
	printf("\n");
}

// assert memory chunk
#define assertmem(description, expected, actual, size) _assertmem(__func__, description, expected, actual, size)
static void _assertmem(const char* testname, const char* description, char *expected, char *actual, size_t size) {

	if (expected == NULL || actual == NULL || memcmp(expected, actual, size) != 0) {
		printf("[FAILED] %s: (C) %s. Memory segment of size %li bytes differs.\n", testname, description, size);
		printf("    Expected:\n");
		_print_memory(expected, size);
		printf("    Actual:\n");
		_print_memory(actual, size);
	}
	else {
		printf("[PASSED] %s: (C) %s. Memory segment of size %li bytes matches which is expected.\n", testname, description, size);
	}
}

// assert float32 absolute error with threshold
#define assertarrayf32m(description, expected, actual, count, max_error) _assertarrayf32m(__func__, description, expected, actual, count, max_error)
static void _assertarrayf32m(const char* testname, const char* description, float* expected, float* actual, int count, float max_error) {
	int error_count = 0;
	for (int i = 0; i < count; i++)
	{
		float diff = fabsf(expected[i] - actual[i]);
		if (diff > 0) {
			printf("[FAILED] %s: (C) %s. Expected %f but got %f for element %i. (Error of %f)\n", testname, description, expected[i], actual[i], i, diff);
			error_count++;
		}		
	}

	if (error_count == 0) {
		printf("[PASSED] %s: (C) %s. \n", testname, description);
	}
}

// assert float32 relative error with threshold
#define assertarrayf32mr(description, expected, actual, count, max_relative_error) _assertarrayf32mr(__func__, description, expected, actual, count, max_relative_error)
static void _assertarrayf32mr(const char* testname, const char* description, float* expected, float* actual, int count, float max_relative_error) {
	int error_count = 0;
	for (int i = 0; i < count; i++)
	{
		float diff = fabsf(expected[i] - actual[i]);
		float max = fmaxf(fmaxf(fabsf(expected[i]), fabsf(actual[i])), 1e-10);
		if (max == 0)
			max = 1;
		double relative = diff / (double)max;
		if (relative > max_relative_error) {
			printf("[FAILED] %s: (C) %s. Expected %f but got %f for element %i. Absolute error of %f. Relative error of %f%%\n", testname, description, expected[i], actual[i], i, diff, relative);
			error_count++;
		}
	}

	if (error_count == 0) {
		printf("[PASSED] %s: (C) %s. \n", testname, description);
	}
}

// assert float32 absolute error
#define assertarrayf32(description, expected, actual, count) _assertarrayf32(__func__, description, expected, actual, count)
static void _assertarrayf32(const char* testname, const char* description, float* expected, float* actual, int count) {

	_assertmem(testname, description, (char*)expected, (char*)actual, count * sizeof(float));
}

#define asserti32(description, expected, actual) _asserti32(__func__, description, expected, actual)
static void _asserti32(const char* testname, const char* description, int expected, int actual)  {

	if (actual != expected)
		printf("[FAILED] %s: (C) %s. Expected %i but got %i. (Error of %i)\n", testname, description, expected, actual, expected - actual);
	else
		printf("[PASSED] %s: (C) %s. Got %i which is expected.\n", testname, description, actual);
}

#define assertf32(description, expected, actual) _assertf32(__func__, description, expected, actual)
static void _assertf32(const char* testname, const char* description, float expected, float actual) {

	float diff = fabsf(actual - expected);
	float maxError = 0;

	if (diff > maxError)
		printf("[FAILED] %s: (C) %s. Expected %f but got %f. (Error of %f)\n", testname, description, expected, actual, diff);
	else
		printf("[PASSED] %s: (C) %s. Got %f which is expected.\n", testname, description, actual);
}

#pragma IMAGINET_FRAGMENT_END

#endif /* _IMAI_IASSERT_H_ */