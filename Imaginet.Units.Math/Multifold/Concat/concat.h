#pragma IMAGINET_INCLUDES_BEGIN
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "concat"
static inline void concat(
	const void** restrict input,
	int num_input,
	int step,
	const int* restrict size,
	int slot,
	void* restrict output)
{
	void *input_ptr[num_input];
	memcpy(input_ptr, input, num_input * sizeof(void *));
	for (int j = 0; j < slot; j++) {
		for (int i = 0; i < num_input; i++) {
			int count = size[i];
			memcpy(output, input_ptr[i], count);
			output += count;
			input_ptr[i] += count;
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "concat_x2"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "concat"
static inline void concat_x2(
	const void* restrict input0,
	const void* restrict input1,
	int step, int size0, int size1, int slot,
	void* restrict output)
{
	concat(
		(const void*[]) { input0, input1 }, 
		2, 
		step, 
		(const int[]){ size0 , size1 },
		slot, 
		output);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "concat_x4"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "concat"
static inline void concat_x4(
	const void* restrict input0,
	const void* restrict input1,
	const void* restrict input2,
	const void* restrict input3,
	int step, 
	int size0, 
	int size1, 
	int size2,
	int size3,
	int slot,
	void* restrict output)
{
	concat(
		(const void* []) { input0, input1, input2, input3 }, 
		4, 
		step, 
		(const int[]) { size0, size1, size2, size3 }, 
		slot, 
		output);
}
#pragma IMAGINET_FRAGMENT_END

