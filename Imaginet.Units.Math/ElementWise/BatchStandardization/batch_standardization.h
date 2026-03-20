#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "batch_standardization_f32"

// Standardize all elements using the global mean and standard deviation of the entire batch.
// count = input.shape.flat
static inline void batch_standardization_f32(const float* restrict x, int count, float* restrict output)
{
	float mean = 0.0f;
	for (int i = 0; i < count; i++) {
		mean += x[i];
	}
	mean /= (float)count;

	float variance = 0.0f;
	for (int i = 0; i < count; i++) {
		float diff = x[i] - mean;
		variance += diff * diff;
	}
	variance /= (float)count;

	float std_dev = sqrtf(variance);
	if (std_dev == 0.0f) std_dev = 1.0f;

	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - mean) / std_dev;
	}
}

#pragma IMAGINET_FRAGMENT_END
