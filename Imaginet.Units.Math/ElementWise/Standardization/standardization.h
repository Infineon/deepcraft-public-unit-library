#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "standardization_f32"
static inline void standardization_f32(const float* restrict x, int count, float* restrict output, float mean, float std)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - mean) / std;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "standardization_i8"
static inline void standardization_i8(const int8_t* restrict x, int count, int8_t* restrict output, float mean, float std)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - mean) / std;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "standardization_u8"
static inline void standardization_u8(const uint8_t* restrict x, int count, uint8_t* restrict output, float mean, float std)
{
	for (int i = 0; i < count; i++) {
		output[i] = (x[i] - mean) / std;
	}
}
#pragma IMAGINET_FRAGMENT_END