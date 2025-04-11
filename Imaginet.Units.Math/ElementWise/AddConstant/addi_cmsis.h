#pragma IMAGINET_INCLUDES_BEGIN
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "addi_cmsis_f32"
static inline void addi_cmsis_f32(
	const float* restrict x,
	int count,
	float immediate,
	float* restrict output)
{
	arm_offset_f32(x, immediate, output, count);
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "addi_cmsis_q31"
static inline void addi_cmsis_q31(
	const q31_t* restrict x,
	int count,
	q31_t immediate,
	q31_t* restrict output)
{
	arm_offset_q31(x, immediate, output, count);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "addi_cmsis_q15"
static inline void addi_cmsis_q15(
	const q15_t* restrict x,
	int count,
	q15_t immediate,
	q15_t* restrict output)
{
	arm_offset_q15(x, immediate, output, count);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "addi_cmsis_q7"
static inline void addi_cmsis_q7(
	const q7_t* restrict x,
	int count,
	q7_t immediate,
	q7_t* restrict output)
{
	arm_offset_q7(x, immediate, output, count);
}
#pragma IMAGINET_FRAGMENT_END

/**************** SHIFT VERSIONS *************************/

#pragma IMAGINET_FRAGMENT_BEGIN "addi_shift_cmsis_q31"
static inline void addi_shift_cmsis_q31(
	const q31_t* restrict x,
	int count,
	q31_t immediate,
	int shift,
	q31_t* restrict output)
{
	arm_shift_q31(x, -shift, output, count);
	arm_offset_q31(output, immediate, output, count);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "addi_shift_cmsis_q15"
static inline void addi_shift_cmsis_q15(
	const q15_t* restrict x,
	int count,
	q15_t immediate,
	int shift,
	q15_t* restrict output)
{
	arm_shift_q15(x, -shift, output, count);
	arm_offset_q15(output, immediate, output, count);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "addi_shift_cmsis_q7"
static inline void addi_shift_cmsis_q7(
	const q7_t* restrict x,
	int count,
	q7_t immediate,
	int shift,
	q7_t* restrict output)
{
	arm_shift_q7(x, -shift, output, count);
	arm_offset_q7(output, immediate, output, count);
}
#pragma IMAGINET_FRAGMENT_END