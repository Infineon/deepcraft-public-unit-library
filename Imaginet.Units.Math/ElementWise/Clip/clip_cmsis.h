#pragma IMAGINET_INCLUDES_BEGIN
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "clip_cmsis_f32"

static inline void clip_cmsis_f32(const float* restrict input, int count, float min, float max, float* restrict output)
{
	arm_clip_f32(input, output, min, max, count);
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "clip_cmsis_q31"

static inline void clip_cmsis_q31(const q31_t* restrict input, int count, q31_t min, q31_t max, q31_t* restrict output)
{
	arm_clip_q31(input, output, min, max, count);
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "clip_cmsis_q15"

static inline void clip_cmsis_q15(const q15_t* restrict input, int count, q15_t min, q15_t max, q15_t* restrict output)
{
	arm_clip_q15(input, output, min, max, count);
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "clip_cmsis_q7"

static inline void clip_cmsis_q7(const q7_t* restrict input, int count, q7_t min, q7_t max, q7_t* restrict output)
{
	arm_clip_q7(input, output, min, max, count);
}

#pragma IMAGINET_FRAGMENT_END

