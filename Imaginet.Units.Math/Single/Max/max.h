#pragma IMAGINET_INCLUDES_BEGIN
#include <float.h>
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