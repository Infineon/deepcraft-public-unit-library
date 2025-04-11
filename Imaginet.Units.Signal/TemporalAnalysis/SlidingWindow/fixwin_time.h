#pragma IMAGINET_INCLUDES_BEGIN
#include <float.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#pragma IMAGINET_INCLUDES_END

#include "../../Common/CBuffer/cbuffer.h"

#pragma IMAGINET_FRAGMENT_BEGIN "fixwin_time_t"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "CBuffer/cbuffer.h:cbuffer"

typedef struct {
	cbuffer_t data_buffer;			// Circular Buffer for features
	cbuffer_t time_buffer;			// Circular Buffer for timestamps. Contains min,max for each input chunk
	int input_size;					// Number of bytes in each input chunk
} fixwin_time_t;

typedef float timestamp_t;
#define TIMESTAMP_MAX FLT_MAX
#define TIMESTAMP_MIN (-FLT_MAX)

#ifdef _MSC_VER
static_assert(sizeof(fixwin_time_t) <= 64, "Data structure 'fixwin_time_t' is too big");
#endif

#pragma IMAGINET_FRAGMENT_END

// All fragments depend on this
#pragma IMAGINET_FRAGMENT_DEPENDENCY "fixwin_time_t"

#pragma IMAGINET_FRAGMENT_BEGIN "fixwin_time_init"
/**
* Initializes a fixwin_time sampler handle.
*
* @param handle Pointer to a preallocated memory area of fixwin_time_handle_size() bytes to initialize.
*
* @param input_size Number of bytes to enqueue.
* @param count Number of items (of size input_size) in each window
*/
static inline void fixwin_time_init(void* restrict handle, int input_size, int count)
{
	fixwin_time_t* fep = (fixwin_time_t*)handle;
	fep->input_size = input_size;
	
	char* mem = ((char*)handle) + sizeof(fixwin_time_t);

	int data_buffer = input_size * count;
	int time_buffer = 2 * count * sizeof(timestamp_t);	// 2 = min,max
	
	cbuffer_init(&fep->data_buffer, mem, data_buffer);	
	cbuffer_init(&fep->time_buffer, mem + data_buffer, time_buffer);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "fixwin_time_enqueue"
/**
 * Enqueue handle->input_size values from given *data pointer to internal window buffer.
 *
 * @param handle Pointer to an initialized handle.
 * @param data Data to enqueue.
 * @param time Pointer to time_count timestamps.
 * @param time_count Number of float values in *time pointer
 * @return IPWIN_RET_SUCCESS (0) or IPWIN_RET_ERROR (-2) if internal buffer is out of memory.
 */
static inline int fixwin_time_enqueue(void* restrict handle, const void* restrict data, const void* restrict time, int time_count)
{
	fixwin_time_t* fep = (fixwin_time_t*)handle;

	if (cbuffer_enqueue(&fep->data_buffer, data, fep->input_size) != 0) 
		return IPWIN_RET_ERROR;
	
	timestamp_t min = TIMESTAMP_MAX;
	timestamp_t max = TIMESTAMP_MIN;
	for (int i = 0; i < time_count; i++)
	{
		const float value = ((timestamp_t *)time)[i];
		if (value > max)
			max = value;
		if (value < min)
			min = value;
	}

	if (cbuffer_enqueue(&fep->time_buffer, &min, sizeof(timestamp_t)) != 0)
		return IPWIN_RET_ERROR;
	
	if (cbuffer_enqueue(&fep->time_buffer, &max, sizeof(timestamp_t)) != 0)
		return IPWIN_RET_ERROR;

	return IPWIN_RET_SUCCESS;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "fixwin_time_dequeue"
/*
* Try to dequeue a window.
*
* @param handle Pointer to an initialized handle.
* @param dst Pointer where to write window.
* @param stride_count Number of items (of size handle->input_size) to stride window.
* @param time pointer to float[2] array where to write min and max timestamp.
* @return IPWIN_RET_SUCCESS (0) or IPWIN_RET_NODATA (-1) is no data is available.
*/
static inline int fixwin_time_dequeue(void* restrict handle, void* restrict dst, int count, int stride_count, void* restrict time, bool merge_time)
{
	fixwin_time_t* fep = (fixwin_time_t*)handle;

	const int size = count * fep->input_size;
	if (cbuffer_get_used(&fep->data_buffer) >= size) {
		if (cbuffer_copyto(&fep->data_buffer, dst, size, 0) != 0)
			return IPWIN_RET_ERROR;
	
		
		if (cbuffer_advance(&fep->data_buffer, stride_count * fep->input_size) != 0)
			return IPWIN_RET_ERROR;
		
		if (fep->time_buffer.used < count * 2 * sizeof(timestamp_t))
			return IPWIN_RET_ERROR;
		
		timestamp_t min;
		timestamp_t max;
		if (merge_time) {
			min = ((timestamp_t*)time)[0];
			max = ((timestamp_t*)time)[1];
		}
		else {
			min = TIMESTAMP_MAX;
			max = TIMESTAMP_MIN;
		}

		cbuffer_t* time_buf = &fep->time_buffer;
		for(int i = 0; i < count * 2; i++)
		{			
			const timestamp_t value = *(timestamp_t*)cbuffer_readptr(time_buf, i * sizeof(timestamp_t), NULL);
			if (value > max)
				max = value;
			if (value < min)
				min = value;
		}
		((timestamp_t *)time)[0] = min;
		((timestamp_t *)time)[1] = max;
		
		if (cbuffer_advance(time_buf, stride_count * 2 * sizeof(timestamp_t)) != 0)
			return IPWIN_RET_ERROR;
		
		return IPWIN_RET_SUCCESS;
	}

	return IPWIN_RET_NODATA;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "fixwin_time_can_dequeue"

static inline int fixwin_time_can_dequeue(void* restrict handle, int count)
{
	fixwin_time_t* fep = (fixwin_time_t*)handle;

	const int size = count * fep->input_size;
	int used = cbuffer_get_used(&fep->data_buffer);

	if (size > used) 
		return IPWIN_RET_NODATA;

	return IPWIN_RET_SUCCESS;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "fixwin_time_can_enqueue"

static inline int fixwin_time_can_enqueue(void* restrict handle, int count)
{
	fixwin_time_t* fep = (fixwin_time_t*)handle;

	const int size = count * fep->input_size;
	int free = cbuffer_get_free(&fep->data_buffer);

	if (size <= free) 
		return IPWIN_RET_SUCCESS;

	return IPWIN_RET_ERROR;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "fixwin_time_reset"
/*
* Reset sampler to its initial state
*
* @param handle Pointer to an _initialized_ handle to reset.
*/
static inline void fixwin_time_reset(void* restrict handle)
{
	fixwin_time_t* fep = (fixwin_time_t*)handle;
	cbuffer_reset(&(fep->data_buffer));
	cbuffer_reset(&(fep->time_buffer));
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "fixwin_time_clear"

static inline void fixwin_time_clear(void* restrict buffer)
{
	timestamp_t* time = (timestamp_t*)buffer;

	*time++ = TIMESTAMP_MAX;
	*time = TIMESTAMP_MIN;
}
#pragma IMAGINET_FRAGMENT_END