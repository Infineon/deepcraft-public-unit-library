#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "quantize_f32_to_q7"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void quantize_f32_to_q7(const float* restrict src, q7_t* restrict dst, int count, int shift)
{
	// arm_scale_f32(dst, 1d / (1 << shift), temp, count);
	// arm_float_to_q7(temp, dst, count);
	while (count > 0U)
	{
		*dst++ = (q7_t) __SSAT((q31_t) (*src++ * 128.0f / (1 << shift)), 8);
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "quantize_f32_to_q15"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void quantize_f32_to_q15(const float* restrict src, q15_t* restrict dst, int count, int shift)
{
	// arm_scale_f32(dst, 1d / (1 << shift), temp, count);
	// arm_float_to_q15(temp, dst, count);
	while (count > 0U)
	{
		*dst++ = (q15_t) __SSAT((q31_t) (*src++ * 32768.0f / (1 << shift)), 16);
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "quantize_f32_to_q31"
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
static inline void quantize_f32_to_q31(const float* restrict src, q31_t* restrict dst, int count, int shift)
{
	// arm_scale_f32(dst, 1d / (1 << shift), temp, count);
	// arm_float_to_q31(temp, dst, count);
	while (count > 0U)
	{
		*dst++ = clip_q63_to_q31((q63_t)(*src++ * 2147483648.0f / (1 << shift)));
		count--;
	}
}
#pragma IMAGINET_FRAGMENT_END