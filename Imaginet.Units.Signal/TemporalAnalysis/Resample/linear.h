#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "resample_dequeue_linear_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample.h:resample_t"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample.h:resample_write_time"

static inline int resample_dequeue_linear_f32(void* restrict state_ptr, float* restrict output, int count, float* restrict time_output)
{
	resample_t* state = (resample_t*)state_ptr;
	if (!state->has_sample || state->phase > 0)
		return IPWIN_RET_NODATA;

	const float* previous = (const float*)((const char*)state_ptr + sizeof(resample_t));
	const float* current = (const float*)((const char*)previous + state->sample_bytes);

	if (state->phase == 0)
	{
		memcpy(output, current, count * sizeof(float));
	}
	else
	{
		float t = (float)(state->ticks_per_input + state->phase) / (float)state->ticks_per_input;
		for (int i = 0; i < count; ++i)
			output[i] = previous[i] + t * (current[i] - previous[i]);
	}

	resample_write_time(state, time_output, state->phase < 0, 1);
	state->phase += state->ticks_per_output;
	return IPWIN_RET_SUCCESS;
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "resample_dequeue_linear_i8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample.h:resample_t"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "resample.h:resample_write_time"

static inline int resample_dequeue_linear_i8(void* restrict state_ptr, int8_t* restrict output, int count, float* restrict time_output)
{
	resample_t* state = (resample_t*)state_ptr;
	if (!state->has_sample || state->phase > 0)
		return IPWIN_RET_NODATA;

	const int8_t* previous = (const int8_t*)((const char*)state_ptr + sizeof(resample_t));
	const int8_t* current = previous + state->sample_bytes;

	if (state->phase == 0)
	{
		memcpy(output, current, count * sizeof(int8_t));
	}
	else
	{
		float t = (float)(state->ticks_per_input + state->phase) / (float)state->ticks_per_input;
		for (int i = 0; i < count; ++i)
		{
			float value = previous[i] + t * (current[i] - previous[i]);
			output[i] = (int8_t)(value >= 0.0f ? value + 0.5f : value - 0.5f);
		}
	}

	resample_write_time(state, time_output, state->phase < 0, 1);
	state->phase += state->ticks_per_output;
	return IPWIN_RET_SUCCESS;
}

#pragma IMAGINET_FRAGMENT_END
