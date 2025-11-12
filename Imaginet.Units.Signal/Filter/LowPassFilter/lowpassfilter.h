#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h> // For expf() and M_PI
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "lowpass_filter_f32"

static inline void lowpass_filter_f32(const float* restrict input, int8_t* restrict state_bytes, float* restrict output, float cutoff_freq, int count, int freq)
{
	// Cast the byte buffer to float array for accessing filter state
	float* state = (float*)state_bytes;
	
	// Calculate alpha based on the cutoff frequency and the input data's frequency.
	// A common formula for a first-order low-pass filter:
	// alpha = 1 - exp(-2 * pi * cutoff_freq / sampling_freq)
	float alpha = 1.0f - expf(-2.0f * M_PI * cutoff_freq / (float)freq);

	for (int i = 0; i < count; ++i)
	{
		output[i] = alpha * input[i] + (1.0f - alpha) * state[i];
		state[i] = output[i];
	}
}

#pragma IMAGINET_FRAGMENT_END