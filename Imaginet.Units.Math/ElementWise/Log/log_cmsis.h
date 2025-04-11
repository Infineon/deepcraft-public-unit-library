#pragma IMAGINET_INCLUDES_BEGIN
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "log_cmsis_f32"
static inline void log_cmsis_f32(const float* restrict x, int count, float scale, float* restrict result)
{
	arm_vlog_f32(x, result, count);
	arm_scale_f32(result, scale, result, count);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "log_cmsis_q31"

static inline void log_cmsis_q31(
	const q31_t* restrict x,
	int count,
	q31_t offset,
	q31_t scale,
	q31_t* restrict result)
{
	arm_vlog_q31(x, result, count);
	arm_scale_q31(result, scale, 5, result, count);
	arm_offset_q31(result, offset, result, count);
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "log_cmsis_q15"

static inline void log_cmsis_q15(
	const q15_t* restrict x, 
	int count,
	q15_t offset,
	q15_t scale,
	q15_t* result)
{
	arm_vlog_q15(x, result, count);
	arm_scale_q15(result, scale, 5, result, count);
	arm_offset_q15(result, offset, result, count);
}
#pragma IMAGINET_FRAGMENT_END