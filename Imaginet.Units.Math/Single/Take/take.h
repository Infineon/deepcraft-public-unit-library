#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "take_f32"
static inline void take_f32(const float* restrict input, const int step, const int size, const int slot, int index, float* restrict output)
{
	const int m0 = step * size;
	input += step * index;
	for (int j = 0; j < slot; j++) {			
		for (int i = 0; i < step; i++) {			
			*output++ = input[i];
		}
		input += m0;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "take_i8"
static inline void take_i8(const int8_t* restrict input, const int step, const int size, const int slot, int index, int8_t* restrict output)
{
	const int m0 = step * size;
	input += step * index;
	for (int j = 0; j < slot; j++) {
		for (int i = 0; i < step; i++) {
			*output++ = input[i];
		}
		input += m0;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "take_i16"
static inline void take_i16(const int16_t* restrict input, const int step, const int size, const int slot, int index, int16_t* restrict output)
{
	const int m0 = step * size;
	input += step * index;
	for (int j = 0; j < slot; j++) {
		for (int i = 0; i < step; i++) {
			*output++ = input[i];
		}
		input += m0;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "take_i32"
static inline void take_i32(const int32_t* restrict input, const int step, const int size, const int slot, int index, int32_t* restrict output)
{
	const int m0 = step * size;
	input += step * index;
	for (int j = 0; j < slot; j++) {
		for (int i = 0; i < step; i++) {
			*output++ = input[i];
		}
		input += m0;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "take_u8"
static inline void take_u8(const uint8_t* restrict input, const int step, const int size, const int slot, int index, uint8_t* restrict output)
{
	const int m0 = step * size;
	input += step * index;
	for (int j = 0; j < slot; j++) {
		for (int i = 0; i < step; i++) {
			*output++ = input[i];
		}
		input += m0;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "take_u16"
static inline void take_u16(const uint16_t* restrict input, const int step, const int size, const int slot, int index, uint16_t* restrict output)
{
	const int m0 = step * size;
	input += step * index;
	for (int j = 0; j < slot; j++) {
		for (int i = 0; i < step; i++) {
			*output++ = input[i];
		}
		input += m0;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "take_u32"
static inline void take_u32(const uint32_t* restrict input, const int step, const int size, const int slot, int index, uint32_t* restrict output)
{
	const int m0 = step * size;
	input += step * index;
	for (int j = 0; j < slot; j++) {
		for (int i = 0; i < step; i++) {
			*output++ = input[i];
		}
		input += m0;
	}
}
#pragma IMAGINET_FRAGMENT_END
