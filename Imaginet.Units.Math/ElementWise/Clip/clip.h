#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "clip_f32"
static inline void clip_f32(const float* restrict input, int count, float min, float max, float* restrict output)
{	
	for (int i = 0; i < count; i++) {
		float value = input[i];
		if (value > max)
			value = max;
		if (value < min)
			value = min;

		output[i] = value;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "clip_i8"
static inline void clip_i8(const int8_t* restrict input, int count, int8_t min, int8_t max, int8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		int8_t value = input[i];
		if (value > max)
			value = max;
		if (value < min)
			value = min;

		output[i] = value;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "clip_i16"
static inline void clip_i16(const int16_t* restrict input, int count, int16_t min, int16_t max, int16_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		int16_t value = input[i];
		if (value > max)
			value = max;
		if (value < min)
			value = min;

		output[i] = value;
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "clip_i32"
static inline void clip_i32(const int32_t* restrict input, int count, int32_t min, int32_t max, int32_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		int32_t value = input[i];
		if (value > max)
			value = max;
		if (value < min)
			value = min;

		output[i] = value;
	}
}
#pragma IMAGINET_FRAGMENT_END