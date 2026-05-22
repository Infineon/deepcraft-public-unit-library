#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "full_f32"

static inline void full_f32(float* restrict output, int count, const float* restrict fill_value)
{
	const float value = *fill_value;
	for (int i = 0; i < count; ++i)
		output[i] = value;
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "full_i8"

static inline void full_i8(int8_t* restrict output, int count, const int8_t* restrict fill_value)
{
	const int8_t value = *fill_value;
	for (int i = 0; i < count; ++i)
		output[i] = value;
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "full_i16"

static inline void full_i16(int16_t* restrict output, int count, const int16_t* restrict fill_value)
{
	const int16_t value = *fill_value;
	for (int i = 0; i < count; ++i)
		output[i] = value;
}

#pragma IMAGINET_FRAGMENT_END
