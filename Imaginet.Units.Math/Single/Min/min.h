#pragma IMAGINET_INCLUDES_BEGIN
#include <float.h>
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_sparse_f32"

static inline float __min_sparse_f32(const float* restrict input, int step, int size)
{
	float min_value = FLT_MAX;
	for (int i = 0; i < size; i++) {
		float value = *input;
		if (value < min_value)
			min_value = value;	
		input += step;
	}
	return min_value;
}

static inline void min_sparse_f32(const float* restrict input, int step, int size, int slot, float* restrict output)
{
	int d3 = step * size;

	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __min_sparse_f32(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "min_f32"

static inline float __min_f32(const float* restrict input, int size)
{
	float min_value = FLT_MAX;
	for (int i = 0; i < size; i++) {
		float value = *input++;
		if (value < min_value)
			min_value = value;
	}
	return min_value;
}

static inline void min_f32(const float* restrict input, int size, int slot, float* restrict output)
{
	for (int k = 0; k < slot; k++) {
		*output++ = __min_f32(input, size);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_sparse_i8"
static inline int8_t __min_sparse_i8(const int8_t* restrict input, int step, int size)
{
	int8_t min_value = INT8_MAX;
	for (int i = 0; i < size; i++) {
		int8_t value = *input;
		if (value < min_value)
			min_value = value;
		input += step;
	}
	return min_value;
}

static inline void min_sparse_i8(const int8_t* restrict input, int step, int size, int slot, int8_t* restrict output)
{
	int d3 = step * size;
	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __min_sparse_i8(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_i8"
static inline int8_t __min_i8(const int8_t* restrict input, int size)
{
	int8_t min_value = INT8_MAX;
	for (int i = 0; i < size; i++) {
		int8_t value = *input++;
		if (value < min_value)
			min_value = value;
	}
	return min_value;
}

static inline void min_i8(const int8_t* restrict input, int size, int slot, int8_t* restrict output)
{
	for (int k = 0; k < slot; k++) {
		*output++ = __min_i8(input, size);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_sparse_i16"
static inline int16_t __min_sparse_i16(const int16_t* restrict input, int step, int size)
{
	int16_t min_value = INT16_MAX;
	for (int i = 0; i < size; i++) {
		int16_t value = *input;
		if (value < min_value)
			min_value = value;
		input += step;
	}
	return min_value;
}

static inline void min_sparse_i16(const int16_t* restrict input, int step, int size, int slot, int16_t* restrict output)
{
	int d3 = step * size;
	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __min_sparse_i16(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_i16"
static inline int16_t __min_i16(const int16_t* restrict input, int size)
{
	int16_t min_value = INT16_MAX;
	for (int i = 0; i < size; i++) {
		int16_t value = *input++;
		if (value < min_value)
			min_value = value;
	}
	return min_value;
}

static inline void min_i16(const int16_t* restrict input, int size, int slot, int16_t* restrict output)
{
	for (int k = 0; k < slot; k++) {
		*output++ = __min_i16(input, size);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_sparse_i32"
static inline int32_t __min_sparse_i32(const int32_t* restrict input, int step, int size)
{
	int32_t min_value = INT32_MAX;
	for (int i = 0; i < size; i++) {
		int32_t value = *input;
		if (value < min_value)
			min_value = value;
		input += step;
	}
	return min_value;
}

static inline void min_sparse_i32(const int32_t* restrict input, int step, int size, int slot, int32_t* restrict output)
{
	int d3 = step * size;
	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __min_sparse_i32(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_i32"
static inline int32_t __min_i32(const int32_t* restrict input, int size)
{
	int32_t min_value = INT32_MAX;
	for (int i = 0; i < size; i++) {
		int32_t value = *input++;
		if (value < min_value)
			min_value = value;
	}
	return min_value;
}

static inline void min_i32(const int32_t* restrict input, int size, int slot, int32_t* restrict output)
{
	for (int k = 0; k < slot; k++) {
		*output++ = __min_i32(input, size);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END