#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_d32_to_f32"
static inline void dequantize_d32_to_f32(const int32_t* restrict src, float* restrict dst, int count, float scale, int offset)
{
	while (count > 0U)
	{
		*dst++ = (*src++ - offset) * scale;
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_d16_to_f32"
static inline void dequantize_d16_to_f32(const int16_t* restrict src, float* restrict dst, int count, float scale, int offset)
{
	while (count > 0U)
	{
		*dst++ = (*src++ - offset) * scale;
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_d8_to_f32"
static inline void dequantize_d8_to_f32(const int8_t* restrict src, float* restrict dst, int count, float scale, int offset)
{
	while (count > 0U)
	{
		*dst++ = (*src++ - offset) * scale;
		count--;
	} 
}
#pragma IMAGINET_FRAGMENT_END