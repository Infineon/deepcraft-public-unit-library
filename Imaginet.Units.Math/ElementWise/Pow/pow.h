#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "pow_f32"
static inline void pow_f32(const float* restrict x, int count, float exponent, float* restrict result)
{
	for (int i = 0; i < count; i++) {
		*result++ = powf(*x++, exponent);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "pow_i8"
static inline void pow_i8(const int8_t* restrict x, int count, float exponent, int8_t* restrict result)
{
	for (int i = 0; i < count; i++) {
		float val = powf((float)*x++, exponent);
		if (val > 127.0f) val = 127.0f;
		else if (val < -128.0f) val = -128.0f;
		*result++ = (int8_t)val;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "pow_i16"
static inline void pow_i16(const int16_t* restrict x, int count, float exponent, int16_t* restrict result)
{
	for (int i = 0; i < count; i++) {
		float val = powf((float)*x++, exponent);
		if (val > 32767.0f) val = 32767.0f;
		else if (val < -32768.0f) val = -32768.0f;
		*result++ = (int16_t)val;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "pow_i32"
static inline void pow_i32(const int32_t* restrict x, int count, float exponent, int32_t* restrict result)
{
	for (int i = 0; i < count; i++) {
		float val = powf((float)*x++, exponent);
		if (val > 2147483647.0f) val = 2147483647.0f;
		else if (val < -2147483648.0f) val = -2147483648.0f;
		*result++ = (int32_t)val;
	}
}
#pragma IMAGINET_FRAGMENT_END

