#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "softclip_tanh_f32"
static inline void softclip_tanh_f32(const float* restrict x, int count, float* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = tanhf(x[i]);
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "softclip_cubic_f32"
static inline void softclip_cubic_f32(const float* restrict x, int count, float* restrict output)
{
	for (int i = 0; i < count; i++) {
		float t = x[i];
		if (t < -1.0f) {
			t = -1.0f;
		}
		else if (t > 1.0f) {
			t = 1.0f;
		}
		output[i] = 1.5f * t - 0.5f * t * t * t;
	}
}
#pragma IMAGINET_FRAGMENT_END
