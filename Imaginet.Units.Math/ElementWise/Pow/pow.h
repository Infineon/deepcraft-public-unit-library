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

