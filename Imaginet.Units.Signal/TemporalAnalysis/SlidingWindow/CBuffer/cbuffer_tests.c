/*
* Imagimob AB CONFIDENTIAL
* Unpublished Copyright (c) 2019- [Imagimob AB], All Rights Reserved.
* NOTICE: All information contained herein is, and remains the property of Imagimob AB.
*
* Unit tests for cbuffer.c
*
* File created: 2019-04-29 Albert Seward (albert.seward@imagimob.com)
*/

#pragma IMAGINET_INCLUDES_BEGIN
#include <stdlib.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#include "cbuffer.h"
#include "iassert.h"

#pragma IMAGINET_FRAGMENT_DEPENDENCY "iassert.h:iassert"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "cbuffer.h:cbuffer"

#pragma IMAGINET_FRAGMENT_BEGIN "cbuffer_data_size_exceeds_buf_size_returns_CBUFFER_NOMEM_test"
void cbuffer_data_size_exceeds_buf_size_returns_CBUFFER_NOMEM_test()
{
	// Arrange init
	cbuffer_t buffer;
	const int buffer_size = 4;
	cbuffer_init(&buffer, calloc(buffer_size, sizeof(float)), buffer_size * sizeof(float));
	
	// Act enqueue
	float data1[5] = { 1.0, 2.0, 3.0, 4.0, 5.0 };
	int data1_size = sizeof(data1);
	int ret = cbuffer_enqueue(&buffer, data1, data1_size);

	// Assert return
	asserti32("Assert return value", CBUFFER_NOMEM, ret);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cbuffer_enqueue_test"
void cbuffer_enqueue_test()
{
	// Arrange init
	cbuffer_t buffer;
	const int buffer_size = 8;
	cbuffer_init(&buffer, malloc(buffer_size), buffer_size);
	
	// Act enqueue
	char* data1 = "abcde";
	int data1_size = strlen(data1);
	int ret = cbuffer_enqueue(&buffer, data1, data1_size);

	// Assert state
	asserti32("Assert read pointer", 0, buffer.read);
	asserti32("Assert write pointer", data1_size, buffer.write);
	asserti32("Assert buffer size", buffer_size, buffer.size);
	asserti32("Assert return value", CBUFFER_SUCCESS, ret);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cbuffer_enqueue_full_test"
void cbuffer_enqueue_full_test()
{
	// Arrange init
	cbuffer_t buffer;
	const int buffer_size = 5;
	cbuffer_init(&buffer, malloc(buffer_size), buffer_size);

	// Act enqueue
	char* data1 = "abcde";
	int data1_size = strlen(data1);
	int ret = cbuffer_enqueue(&buffer, data1, data1_size);

	// Assert state
	asserti32("Assert read pointer", 0, buffer.read);
	asserti32("Assert write pointer", 0, buffer.write);
	asserti32("Assert buffer size", buffer_size, buffer.size);
	asserti32("Assert used buffer size", 5, buffer.used);
	asserti32("Assert return value", CBUFFER_SUCCESS, ret);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cbuffer_enqueue_overflow_test"
void cbuffer_enqueue_overflow_test()
{
	// Arrange init
	cbuffer_t buffer;
	const int buffer_size = 4;
	cbuffer_init(&buffer, malloc(buffer_size), buffer_size);

	// Act enqueue
	char* data1 = "abcde";
	int data1_size = strlen(data1);
	int ret = cbuffer_enqueue(&buffer, data1, data1_size);

	// Assert state
	asserti32("Assert read pointer", 0, buffer.read);
	asserti32("Assert write pointer", 0, buffer.write);
	asserti32("Assert buffer size", buffer_size, buffer.size);
	asserti32("Assert used buffer size", 0, buffer.used);
	asserti32("Assert return value", CBUFFER_NOMEM, ret);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cbuffer_enqueue_advance_reset_test"
void cbuffer_enqueue_advance_reset_test() {
	cbuffer_t buffer;
	const int buffer_size = 8;
	cbuffer_init(&buffer, malloc(buffer_size), buffer_size);

	char* data1 = "abcde";
	int data1_size = strlen(data1);
	int ret1 = cbuffer_enqueue(&buffer, data1, data1_size);
	asserti32("Assert cbuffer_enqueue return value", CBUFFER_SUCCESS, ret1);
	
	int used1 = cbuffer_get_used(&buffer);
	asserti32("Assert used", data1_size, used1);
	
	int free1 = cbuffer_get_free(&buffer);
	asserti32("Assert free", buffer_size - data1_size, free1);

	int ret2 = cbuffer_advance(&buffer, data1_size);
	asserti32("Assert cbuffer_advance return value", CBUFFER_SUCCESS, ret2);

	int used2 = cbuffer_get_used(&buffer);
	asserti32("Assert used", 0, used2);

	int free2 = cbuffer_get_free(&buffer);
	asserti32("Assert free", buffer_size, free2);

	int expected_rw_index = 0;		// With optimization this is 0, without it's 5.
	asserti32("Assert read pointer", expected_rw_index, buffer.read);
	asserti32("Assert write pointer", expected_rw_index, buffer.write);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cbuffer_enqueue_advance_edge_test"
void cbuffer_enqueue_advance_edge_test() {
	cbuffer_t buffer;
	const int buffer_size = 5;
	cbuffer_init(&buffer, malloc(buffer_size), buffer_size);

	char* data1 = "abcde";
	int data1_size = strlen(data1);
	int ret1 = cbuffer_enqueue(&buffer, data1, data1_size);
	asserti32("Assert cbuffer_enqueue return value", CBUFFER_SUCCESS, ret1);

	int ret2 = cbuffer_advance(&buffer, data1_size);
	asserti32("Assert cbuffer_advance return value", CBUFFER_SUCCESS, ret2);
	asserti32("Assert read pointer", 0, buffer.read);
	asserti32("Assert write pointer", 0, buffer.write);
	asserti32("Assert used", 0, buffer.used);
	asserti32("Assert free", buffer_size, buffer.size - buffer.used);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cbuffer_enqueue_advance_enqueue_sequence_test"
void cbuffer_enqueue_advance_enqueue_sequence_test()
{
	cbuffer_t buffer;
	float value;

	// Arrange init
	const int buffer_size = 8;
	int bpi = sizeof(float);
	cbuffer_init(&buffer, calloc(buffer_size, bpi), buffer_size * bpi);

	// Act 1) enqueue 5 items
	float data1[5] = { 1.0, 2.0, 3.0, 4.0, 5.0 };
	cbuffer_enqueue(&buffer, data1, sizeof(data1));
	asserti32("Assert write pointer", 5 * bpi, buffer.write);
	asserti32("Assert read pointer", 0, buffer.read);

	// Act 2) advance 4 items
	cbuffer_advance(&buffer, bpi * 4);
	asserti32("Assert read pointer", bpi * 4, buffer.read);

	// Act 3) enqueue 5 items. 
	// The last 2 items will be placed in the beginning.
	float data2[5] = { 11.0, 12.0, 13.0, 14.0, 15.0 };
	cbuffer_enqueue(&buffer, data2, sizeof(data2));
	// Index                  0     1     2    3    4    5     6     7
	// Pointers                           W>        R>
	float fptr_expected[] = { 14.0, 15.0, 3.0, 4.0, 5.0, 11.0, 12.0, 13.0 };
	float* fptr = (float*)buffer.buf;
	asserti32("Assert write pointer", bpi * 2, buffer.write);
	asserti32("Assert read pointer", bpi * 4, buffer.read);
	assertmem("Assert raw buffer", (char *)fptr_expected, (char*)fptr, sizeof(fptr_expected));
	
	cbuffer_copyto(&buffer, &value, bpi, 0 * bpi);
	assertf32("Assert read single float offset 0", 5.0, value);

	cbuffer_copyto(&buffer, &value, bpi, 1 * bpi);
	assertf32("Assert read single float offset 1", 11.0, value);

	cbuffer_copyto(&buffer, &value, bpi, 2 * bpi);
	assertf32("Assert read single float offset 2", 12.0, value);

	cbuffer_copyto(&buffer, &value, bpi, 3 * bpi);
	assertf32("Assert read single float offset 3", 13.0, value);

	cbuffer_copyto(&buffer, &value, bpi, 4 * bpi);
	assertf32("Assert read single float offset 4", 14.0, value);

	cbuffer_copyto(&buffer, &value, bpi, 5 * bpi);
	assertf32("Assert read single float offset 5", 15.0, value);

	float read_expected[] = { 5.0, 11.0, 12.0, 13.0, 14.0, 15.0 };
	float read_buffer[6];
	cbuffer_copyto(&buffer, read_buffer, sizeof(read_expected), 0);
	assertmem("Assert read buffer", (char*)read_expected, (char*)read_buffer, sizeof(read_expected));
}
#pragma IMAGINET_FRAGMENT_END

int main(void)
{
	cbuffer_data_size_exceeds_buf_size_returns_CBUFFER_NOMEM_test();
	cbuffer_enqueue_test();
	cbuffer_enqueue_advance_reset_test();
	cbuffer_enqueue_full_test();
	cbuffer_enqueue_overflow_test();
	cbuffer_enqueue_advance_enqueue_sequence_test();
	cbuffer_enqueue_advance_edge_test();
	return 0;
}