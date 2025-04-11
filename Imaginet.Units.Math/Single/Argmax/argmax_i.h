#pragma IMAGINET_INCLUDES_BEGIN
#include <float.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "argmax_i32"
static inline int16_t __argmax_i32(const int32_t* restrict input, int step, int count)
{
	int32_t max = INT_MIN;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		int32_t value = input[i * step];
		if (value > max) {
			index = i;
			max = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmax_i32(const int32_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmax_i32(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END



#pragma IMAGINET_FRAGMENT_BEGIN "argmax_i16"
static inline int16_t __argmax_i16(const int16_t* restrict input, int step, int count)
{
	int16_t max = SHRT_MIN;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		int16_t value = input[i * step];
		if (value > max) {
			index = i;
			max = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmax_i16(const int16_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmax_i16(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "argmax_i8"
static inline int16_t __argmax_i8(const int8_t* restrict input, int step, int count)
{
	int8_t max = SCHAR_MIN;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		int8_t value = input[i * step];
		if (value > max) {
			index = i;
			max = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmax_i8(const int8_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmax_i8(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "argmax_u32"
static inline int16_t __argmax_u32(const uint32_t* restrict input, int step, int count)
{
	uint32_t max = 0;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		uint32_t value = input[i * step];
		if (value > max) {
			index = i;
			max = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmax_u32(const uint32_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmax_u32(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "argmax_u16"
static inline int16_t __argmax_u16(const uint16_t* restrict input, int step, int count)
{
	uint16_t max = 0;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		uint16_t value = input[i * step];
		if (value > max) {
			index = i;
			max = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmax_u16(const uint16_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmax_u16(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "argmax_u8"
static inline int16_t __argmax_u8(const uint8_t* restrict input, int step, int count)
{
	uint8_t max = 0;
	int16_t index = 0;
	for (int i = 0; i < count; i++) {
		uint8_t value = input[i * step];
		if (value > max) {
			index = i;
			max = value;
		}
	}
	return index;
}

// input array (any shape >= 2D)
// output array (same shape as input array except with axis removed)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void argmax_u8(const uint8_t* restrict input, int d0, int d1, int d2, int16_t* restrict output)
{
	int d3 = d0 * d1;

	for (int k = 0; k < d2; k++) {
		int id = k * d3;
		for (int i = 0; i < d0; i++) {
			output[i + k * d0] = __argmax_u8(input + i + id, d0, d1);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END
