#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "quantize_f32_to_i8"
static inline void quantize_f32_to_i8(const float* restrict src, int8_t* restrict dst, int count)
{
	while (count > 0U)
	{
		*dst++ = (int8_t)(*src++);
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "quantize_f32_to_i16"
static inline void quantize_f32_to_i16(const float* restrict src, int16_t* restrict dst, int count)
{
	while (count > 0U)
	{
		*dst++ = (int16_t)(*src++);
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "quantize_f32_to_i32"
static inline void quantize_f32_to_i32(const float* restrict src, int32_t* restrict dst, int count)
{
	while (count > 0U)
	{
		*dst++ = (int32_t)(*src++);
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END
