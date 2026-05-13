#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "low_shelf_f32"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static inline void low_shelf_f32(const float* restrict input, int8_t* restrict state_bytes,
	float* restrict output, int count, int sample_rate,
	float frequency, float Q, float gain_db)
{
	float w0 = 2.0f * (float)M_PI * frequency / (float)sample_rate;
	float cos_w0 = cosf(w0);
	float alpha = sinf(w0) / (2.0f * Q);
	float A = powf(10.0f, gain_db / 40.0f);
	float tsa = 2.0f * sqrtf(A) * alpha;
	float a0 = (A + 1.0f) + (A - 1.0f) * cos_w0 + tsa;
	float b0 = (A * ((A + 1.0f) - (A - 1.0f) * cos_w0 + tsa)) / a0;
	float b1 = (2.0f * A * ((A - 1.0f) - (A + 1.0f) * cos_w0)) / a0;
	float b2 = (A * ((A + 1.0f) - (A - 1.0f) * cos_w0 - tsa)) / a0;
	float a1 = (-2.0f * ((A - 1.0f) + (A + 1.0f) * cos_w0)) / a0;
	float a2 = ((A + 1.0f) + (A - 1.0f) * cos_w0 - tsa) / a0;

	float* state = (float*)state_bytes;

	for (int i = 0; i < count; i++) {
		float z1 = state[i * 2];
		float z2 = state[i * 2 + 1];
		float x = input[i];
		float y = b0 * x + z1;
		z1 = b1 * x - a1 * y + z2;
		z2 = b2 * x - a2 * y;
		state[i * 2] = z1;
		state[i * 2 + 1] = z2;
		output[i] = y;
	}
}

#pragma IMAGINET_FRAGMENT_END
