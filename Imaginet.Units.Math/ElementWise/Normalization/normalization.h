#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <float.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "normalization_f32"
static inline void normalization_f32(const float* restrict x, int count, float* restrict output, float min, float max)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - min)/(max - min);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "normalization_i8"
static inline void normalization_i8(const int8_t* restrict x, int count, int8_t* restrict output, float min, float max)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - min)/(max - min);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "normalization_u8"
static inline void normalization_u8(const uint8_t* restrict x, int count, uint8_t* restrict output, float min, float max)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - min)/(max - min);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "batch_normalization_f32"
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