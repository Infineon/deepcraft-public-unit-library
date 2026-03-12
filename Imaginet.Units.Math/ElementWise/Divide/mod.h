#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "mod_f32"
static inline void mod_f32(const float* restrict x, int count, float divisor, float* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = fmod(x[i], divisor);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "mod_i8"
static inline void mod_i8(const int8_t* restrict x, int count, float divisor, int8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = fmod(x[i], divisor);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "mod_u8"
static inline void mod_u8(const uint8_t* restrict x, int count, float divisor, uint8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = fmod(x[i], divisor);
	}
}
#pragma IMAGINET_FRAGMENT_END