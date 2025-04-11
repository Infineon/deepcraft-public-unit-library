#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
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