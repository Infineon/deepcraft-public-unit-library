#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "subavg_f32"

static inline void __subavg_f32(const float *restrict input, int step, int count, float *restrict output)
{
	double avg = 0;
	for (int i = 0; i < count; i++) {
		avg += input[i * step];
	}

	avg /= (double)count;
	
	for (int i = 0; i < count; i++) {
		output[i * step] = input[i * step] - avg;
	}
}

// input array (any shape >= 2D)
// output array (same shape as input array)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void subavg_f32(const float* restrict input, int d0, int d1, int d2, float* restrict output)
{
	const int d3 = d0 * d1;

	for (int j = 0; j < d2; j++) {
		int j3 = j * d3;
		for (int i = 0; i < d0; i++) {
			__subavg_f32(input + i + j3, d0, d1, output + i + j3);
		}
	}
}

#pragma IMAGINET_FRAGMENT_END