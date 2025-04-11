/*
* Imagimob AB CONFIDENTIAL
* Unpublished Copyright (c) 2019- [Imagimob AB], All Rights Reserved.
* NOTICE: All information contained herein is, and remains the property of Imagimob AB.
*
* A circular buffer, circular queue, cyclic buffer or ring buffer is a data structure that 
* uses a single, fixed-size buffer as if it were connected end-to-end. This structure lends 
* itself easily to buffering data streams.
*
* File created: 2019-03-15 Albert Seward (albert.seward@imagimob.com)
*/

#ifndef _IMAI_CBUFFER_H_
#define _IMAI_CBUFFER_H_

#pragma IMAGINET_INCLUDES_BEGIN
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "cbuffer"
#pragma IMAGINET_FRAGMENT_TEST "cbuffer_tests.c:cbuffer_data_size_exceeds_buf_size_returns_CBUFFER_NOMEM_test"
#pragma IMAGINET_FRAGMENT_TEST "cbuffer_tests.c:cbuffer_enqueue_test"
#pragma IMAGINET_FRAGMENT_TEST "cbuffer_tests.c:cbuffer_enqueue_advance_reset_test"
#pragma IMAGINET_FRAGMENT_TEST "cbuffer_tests.c:cbuffer_enqueue_full_test"
#pragma IMAGINET_FRAGMENT_TEST "cbuffer_tests.c:cbuffer_enqueue_overflow_test"
#pragma IMAGINET_FRAGMENT_TEST "cbuffer_tests.c:cbuffer_enqueue_advance_enqueue_sequence_test"
#pragma IMAGINET_FRAGMENT_TEST "cbuffer_tests.c:cbuffer_enqueue_advance_edge_test"

// Represents a Circular Buffer
// https://en.wikipedia.org/wiki/Circular_buffer
typedef struct
{
	char *buf;
	int size;		// total bytes allocated in *buf
	int used;		// current bytes used in buffer.
	int read;
	int write;
} cbuffer_t;

#define CBUFFER_SUCCESS 0
#define CBUFFER_NOMEM -1

// Initializes a cbuffer handle with given memory and size.
static inline void cbuffer_init(cbuffer_t *dest, void *mem, int size) {
	dest->buf = mem;
	dest->size = size;
	dest->used = 0;
	dest->read = 0;
	dest->write = 0;
}

// Returns the number of free bytes in buffer.
static inline int cbuffer_get_free(cbuffer_t *buf) {
	return buf->size - buf->used;
}

// Returns the number of used bytes in buffer.
static inline int cbuffer_get_used(cbuffer_t *buf) {
	return buf->used;
}

// Writes given data to buffer.
// Returns CBUFFER_SUCCESS or CBUFFER_NOMEM if out of memory.
static inline int cbuffer_enqueue(cbuffer_t *buf, const void *data, int data_size) {
	int free = cbuffer_get_free(buf);

	// Out of memory?
	if (free < data_size)
		return CBUFFER_NOMEM;

	// Is the data split in the end?
	if (buf->write + data_size > buf->size) {
		int first_size = buf->size - buf->write;
		memcpy(buf->buf + buf->write, data, first_size);
		memcpy(buf->buf, ((char *)data) + first_size, data_size - first_size);
	}
	else {
		memcpy(buf->buf + buf->write, data, data_size);
	}
	buf->write += data_size;
	if (buf->write >= buf->size)
		buf->write -= buf->size;

	buf->used += data_size;
	return CBUFFER_SUCCESS;
}

// Advances the read pointer by given count.
// Returns CBUFFER_SUCCESS on success or CBUFFER_NOMEM if count is more than available data
static inline int cbuffer_advance(cbuffer_t *buf, int count) {
	int used = cbuffer_get_used(buf);

	if (count > used)
		return CBUFFER_NOMEM;

	buf->read += count;
	if (buf->read >= buf->size)
		buf->read -= buf->size;

	// Reset pointers to 0 if buffer is empty in order to avoid unwanted wrapps.
	if (buf->read == buf->write) {
		buf->read = 0;
		buf->write = 0;
	}

	buf->used -= count;
	return CBUFFER_SUCCESS;
}

// Reset instance (clear buffer)
static inline void cbuffer_reset(cbuffer_t *buf) {
	buf->read = 0;
	buf->write = 0;
	buf->used = 0;
}

// Copies given "count" bytes to the "dst" buffer without advancing the buffer read offset.
// Returns CBUFFER_SUCCESS on success or CBUFFER_NOMEM if count is more than available data.
static inline int cbuffer_copyto(cbuffer_t *buf, void *dst, int count, int offset) {
	
	if (count > cbuffer_get_used(buf))
		return CBUFFER_NOMEM;

	int a0 = buf->read + offset;
	if (a0 >= buf->size)
		a0 -= buf->size;

	int c0 = count;
	if (a0 + c0 > buf->size)
		c0 = buf->size - a0;
	
	memcpy(dst, buf->buf + a0, c0);
	
	int c1 = count - c0;

	if (c1 > 0)
		memcpy(((char *)dst) + c0, buf->buf, c1);

	return CBUFFER_SUCCESS;
}

// Returns a read pointer at given offset and  
// updates *can_read_bytes (if not NULL) with the number of bytes that can be read.
// 
// Note! Byte count written to can_read_bytes can be less than what cbuffer_get_used() returns.
// This happens when the read has to be split in two since it's a circular buffer.
static inline void *cbuffer_readptr(cbuffer_t* buf, int offset, int *can_read_bytes)
{
	int a0 = buf->read + offset;
	if (a0 >= buf->size)
		a0 -= buf->size;
	if (can_read_bytes != NULL)
	{
		int c0 = buf->used;
		if (a0 + c0 > buf->size)
			c0 = buf->size - a0;

		*can_read_bytes = c0;
	}
	return buf->buf + a0;
}

#pragma IMAGINET_FRAGMENT_END

#if DEBUG
#pragma IMAGINET_FRAGMENT_BEGIN "cbuffer_print_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "cbuffer"
void cbuffer_print_f32(cbuffer_t* buf)
{
	float* data = (float*)buf->buf;
	int bpi = sizeof(float);
	printf("Read: %i, Write: %i, Size %i, Content: ", buf->read / bpi, buf->write / bpi, buf->size / bpi);
	for (int i = 0; i < buf->size / bpi; i++) {
		printf("%f ", data[i]);
	}
	printf("\n");
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cbuffer_print_char"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "cbuffer"
void cbuffer_print_char(cbuffer_t* buf) 
{
	char* data = (char*)buf->buf;
	int bpi = sizeof(char);
	printf("Read: %i, Write: %i, Size %i, Content: ", buf->read / bpi, buf->write / bpi, buf->size / bpi);
	for (int i = 0; i < buf->size / bpi; i++) {
		printf("%c", data[i]);
	}
	printf("\n");
}
#pragma IMAGINET_FRAGMENT_END
#endif /* DEBUG */

#endif /* _IMAI_CBUFFER_H_ */