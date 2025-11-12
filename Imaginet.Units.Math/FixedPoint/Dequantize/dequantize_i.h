#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#include <stdio.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_i32_to_f32"
static inline void dequantize_i32_to_f32(const int32_t* restrict src, float* restrict dst, int count)
{
	while (count > 0U)
	{
		*dst++ = *src++;
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_i16_to_f32"
static inline void dequantize_i16_to_f32(const int16_t* restrict src, float* restrict dst, int count)
{
	while (count > 0U)
	{
		*dst++ = *src++;
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_i8_to_f32"
static inline void dequantize_i8_to_f32(const int8_t* restrict src, float* restrict dst, int count)
{
	while (count > 0U)
	{
		*dst++ = *src++;
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_i32_to_i8"
static inline void dequantize_i32_to_i8(const int32_t* restrict src, int8_t* restrict dst, int count)
{
	int processed = 0;
	while (count > 0U)
	{
		// Clamp to int8 range
		if (val > 127) val = 127;
		else if (val < -128) val = -128;
		
		*dst++ = (int8_t)val;

		count--;
		processed++;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_i16_to_i8"
static inline void dequantize_i16_to_i8(const int16_t* restrict src, int8_t* restrict dst, int count)
{
	int processed = 0;
	while (count > 0U)
	{
		// Clamp to int8 range
		if (val > 127) val = 127;
		else if (val < -128) val = -128;
		
		*dst++ = (int8_t)val;
		
		count--;
		processed++;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_i8_to_i8"
static inline void dequantize_i8_to_i8(const int8_t* restrict src, int8_t* restrict dst, int count)
{
	int processed = 0;
	while (count > 0U)
	{
		int8_t val = *src++;
		
		*dst++ = val;
		
		count--;
		processed++;
	}
}
#pragma IMAGINET_FRAGMENT_END