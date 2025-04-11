#pragma IMAGINET_INCLUDES_BEGIN
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "norm_sparse_cmsis_q31"
static inline q31_t __norm_sparse_cmsis_q31(const q31_t* restrict input, int step, int count, int shift)
{
	q63_t sum = 0;
	while (count > 0U)
	{
		sum += ARM_SQ((q63_t)*input) >> 31;	// (1.31 x 1.31 = 2.62) >> 31 = 33.31 
		input += step;
		count--;
	}
	q31_t result;
	arm_sqrt_q31(sum >> (shift * 2), &result); // result is (1+shift).(31-shift)
	return result;
}

static inline void norm_sparse_cmsis_q31(const q31_t* restrict input, int d0, int d1, int d2, q31_t* restrict output, int shift)
{ 
	const int d3 = d0 * d1;
	for (int k = 0; k < d2; k++) {
		const q31_t* ip = input + k * d3;
		for (int i = 0; i < d0; i++) {
			*output++ = __norm_sparse_cmsis_q31(ip++, d0, d1, shift);	
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

// ===== basic =====

#pragma IMAGINET_FRAGMENT_BEGIN "norm_cmsis_q31"
static inline void norm_cmsis_q31(const q31_t* restrict input, int d1, int d2, q31_t* restrict output, int shift)
{
	for (int k = 0; k < d2; k++) {
		q63_t sum;
		arm_dot_prod_q31(input, input, d1, &sum);
		arm_sqrt_q31(sum >> (17 + shift * 2), output++); // 16.48 - 3.29
		input += d1;
	}
}
#pragma IMAGINET_FRAGMENT_END


// ===== complex (d1=2) =====
#pragma IMAGINET_FRAGMENT_BEGIN "norm_cmsis_cmplx_q31"
static inline void norm_cmsis_cmplx_q31(const q31_t* restrict input, int d2, q31_t* restrict output, int shift)
{
	arm_cmplx_mag_q31(input, output, d2);
	arm_shift_q31(output, 1-shift, output, d2);
}
#pragma IMAGINET_FRAGMENT_END
