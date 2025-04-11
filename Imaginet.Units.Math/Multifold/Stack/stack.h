#pragma IMAGINET_INCLUDES_BEGIN
#include <string.h>
#pragma IMAGINET_INCLUDES_END


#pragma IMAGINET_FRAGMENT_BEGIN "__stack_f32"
static inline void __stack_f32(const float** restrict input, const int step, const int size, const int slot, float* restrict output)
{
	for (int j = 0; j < slot; j++) {
		for (int i = 0; i < size; i++) {
			const float* ip = input[i] + j*step;
			for (int k = 0; k < step; k++) {
				*output++ = ip[k];
			}
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "stack_x2_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "__stack_f32"
static inline void stack_x2_f32(
	const float* restrict d0,
	const float* restrict d1
	int step, int size, int slot,
	float* restrict output)
{
	const float* arrays[4];
	const float** ca = arrays;

	if (d0 != NULL)
		*ca++ = d0;

	if (d1 != NULL)
		*ca++ = d1;

	__stack_f32(arrays, step, size, slot, output);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "stack_x4_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "__stack_f32"
static inline void stack_x4_f32(
	const float* restrict d0,
	const float* restrict d1, 
	const float* restrict d2,
	const float* restrict d3, 
	int step, int size, int slot,
	float* restrict output)
{	
	const float* arrays[4];
	const float** ca = arrays;

	if (d0 != NULL)
		*ca++ = d0;

	if (d1 != NULL)
		*ca++ = d1;

	if (d2 != NULL)
		*ca++ = d2;

	if (d3 != NULL)
		*ca++ = d3;

	__stack_f32(arrays, step, size, slot, output);
}
#pragma IMAGINET_FRAGMENT_END