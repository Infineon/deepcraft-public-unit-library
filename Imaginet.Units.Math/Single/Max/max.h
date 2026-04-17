#pragma IMAGINET_INCLUDES_BEGIN
#include <float.h>
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "max_sparse_f32"

static inline float __max_sparse_f32(const float* restrict input, int step, int size)
{
	float max_value = -FLT_MAX;
	for (int i = 0; i < size; i++) {
		float value = *input;
		if (value > max_value)
			max_value = value;	
		input += step;
	}
	return max_value;
}

static inline void max_sparse_f32(const float* restrict input, int step, int size, int slot, float* restrict output)
{
	int d3 = step * size;

	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __max_sparse_f32(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "max_f32"

static inline float __max_f32(const float* restrict input, int size)
{
	float max_value = -FLT_MAX;
	for (int i = 0; i < size; i++) {
		float value = *input++;
		if (value > max_value)
			max_value = value;
	}
	return max_value;
}

static inline void max_f32(const float* restrict input, int size, int slot, float* restrict output)
{
	for (int k = 0; k < slot; k++) {
		*output++ = __max_f32(input, size);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "max_sparse_i8"
static inline int8_t __max_sparse_i8(const int8_t* restrict input, int step, int size)
{
	int8_t max_value = INT8_MIN;
	for (int i = 0; i < size; i++) {
		int8_t value = *input;
		if (value > max_value)
			max_value = value;
		input += step;
	}
	return max_value;
}

static inline void max_sparse_i8(const int8_t* restrict input, int step, int size, int slot, int8_t* restrict output)
{
	int d3 = step * size;
	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __max_sparse_i8(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "max_i8"
static inline int8_t __max_i8(const int8_t* restrict input, int size)
{
	int8_t max_value = INT8_MIN;
	for (int i = 0; i < size; i++) {
		int8_t value = *input++;
		if (value > max_value)
			max_value = value;
	}
	return max_value;
}

static inline void max_i8(const int8_t* restrict input, int size, int slot, int8_t* restrict output)
{
	for (int k = 0; k < slot; k++) {
		*output++ = __max_i8(input, size);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "max_sparse_i16"
static inline int16_t __max_sparse_i16(const int16_t* restrict input, int step, int size)
{
	int16_t max_value = INT16_MIN;
	for (int i = 0; i < size; i++) {
		int16_t value = *input;
		if (value > max_value)
			max_value = value;
		input += step;
	}
	return max_value;
}

static inline void max_sparse_i16(const int16_t* restrict input, int step, int size, int slot, int16_t* restrict output)
{
	int d3 = step * size;
	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __max_sparse_i16(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "max_i16"
static inline int16_t __max_i16(const int16_t* restrict input, int size)
{
	int16_t max_value = INT16_MIN;
	for (int i = 0; i < size; i++) {
		int16_t value = *input++;
		if (value > max_value)
			max_value = value;
	}
	return max_value;
}

static inline void max_i16(const int16_t* restrict input, int size, int slot, int16_t* restrict output)
{
	for (int k = 0; k < slot; k++) {
		*output++ = __max_i16(input, size);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "max_sparse_i32"
static inline int32_t __max_sparse_i32(const int32_t* restrict input, int step, int size)
{
	int32_t max_value = INT32_MIN;
	for (int i = 0; i < size; i++) {
		int32_t value = *input;
		if (value > max_value)
			max_value = value;
		input += step;
	}
	return max_value;
}

static inline void max_sparse_i32(const int32_t* restrict input, int step, int size, int slot, int32_t* restrict output)
{
	int d3 = step * size;
	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __max_sparse_i32(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "max_i32"
static inline int32_t __max_i32(const int32_t* restrict input, int size)
{
	int32_t max_value = INT32_MIN;
	for (int i = 0; i < size; i++) {
		int32_t value = *input++;
		if (value > max_value)
			max_value = value;
	}
	return max_value;
}

static inline void max_i32(const int32_t* restrict input, int size, int slot, int32_t* restrict output)
{
	for (int k = 0; k < slot; k++) {
		*output++ = __max_i32(input, size);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END