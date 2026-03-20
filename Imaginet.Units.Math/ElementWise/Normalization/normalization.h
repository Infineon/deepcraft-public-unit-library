#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "normalization_f32"
static inline void normalization_f32(const float* restrict x, int count, float* restrict output, float min, float max)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - min)/(max - min);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "normalization_i8"
static inline void normalization_i8(const int8_t* restrict x, int count, int8_t* restrict output, float min, float max)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - min)/(max - min);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "normalization_u8"
static inline void normalization_u8(const uint8_t* restrict x, int count, uint8_t* restrict output, float min, float max)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - min)/(max - min);
	}
}
#pragma IMAGINET_FRAGMENT_END