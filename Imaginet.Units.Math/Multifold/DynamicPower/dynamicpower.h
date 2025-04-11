#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "dynamic_power_f32"

static inline void dynamic_power_f32(const float* restrict input, const float* restrict exponent, float* restrict output, int count)
{
    float exp = *exponent;
    for (int i = 0; i < count; i++) {
        *output++ = powf(*input++, exp);
    }
}

#pragma IMAGINET_FRAGMENT_END