#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "norm_sparse_f32"
static inline float __norm_sqrt_sum_sparse_f32(const float* restrict input, int step, int count)
{
	float sum = 0;
	for (int j = 0; j < count; j++) {
		float item = *input;
		sum += item * item;
		input += step;
	}
	return sqrtf(sum);
}

static inline void norm_sparse_f32(const float* restrict input, int d0, int d1, int d2, float* restrict output)
{
	const int d3 = d0 * d1;
	for (int k = 0; k < d2; k++) {
		const float* ip = input + k * d3;
		for (int i = 0; i < d0; i++) {			
			*output++ = __norm_sqrt_sum_sparse_f32(ip++, d0, d1);
		}
	}
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "norm_f32"
static inline float __norm_sqrt_sum_f32(const float* restrict input, int count)
{
	float sum = 0;
	for (int j = 0; j < count; j++) {
		float item = *input++;
		sum += item * item;
	}
	return sqrtf(sum);
}

static inline void norm_f32(const float* restrict input, int d1, int d2, float* restrict output)
{
	for (int k = 0; k < d2; k++) {
		*output++ = __norm_sqrt_sum_f32(input, d1);
		input += d1;
	}
}
#pragma IMAGINET_FRAGMENT_END