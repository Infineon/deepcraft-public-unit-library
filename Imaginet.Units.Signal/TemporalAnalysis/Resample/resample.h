#pragma IMAGINET_INCLUDES_BEGIN
#include <float.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "resample_t"
typedef struct {
	int sample_bytes;
	int ticks_per_input;	// time between two input samples
	int ticks_per_output;	// time between two output samples
	int phase;				// time of the next output relative to the newest input; due when <= 0
	int has_sample;
	float previous_time[2];	// min,max timestamp of the previous input sample
	float current_time[2];	// min,max timestamp of the newest input sample
} resample_t;

#ifdef _MSC_VER
static_assert(sizeof(resample_t) == 36, "Data structure 'resample_t' must match state_header_bytes in Resample.imunit");
#endif
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "resample_init"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample_t"

static inline void resample_init(void* restrict state_ptr, int sample_bytes, int ticks_per_input, int ticks_per_output)
{
	resample_t* state = (resample_t*)state_ptr;

	int a = ticks_per_output;
	int b = ticks_per_input;
	while (b != 0)
	{
		int remainder = a % b;
		a = b;
		b = remainder;
	}

	state->sample_bytes = sample_bytes;
	state->ticks_per_input = ticks_per_input / a;
	state->ticks_per_output = ticks_per_output / a;
	state->phase = 0;
	state->has_sample = 0;
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "resample_enqueue"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample_t"

static inline int resample_enqueue(void* restrict state_ptr, const void* restrict input, const float* restrict time, int time_count)
{
	resample_t* state = (resample_t*)state_ptr;
	char* previous = (char*)state_ptr + sizeof(resample_t);
	char* current = previous + state->sample_bytes;

	float min = FLT_MAX;
	float max = -FLT_MAX;
	for (int i = 0; i < time_count; ++i)
	{
		if (time[i] < min)
			min = time[i];
		if (time[i] > max)
			max = time[i];
	}

	if (state->has_sample)
	{
		memcpy(previous, current, state->sample_bytes);
		state->previous_time[0] = state->current_time[0];
		state->previous_time[1] = state->current_time[1];
		state->phase -= state->ticks_per_input;
	}
	else
	{
		memcpy(previous, input, state->sample_bytes);
		state->previous_time[0] = min;
		state->previous_time[1] = max;
		state->phase = 0;
		state->has_sample = 1;
	}

	memcpy(current, input, state->sample_bytes);
	state->current_time[0] = min;
	state->current_time[1] = max;
	return IPWIN_RET_SUCCESS;
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "resample_write_time"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample_t"

static inline void resample_write_time(const resample_t* state, float* restrict time_output, int uses_previous, int uses_current)
{
	if (time_output == NULL)
		return;

	float min = FLT_MAX;
	float max = -FLT_MAX;
	if (uses_previous)
	{
		min = state->previous_time[0];
		max = state->previous_time[1];
	}
	if (uses_current)
	{
		if (state->current_time[0] < min)
			min = state->current_time[0];
		if (state->current_time[1] > max)
			max = state->current_time[1];
	}

	time_output[0] = min;
	time_output[1] = max;
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "resample_can_dequeue"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample_t"

static inline int resample_can_dequeue(void* restrict state_ptr)
{
	const resample_t* state = (const resample_t*)state_ptr;
	return (state->has_sample && state->phase <= 0) ? IPWIN_RET_SUCCESS : IPWIN_RET_NODATA;
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "resample_can_enqueue"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample_t"

static inline int resample_can_enqueue(void* restrict state_ptr)
{
	const resample_t* state = (const resample_t*)state_ptr;
	return (state->has_sample && state->phase <= 0) ? IPWIN_RET_NODATA : IPWIN_RET_SUCCESS;
}

#pragma IMAGINET_FRAGMENT_END
