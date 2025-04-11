#pragma IMAGINET_INCLUDES_BEGIN
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "norm_sparse_cmsis_q15"
static inline q15_t __norm_sparse_cmsis_q15(const q15_t* restrict input, int step, int count, int shift)
{
	q31_t sum = 0;
	while (count > 0U)
	{
		sum += ARM_SQ((q31_t)*input) >> (1 + shift * 2);
		input += step;
		count--;
	}
	q31_t result;
	arm_sqrt_q31(sum, &result); 
	return result >> 15; // result is (1+shift).(15-shift)
}

static inline void norm_sparse_cmsis_q15(const q15_t* restrict input, int d0, int d1, int d2, q15_t* restrict output, int shift)
{
	const int d3 = d0 * d1;
	for (int k = 0; k < d2; k++) {
		const q15_t* ip = input + k * d3;
		for (int i = 0; i < d0; i++) {
			*output++ = __norm_sparse_cmsis_q15(ip++, d0, d1, shift);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

// ===== basic =====

#pragma IMAGINET_FRAGMENT_BEGIN "norm_cmsis_q15"
static inline void norm_cmsis_q15(const q15_t* restrict input, int d1, int d2, q15_t* restrict output, int shift)
{
	for (int k = 0; k < d2; k++) {
		q63_t sum;
		arm_dot_prod_q15(input, input, d1, &sum);	
		q31_t sum31 = sum >> (1 + shift * 2);
		q31_t res31;
		arm_sqrt_q31(sum31, &res31);
		*output++ = (q15_t)(res31 >> 15); 
		input += d1;
	}
}
#pragma IMAGINET_FRAGMENT_END


// ===== complex (d1=2) =====
#pragma IMAGINET_FRAGMENT_BEGIN "norm_cmsis_cmplx_q15"
static inline void norm_cmsis_cmplx_q15(const q15_t* restrict input, int d2, q15_t* restrict output, int shift)
{
	arm_cmplx_mag_q15(input, output, d2);
	if(shift != 1)
		arm_shift_q15(output, 1-shift, output, d2);
}
#pragma IMAGINET_FRAGMENT_END
