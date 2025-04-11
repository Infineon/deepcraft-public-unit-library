#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "average_f32"

static inline float __average_f32(const float* input, int step, int count)
{
	float avg = 0;
	for (int i = 0; i < count; i++) {
		avg += input[i * step];
	}

	return avg / (float)count;
}

// input array (any shape >= 2D)
// output array (input.shape.remove(axis))
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void average_f32(const float* restrict input, int d0, int d1, int d2, float* restrict output)
{
	const int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __average_f32(input + i + id, d0, d1);
		}
	}
}

#pragma IMAGINET_FRAGMENT_END