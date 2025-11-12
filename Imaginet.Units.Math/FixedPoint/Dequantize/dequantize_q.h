#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#include <stdio.h>
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_q31_to_f32"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void dequantize_q31_to_f32(const q31_t* restrict src, float* restrict dst, int count, int shift)
{
	// arm_q15_to_float(src, dst, count);
	// arm_scale_f32(dst, (1 << shift), dst, count);

	while (count > 0U)
	{
		*dst++ = ((float32_t)*src++ / (float32_t)(2147483648 >> shift));
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_q15_to_f32"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void dequantize_q15_to_f32(const q15_t* restrict src, float* restrict dst, int count, int shift)
{
	// arm_q15_to_float(src, dst, count);
	// arm_scale_f32(dst, (1 << shift), dst, count);

	while (count > 0U)
	{
		*dst++ = ((float32_t)*src++ / (float32_t)(32768 >> shift));
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_q7_to_f32"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void dequantize_q7_to_f32(const q7_t* restrict src, float* restrict dst, int count, int shift)
{
	// arm_q7_to_float(src, dst, count);
	// arm_scale_f32(dst, (1 << shift), dst, count);

	while (count > 0U)
	{
		*dst++ = ((float32_t)*src++ / (float32_t)(128 >> shift));
		count--;
	} 
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_q31_to_i8"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void dequantize_q31_to_i8(const q31_t* restrict src, int8_t* restrict dst, int count, int shift)
{
	int processed = 0;
	while (count > 0U)
	{
		q31_t raw_src = *src++;
		float32_t val = ((float32_t)raw_src / (float32_t)(2147483648 >> shift));
		
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

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_q15_to_i8"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void dequantize_q15_to_i8(const q15_t* restrict src, int8_t* restrict dst, int count, int shift)
{
	
	int processed = 0;
	while (count > 0U)
	{
		q15_t raw_src = *src++;
		float32_t val = ((float32_t)raw_src / (float32_t)(32768 >> shift));
		
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

#pragma IMAGINET_FRAGMENT_BEGIN "dequantize_q7_to_i8"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void dequantize_q7_to_i8(const q7_t* restrict src, int8_t* restrict dst, int count, int shift)
{
	int processed = 0;
	while (count > 0U)
	{
		q7_t raw_src = *src++;
		float32_t val = ((float32_t)raw_src / (float32_t)(128 >> shift));
		
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