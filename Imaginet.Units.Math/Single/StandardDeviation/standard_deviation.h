#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "standard_deviation_f32"

static inline float __standard_deviation_f32(const float* input, int step, int count)
{
	if (count <= 0) {
		return 0.0f;
	}

	float sum = 0.0f;
	for (int i = 0; i < count; i++) {
		sum += input[i * step];
	}
	float mean = sum / (float)count;

	float variance = 0.0f;
	for (int i = 0; i < count; i++) {
		float diff = input[i * step] - mean;
		variance += diff * diff;
	}
	variance /= (float)count;

	return sqrtf(variance);
}

// input array (any shape)
// output array (input.shape.remove(axis))
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void standard_deviation_f32(const float* restrict input, int d0, int d1, int d2, float* restrict output)
{
	const int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __standard_deviation_f32(input + i + id, d0, d1);
		}
	}
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "standard_deviation_i8"

static inline int8_t __standard_deviation_i8(const int8_t* input, int step, int count)
{
	if (count <= 0) {
		return 0;
	}

	float sum = 0.0f;
	for (int i = 0; i < count; i++) {
		sum += (float)input[i * step];
	}
	float mean = sum / (float)count;

	float variance = 0.0f;
	for (int i = 0; i < count; i++) {
		float diff = (float)input[i * step] - mean;
		variance += diff * diff;
	}
	variance /= (float)count;

	return (int8_t)sqrtf(variance);
}

static inline void standard_deviation_i8(const int8_t* restrict input, int d0, int d1, int d2, int8_t* restrict output)
{
	const int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __standard_deviation_i8(input + i + id, d0, d1);
		}
	}
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "standard_deviation_i16"

static inline int16_t __standard_deviation_i16(const int16_t* input, int step, int count)
{
	if (count <= 0) {
		return 0;
	}

	float sum = 0.0f;
	for (int i = 0; i < count; i++) {
		sum += (float)input[i * step];
	}
	float mean = sum / (float)count;

	float variance = 0.0f;
	for (int i = 0; i < count; i++) {
		float diff = (float)input[i * step] - mean;
		variance += diff * diff;
	}
	variance /= (float)count;

	return (int16_t)sqrtf(variance);
}

static inline void standard_deviation_i16(const int16_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	const int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __standard_deviation_i16(input + i + id, d0, d1);
		}
	}
}

#pragma IMAGINET_FRAGMENT_END
