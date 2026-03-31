#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "linear_f32"

static inline void linear_f32(const float* restrict input, float* restrict output, int count, float* previous_input_pointer)
{
	for (int i = 0; i < count; ++i)
		*(output++) = *(input++);
}

#pragma IMAGINET_FRAGMENT_END