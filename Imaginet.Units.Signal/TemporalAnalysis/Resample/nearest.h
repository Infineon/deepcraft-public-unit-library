#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "resample_dequeue_nearest_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample.h:resample_t"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample.h:resample_write_time"

static inline int resample_dequeue_nearest_f32(void* restrict state_ptr, float* restrict output, int count, float* restrict time_output)
{
	resample_t* state = (resample_t*)state_ptr;
	if (!state->has_sample || state->phase > 0)
		return IPWIN_RET_NODATA;

	const char* previous = (const char*)state_ptr + sizeof(resample_t);
	const char* current = previous + state->sample_bytes;

	int closer_to_current = 2 * (state->ticks_per_input + state->phase) > state->ticks_per_input;
	memcpy(output, closer_to_current ? current : previous, count * sizeof(float));

	resample_write_time(state, time_output, !closer_to_current, closer_to_current);
	state->phase += state->ticks_per_output;
	return IPWIN_RET_SUCCESS;
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "resample_dequeue_nearest_i8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample.h:resample_t"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample.h:resample_write_time"

static inline int resample_dequeue_nearest_i8(void* restrict state_ptr, int8_t* restrict output, int count, float* restrict time_output)
{
	resample_t* state = (resample_t*)state_ptr;
	if (!state->has_sample || state->phase > 0)
		return IPWIN_RET_NODATA;

	const char* previous = (const char*)state_ptr + sizeof(resample_t);
	const char* current = previous + state->sample_bytes;

	int closer_to_current = 2 * (state->ticks_per_input + state->phase) > state->ticks_per_input;
	memcpy(output, closer_to_current ? current : previous, count * sizeof(int8_t));

	resample_write_time(state, time_output, !closer_to_current, closer_to_current);
	state->phase += state->ticks_per_output;
	return IPWIN_RET_SUCCESS;
}

#pragma IMAGINET_FRAGMENT_END
