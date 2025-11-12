#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h> // For expf() and M_PI
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "highpass_filter_f32"

static inline void highpass_filter_f32(const float* restrict input, int8_t* restrict state_bytes, float* restrict output, float cutoff_freq, int count, int freq)
{
	// Cast the byte buffer to float array for accessing filter state
	float* state = (float*)state_bytes;
	
	// Calculate alpha based on the cutoff frequency and the input data's frequency.
	// The same formula as the low-pass filter can be used to derive alpha.
	float alpha = 1.0f - expf(-2.0f * M_PI * cutoff_freq / (float)freq);

	for (int i = 0; i < count; ++i)
	{
		float lowpass = alpha * input[i] + (1.0f - alpha) * state[i];
		output[i] = input[i] - lowpass;
		state[i] = lowpass;
	}
}

#pragma IMAGINET_FRAGMENT_END