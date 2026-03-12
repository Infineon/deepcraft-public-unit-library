#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "rad2deg_f32"
static inline void rad2deg_f32(const float* restrict x, int count, float* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = x[i] * (180 / M_PI);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rad2deg_i8"
static inline void rad2deg_i8(const int8_t* restrict x, int count, int8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = x[i] * (180 / M_PI);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rad2deg_u8"
static inline void rad2deg_u8(const uint8_t* restrict x, int count, uint8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = x[i] * (180 / M_PI);
	}
}
#pragma IMAGINET_FRAGMENT_END