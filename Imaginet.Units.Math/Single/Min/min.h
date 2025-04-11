#pragma IMAGINET_INCLUDES_BEGIN
#include <float.h>
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