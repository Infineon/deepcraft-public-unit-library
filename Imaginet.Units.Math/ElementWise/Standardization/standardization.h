#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "standardization_f32"
static inline void standardization_f32(const float* restrict x, int count, float* restrict output, float mean, float std)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - mean) / std;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "standardization_i8"
static inline void standardization_i8(const int8_t* restrict x, int count, int8_t* restrict output, float mean, float std)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - mean) / std;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "standardization_u8"
static inline void standardization_u8(const uint8_t* restrict x, int count, uint8_t* restrict output, float mean, float std)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - mean) / std;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "batch_standardization_f32"
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