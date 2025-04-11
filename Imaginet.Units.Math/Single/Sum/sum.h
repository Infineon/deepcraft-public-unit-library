#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "sum_f32"

static inline float __sum_f32(const float* restrict input, int step, int count)
{
	float sum = 0;
	for (int j = 0; j < count; j++) {
		sum += input[j * step];
	}
	return sum;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void sum_f32(const float* restrict input, const int step, const int size, const int slot, float* restrict output)
{
	const int full_step = step * size;

	for (int j = 0; j < slot; j++) {
		const int j3 = j * full_step;
		const int j0 = j * step;
		for (int i = 0; i < step; i++) {
			output[i + j0] = __sum_f32(input + i + j3, step, size);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END
