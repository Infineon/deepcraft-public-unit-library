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

#pragma IMAGINET_FRAGMENT_BEGIN "average_i8"
static inline int8_t __average_i8(const int8_t* input, int step, int count)
{
	int32_t sum = 0;
	for (int i = 0; i < count; i++) {
		sum += input[i * step];
	}
	return (int8_t)(sum / count);
}

static inline void average_i8(const int8_t* restrict input, int d0, int d1, int d2, int8_t* restrict output)
{
	const int d3 = d0 * d1;
	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __average_i8(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "average_i16"
static inline int16_t __average_i16(const int16_t* input, int step, int count)
{
	int32_t sum = 0;
	for (int i = 0; i < count; i++) {
		sum += input[i * step];
	}
	return (int16_t)(sum / count);
}

static inline void average_i16(const int16_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	const int d3 = d0 * d1;
	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __average_i16(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "average_i32"
static inline int32_t __average_i32(const int32_t* input, int step, int count)
{
	int64_t sum = 0;
	for (int i = 0; i < count; i++) {
		sum += input[i * step];
	}
	return (int32_t)(sum / count);
}

static inline void average_i32(const int32_t* restrict input, int d0, int d1, int d2, int32_t* restrict output)
{
	const int d3 = d0 * d1;
	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __average_i32(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END