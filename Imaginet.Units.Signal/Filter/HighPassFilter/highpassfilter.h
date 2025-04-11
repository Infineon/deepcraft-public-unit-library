#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "highpass_filter_f32"

static inline void highpass_filter_f32(const float* restrict input, float* restrict state, float* restrict output, float alpha, int count)
{
	for (int i = 0; i < count; ++i)
	{
		float lowpass = alpha * input[i] - (1.0f - alpha) * state[i];
		output[i] = input[i] - lowpass;
		state[i] = lowpass;
	}
}
#pragma IMAGINET_FRAGMENT_END