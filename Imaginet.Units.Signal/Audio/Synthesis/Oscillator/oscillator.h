#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "oscillator_f32"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static inline void oscillator_f32(int8_t* restrict state_bytes, float* restrict output,
	int length, float frequency, float sample_rate, float amplitude, float initial_phase)
{
	float* phase_ptr = (float*)state_bytes;
	int8_t* init_flag = state_bytes + sizeof(float);

	if (*init_flag == 0) {
		*phase_ptr = initial_phase;
		*init_flag = 1;
	}

	float phi = *phase_ptr;
	float phase_inc = 2.0f * (float)M_PI * frequency / sample_rate;

	for (int i = 0; i < length; i++) {
		output[i] = amplitude * sinf(phi);
		phi += phase_inc;
	}

	/* Keep phase in [0, 2π) to avoid floating-point drift over long runs. */
	phi = fmodf(phi, 2.0f * (float)M_PI);
	if (phi < 0.0f) {
		phi += 2.0f * (float)M_PI;
	}

	*phase_ptr = phi;
}

#pragma IMAGINET_FRAGMENT_END
