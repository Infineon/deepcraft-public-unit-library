#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "quantize_f32_to_d8"
static inline void quantize_f32_to_d8(const float* restrict src, int8_t* restrict dst, int count, float scale, int offset)
{
	while (count > 0U)
	{
		float value = (*src++ / scale) + offset;
		if (value > 127)
			*dst++ = 127;
		else if (value < -128)
			*dst++ = -128;
		else
			*dst++ = (int8_t)value;
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "quantize_f32_to_d16"
static inline void quantize_f32_to_d16(const float* restrict src, int16_t* restrict dst, int count, float scale, int offset)
{
	while (count > 0U)
	{
		float value = (*src++ / scale) + offset;
		if (value > 32767)
			*dst++ = 32767;
		else if (value < -32768)
			*dst++ = -32768;
		else
			*dst++ = (int16_t)value;
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "quantize_f32_to_d32"
static inline void quantize_f32_to_d32(const float* restrict src, int32_t* restrict dst, int count, float scale, int offset)
{
	while (count > 0U)
	{
		float value = (*src++ / scale) + offset;
		if (value > 2147483647)
			*dst++ = 2147483647;
		else if (value < -2147483648)
			*dst++ = -2147483648;
		else
			*dst++ = (int32_t)value;
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END
