#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "cosh_f32"
static inline void cosh_f32(const float* restrict x, int count, float* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = cosh(x[i]);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cosh_i8"
static inline void cosh_i8(const int8_t* restrict x, int count, int8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = cosh(x[i]);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "cosh_u8"
static inline void cosh_u8(const uint8_t* restrict x, int count, uint8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = cosh(x[i]);
	}
}
#pragma IMAGINET_FRAGMENT_END
