#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "np_full_f32"

static inline void np_full_f32(float* restrict output, int count, float fill_value)
{
	for (int i = 0; i < count; ++i)
		output[i] = fill_value;
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "np_full_i8"

static inline void np_full_i8(int8_t* restrict output, int count, int8_t fill_value)
{
	for (int i = 0; i < count; ++i)
		output[i] = fill_value;
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "np_full_i16"

static inline void np_full_i16(int16_t* restrict output, int count, int16_t fill_value)
{
	for (int i = 0; i < count; ++i)
		output[i] = fill_value;
}

#pragma IMAGINET_FRAGMENT_END
