#pragma IMAGINET_INCLUDES_BEGIN
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "abs_cmsis_f32"
static inline void abs_cmsis_f32(
	const float* restrict x,
	int count,
	float* restrict output)
{
	arm_abs_f32(x, output, count);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "abs_cmsis_q31"
static inline void abs_cmsis_q31(
	const q31_t* restrict x,
	int count,
	q31_t* restrict output)
{
	arm_abs_q31(x, output, count);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "abs_cmsis_q15"
static inline void abs_cmsis_q15(
	const q15_t* restrict x,
	int count,
	q15_t* restrict output)
{
	arm_abs_q15(x, output, count);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "abs_cmsis_q7"
static inline void abs_cmsis_q7(
	const q7_t* restrict x,
	int count,
	q7_t* restrict output)
{
	arm_abs_q7(x, output, count);
}
#pragma IMAGINET_FRAGMENT_END