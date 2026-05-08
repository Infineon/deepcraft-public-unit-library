#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "gain_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../Scale/scale.h:scale_f32"
static inline void gain_f32(const float* restrict x, int count, float linear, float* restrict output)
{
	scale_f32(x, count, linear, output);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "gain_i8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../Scale/scale.h:scale_i8"
static inline void gain_i8(const int8_t* restrict x, int count, float linear, int8_t* restrict output)
{
	scale_i8(x, count, linear, output);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "gain_u8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../Scale/scale.h:scale_u8"
static inline void gain_u8(const uint8_t* restrict x, int count, float linear, uint8_t* restrict output)
{
	scale_u8(x, count, linear, output);
}
#pragma IMAGINET_FRAGMENT_END
