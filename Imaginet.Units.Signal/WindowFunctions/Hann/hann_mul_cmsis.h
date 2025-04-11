#pragma IMAGINET_INCLUDES_BEGIN
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "hannmul_cmsis_f32"
static inline void hannmul_cmsis_f32(const float* restrict a, const float* restrict b, float* restrict result, int d0, int d1)
{
	for (int j = 0; j < d1; j++) {
		arm_mult_f32(a, b, result, d0);
		a += d0;
		result += d0;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "hannmul_cmsis_q31"
static inline void hannmul_cmsis_q31(const q31_t* restrict a, const q31_t* restrict b, q31_t* restrict result, int d0, int d1)
{
	for (int j = 0; j < d1; j++) {	
		arm_mult_q31(a, b, result, d0);
		a += d0;
		result += d0;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "hannmul_cmsis_sparse_q31"
#define ARM_MULT_Q31_SAT(a,b)  (__SSAT((q31_t)(((q63_t)(a) * (b)) >> 32), 31) << 1U)
static inline void hannmul_cmsis_sparse_q31(const q31_t* restrict input, const q31_t* restrict w, int d0, int d1, int d2, q31_t* restrict output)
{
	const int d3 = d0 * d1;

	const q31_t* ip = input;
	q31_t* op = output;

	for (int j = 0; j < d2; j++) {
		for (int i = 0; i < d0; i++) {
			for (int k = 0; k < d1; k++) {
				op[k * d0 + i] = ARM_MULT_Q31_SAT(ip[k * d0 + i], w[k]);
			}
		}

		ip += d3;
		op += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "hannmul_cmsis_q15"
static inline void hannmul_cmsis_q15(const q15_t* restrict a, const q15_t* restrict b, q15_t* restrict result, int d0, int d1)
{
	for (int j = 0; j < d1; j++) {
		arm_mult_q15(a, b, result, d0);
		a += d0;
		result += d0;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "hannmul_cmsis_sparse_q15"
#define ARM_MULT_Q15_SAT(a,b)  ((q15_t)__SSAT((((q31_t) (a) * (b)) >> 15), 16))
static inline void hannmul_cmsis_sparse_q15(const q15_t* restrict input, const q15_t* restrict w, int d0, int d1, int d2, q15_t* restrict output)
{
	const int d3 = d0 * d1;

	const q15_t* ip = input;
	q15_t* op = output;

	for (int j = 0; j < d2; j++) {
		for (int i = 0; i < d0; i++) {
			for (int k = 0; k < d1; k++) {
				op[k * d0 + i] = ARM_MULT_Q15_SAT(ip[k * d0 + i], w[k]);
			}
		}

		ip += d3;
		op += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "hannmul_cmsis_q7"
static inline void hannmul_cmsis_q7(const q7_t* restrict a, const q7_t* restrict b, q7_t* restrict result, int d0, int d1)
{
	for (int j = 0; j < d1; j++) {
		arm_mult_q7(a, b, result, d0);
		a += d0;
		result += d0;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "hannmul_cmsis_sparse_q7"
#define ARM_MULT_Q7_SAT(a,b)  ((q7_t)__SSAT((((q15_t) (a) * (b)) >> 7), 8))
static inline void hannmul_cmsis_sparse_q7(const q7_t* restrict input, const q7_t* restrict w, int d0, int d1, int d2, q7_t* restrict output)
{
	const int d3 = d0 * d1;

	const q7_t* ip = input;
	q7_t* op = output;

	for (int j = 0; j < d2; j++) {
		for (int i = 0; i < d0; i++) {
			for (int k = 0; k < d1; k++) {
				op[k * d0 + i] = ARM_MULT_Q7_SAT(ip[k * d0 + i], w[k]);
			}
		}

		ip += d3;
		op += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END