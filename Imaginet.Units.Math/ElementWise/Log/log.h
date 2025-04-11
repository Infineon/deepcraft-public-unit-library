#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "log_f32"
static inline void log_f32(const float* restrict x, int count, float ilbase, float* restrict result)
{
	for (int i = 0; i < count; i++) {
		*result++ = logf(*x++) *ilbase;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "ln_f32"
static inline void ln_f32(const float* restrict x, int count, float* restrict result)
{
	for (int i = 0; i < count; i++) {
		*result++ = logf(*x++);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "log10_f32"
static inline void log10_f32(const float* restrict x, int count, float* restrict result)
{
	for (int i = 0; i < count; i++) {
		*result++ = log10f(*x++);
	}
}
#pragma IMAGINET_FRAGMENT_END