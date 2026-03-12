#pragma IMAGINET_INCLUDES_BEGIN
#include <float.h>
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "argmin_i32"
static inline int16_t __argmin_i32(const int32_t* restrict input, int step, int count)
{
	int32_t min = INT_MAX;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		int32_t value = input[i * step];
		if (value < min) {
			index = i;
			min = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmin_i32(const int32_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmin_i32(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END



#pragma IMAGINET_FRAGMENT_BEGIN "argmin_i16"
static inline int16_t __argmin_i16(const int16_t* restrict input, int step, int count)
{
	int16_t min = SHRT_MAX;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		int16_t value = input[i * step];
		if (value < min) {
			index = i;
			min = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmin_i16(const int16_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmin_i16(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "argmin_i8"
static inline int16_t __argmin_i8(const int8_t* restrict input, int step, int count)
{
	int8_t min = SCHAR_MAX;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		int8_t value = input[i * step];
		if (value < min) {
			index = i;
			min = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmin_i8(const int8_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmin_i8(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "argmin_u32"
static inline int16_t __argmin_u32(const uint32_t* restrict input, int step, int count)
{
	uint32_t min = 0; 
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		uint32_t value = input[i * step];
		if (value < min) {
			index = i;
			min = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmin_u32(const uint32_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmin_u32(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "argmin_u16"
static inline int16_t __argmin_u16(const uint16_t* restrict input, int step, int count)
{
	uint16_t min = 0;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		uint16_t value = input[i * step];
		if (value < min) {
			index = i;
			min = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmin_u16(const uint16_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmin_u16(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "argmin_u8"
static inline int16_t __argmin_u8(const uint8_t* restrict input, int step, int count)
{
	uint8_t min = 0;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		uint8_t value = input[i * step];
		if (value < min) {
			index = i;
			min = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmin_u8(const uint8_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmin_u8(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END
