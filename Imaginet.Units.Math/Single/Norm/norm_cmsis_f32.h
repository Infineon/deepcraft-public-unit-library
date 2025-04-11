#pragma IMAGINET_INCLUDES_BEGIN
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "norm_sparse_cmsis_f32"

static inline float __norm_sparse_cmsis_f32(const float* restrict input, int step, int count)
{
	float sum = 0;
	for (int j = 0; j < count; j++) {
		float v = *input;
		sum += ARM_SQ(v);
		input += step;
	}
	return sum;
}

static inline void norm_sparse_cmsis_f32(const float* restrict input, int d0, int d1, int d2, float* restrict output)
{
	const int d3 = d0 * d1;
	for (int k = 0; k < d2; k++) {
		const float* ip = input + k * d3;
		for (int i = 0; i < d0; i++) {	
			arm_sqrt_f32(__norm_sparse_cmsis_f32(ip++, d0, d1), output++);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

// ===== basic =====

#pragma IMAGINET_FRAGMENT_BEGIN "norm_cmsis_f32"
static inline void norm_cmsis_f32(const float* restrict input, int d1, int d2, float* restrict output)
{
	for (int k = 0; k < d2; k++) {
		float sum;
		arm_dot_prod_f32(input, input, d1, &sum);
		arm_sqrt_f32(sum, output++);
		input += d1;
	}
}
#pragma IMAGINET_FRAGMENT_END


// ===== complex (d1=2) =====

#pragma IMAGINET_FRAGMENT_BEGIN "norm_cmsis_cmplx_f32"
static inline void norm_cmsis_cmplx_f32(const float* restrict input, int d2, float* restrict output)
{
	arm_cmplx_mag_f32(input, output, d2);
}
#pragma IMAGINET_FRAGMENT_END
