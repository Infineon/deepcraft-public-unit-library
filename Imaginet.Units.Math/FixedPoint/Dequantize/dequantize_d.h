#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#include <stdio.h>
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

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_d32_to_i8"
static inline void dequantize_d32_to_i8(const int32_t* restrict src, int8_t* restrict dst, int count, float scale, int offset)
{
	int processed = 0;
	while (count > 0U)
	{
		int32_t raw_src = *src++;
		float val = (raw_src - offset) * scale;
		
		// Scale [0,1] range to int8 range [-128,127]
		int32_t ival = (int32_t)(val * 255.0f - 128.0f);
		if (ival > 127) ival = 127;
		else if (ival < -128) ival = -128;
		
		*dst++ = (int8_t)ival;
		
		count--;
		processed++;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_d16_to_i8"
static inline void dequantize_d16_to_i8(const int16_t* restrict src, int8_t* restrict dst, int count, float scale, int offset)
{
	int processed = 0;
	while (count > 0U)
	{
		int16_t raw_src = *src++;
		float val = (raw_src - offset) * scale;
		
		// Scale [0,1] range to int8 range [-128,127]
		int32_t ival = (int32_t)(val * 255.0f - 128.0f);
		if (ival > 127) ival = 127;
		else if (ival < -128) ival = -128;
		
		*dst++ = (int8_t)ival;
		
		count--;
		processed++;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_d8_to_i8"
static inline void dequantize_d8_to_i8(const int8_t* restrict src, int8_t* restrict dst, int count, float scale, int offset)
{
	int processed = 0;
	while (count > 0U)
	{
		int8_t raw_src = *src++;
		float val = (raw_src - offset) * scale;
		
		// Scale [0,1] range to int8 range [-128,127]
		int32_t ival = (int32_t)(val * 255.0f - 128.0f);
		if (ival > 127) ival = 127;
		else if (ival < -128) ival = -128;
		
		*dst++ = (int8_t)ival;
		
		count--;
		processed++;
	}
}
#pragma IMAGINET_FRAGMENT_END