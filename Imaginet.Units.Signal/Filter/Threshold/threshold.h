#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "threshold_f32"

static inline void threshold_f32(const float* restrict input, float* restrict output, int count, float threshold)
{
	for (int i = 0; i < count; ++i)
	{
		const float v = input[i];
		output[i] = (v >= threshold) ? v : 0.0f;
	}
}

#pragma IMAGINET_FRAGMENT_END

