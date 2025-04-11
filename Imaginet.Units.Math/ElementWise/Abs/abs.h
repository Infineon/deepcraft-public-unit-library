#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "abs_f32"
static inline void abs_f32(
	const float* restrict x,
	int count,
	float* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = fabs(x[i]);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "abs_i8"
static inline void abs_i8(
	const int8_t* restrict x,
	int count,
	int8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = abs(x[i]);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "abs_i16"
static inline void abs_i16(
	const int16_t* restrict x,
	int count,
	int16_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = abs(x[i]);
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "abs_i32"
static inline void abs_i32(
	const int32_t* restrict x,
	int count,
	int32_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = abs(x[i]);
	}
}
#pragma IMAGINET_FRAGMENT_END
