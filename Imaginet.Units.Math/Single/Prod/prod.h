#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

// a.shape == output.shape
#pragma IMAGINET_FRAGMENT_BEGIN "prod_f32"
static inline float __prod_f32(const float* restrict input, int step, int count)
{
	float prod = 1;
	for (int j = 0; j < count; j++) {
		prod *= input[j * step];
	}
	return prod;
}

// a = input array (any shape >= 1D)
// axis = axis to multiply on
// b = input array (shape=a.shape.size(axis))
// output array (shape=a.shape)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void prod_f32(const float* restrict input, int d0, int d1, int d2, float* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __prod_f32(input + i + id, d0, d1);
		}
	}
}

#pragma IMAGINET_FRAGMENT_END
