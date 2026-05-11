#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "delay_state_t"
typedef struct {
	int write_pos;
} delay_state_t;
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_DEPENDENCY "delay_state_t"

#pragma IMAGINET_FRAGMENT_BEGIN "delay_f32"
static inline void delay_f32(const float* restrict input, int8_t* restrict state_bytes,
	float* restrict output, int count, int delay_samples)
{
	delay_state_t* state = (delay_state_t*)state_bytes;
	float* buffer = (float*)(state_bytes + sizeof(delay_state_t));
	int capacity = delay_samples * count;

	for (int i = 0; i < count; i++) {
		output[i] = buffer[state->write_pos + i];
	}

	for (int i = 0; i < count; i++) {
		buffer[state->write_pos + i] = input[i];
	}

	state->write_pos += count;
	if (state->write_pos >= capacity) {
		state->write_pos = 0;
	}
}
#pragma IMAGINET_FRAGMENT_END
