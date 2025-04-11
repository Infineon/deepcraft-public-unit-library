#pragma IMAGINET_INCLUDES_BEGIN
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "min_cmsis_sparse_f32"
static inline float __min_cmsis_sparse_f32(const float* restrict input, int step, int size)
{
	float min_value = *input;
	for (int i = 0; i < size; i++) {
		float value = *input;
		if (value < min_value)
			min_value = value;
		input += step;
	}
	return min_value;
}

static inline void min_cmsis_sparse_f32(const float* restrict input, int step, int size, int slot, float* restrict output)
{
	int d3 = step * size;

	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __min_cmsis_sparse_f32(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_cmsis_sparse_q31"
static inline q31_t __min_cmsis_sparse_q31(const q31_t* restrict input, int step, int size)
{
	q31_t min_value = *input;
	for (int i = 0; i < size; i++) {
		float value = *input;
		if (value < min_value)
			min_value = value;
		input += step;
	}
	return min_value;
}

static inline void min_cmsis_sparse_q31(const q31_t* restrict input, int step, int size, int slot, q31_t* restrict output)
{
	int d3 = step * size;

	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __min_cmsis_sparse_q31(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_cmsis_sparse_q15"
static inline q15_t __min_cmsis_sparse_q15(const q15_t* restrict input, int step, int size)
{
	q15_t min_value = *input;
	for (int i = 0; i < size; i++) {
		float value = *input;
		if (value < min_value)
			min_value = value;
		input += step;
	}
	return min_value;
}

static inline void min_cmsis_sparse_q15(const q15_t* restrict input, int step, int size, int slot, q15_t* restrict output)
{
	int d3 = step * size;

	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __min_cmsis_sparse_q15(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_cmsis_sparse_q7"
static inline q7_t __min_cmsis_sparse_q7(const q7_t* restrict input, int step, int size)
{
	q7_t min_value = *input;
	for (int i = 0; i < size; i++) {
		float value = *input;
		if (value < min_value)
			min_value = value;
		input += step;
	}
	return min_value;
}

static inline void min_cmsis_sparse_q7(const q7_t* restrict input, int step, int size, int slot, q7_t* restrict output)
{
	int d3 = step * size;

	for (int k = 0; k < slot; k++) {
		for (int i = 0; i < step; i++) {
			*output++ = __min_cmsis_sparse_q7(input + i, step, size);
		}
		input += d3;
	}
}
#pragma IMAGINET_FRAGMENT_END

/********************/

#pragma IMAGINET_FRAGMENT_BEGIN "min_cmsis_f32"
static inline void min_cmsis_f32(const float* restrict input, int size, int slot, float* restrict output)
{
	for (int k = 0; k < slot; k++) {
		arm_min_no_idx_f32(input, size, output++);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_cmsis_q31"
static inline void min_cmsis_q31(const q31_t* restrict input, int size, int slot, q31_t* restrict output)
{
	for (int k = 0; k < slot; k++) {
		arm_min_no_idx_q31(input, size, output++);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_cmsis_q15"
static inline void min_cmsis_q15(const q15_t* restrict input, int size, int slot, q15_t* restrict output)
{
	for (int k = 0; k < slot; k++) {
		arm_min_no_idx_q15(input, size, output++);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "min_cmsis_q7"
static inline void min_cmsis_q7(const q7_t* restrict input, int size, int slot, q7_t* restrict output)
{
	for (int k = 0; k < slot; k++) {
		arm_min_no_idx_q7(input, size, output++);
		input += size;
	}
}
#pragma IMAGINET_FRAGMENT_END