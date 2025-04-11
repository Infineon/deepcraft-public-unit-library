#pragma IMAGINET_INCLUDES_BEGIN
#include <float.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "argmax_f32"

static inline int16_t __argmax_f32(const float* restrict input, int step, int count)
{
	float max = -FLT_MAX;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		float value = input[i * step];
		if (value > max) {
			index = i;
			max = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmax_f32(const float* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmax_f32(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END


