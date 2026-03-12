#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "scale_f32"
static inline void scale_f32(const float* restrict x, int count, float scale, float* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = x[i] * scale;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "scale_i8"
static inline void scale_i8(const int8_t* restrict x, int count, float scale, int8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = x[i] * scale;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "scale_u8"
static inline void scale_u8(const uint8_t* restrict x, int count, float scale, uint8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = x[i] * scale;
	}
}
#pragma IMAGINET_FRAGMENT_END