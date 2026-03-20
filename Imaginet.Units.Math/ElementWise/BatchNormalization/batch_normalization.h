#pragma IMAGINET_INCLUDES_BEGIN
#include <float.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "batch_normalization_f32"

// Normalize all elements using the global min and max of the entire batch.
// count = input.shape.flat
static inline void batch_normalization_f32(const float* restrict x, int count, float* restrict output)
{
	float min_val = x[0];
	float max_val = x[0];
	for (int i = 1; i < count; i++) {
		if (x[i] < min_val) min_val = x[i];
		if (x[i] > max_val) max_val = x[i];
	}

	float range = max_val - min_val;
	if (range == 0.0f) range = 1.0f;

	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - min_val) / range;
	}
}

#pragma IMAGINET_FRAGMENT_END
