#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "data_struct_f32"
typedef struct {       
    int initialized;          // To handle first inference
	int current_inference;    // Modulo cycle
} nearest_state_f32;
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "nearest_init_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "data_struct_f32"

static inline void nearest_init_f32(int count, float* state_ptr)
{
	nearest_state_f32* state = (nearest_state_f32*)state_ptr;
	state->initialized = 0;
	state->current_inference = 0;
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "nearest_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "data_struct_f32"

static inline void nearest_f32(const float* restrict input, float* restrict output, int count, float* state_ptr, float output_samples_per_input)
{
	nearest_state_f32* state = (nearest_state_f32*)state_ptr;
	float* previous_value = (float*)((char*)state_ptr + sizeof(nearest_state_f32));

	int use_previous = state->initialized && (state->current_inference < output_samples_per_input / 2.0f);

	for (int i = 0; i < count; ++i)
		output[i] = use_previous ? previous_value[i] : input[i];

	state->current_inference++;
	if (state->current_inference >= (int)output_samples_per_input)
	{
		for (int i = 0; i < count; ++i)
			previous_value[i] = input[i];
		state->current_inference = 0;
		state->initialized = 1;
	}
}

#pragma IMAGINET_FRAGMENT_END