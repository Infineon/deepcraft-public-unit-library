#pragma IMAGINET_FRAGMENT_BEGIN "linear_upsample_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "linear_state.h:linear_state_f32"

static inline void linear_upsample_f32(const float* restrict input, float* restrict output, int count, void* state_ptr, float output_samples_per_input)
{
	linear_state_f32* state = (linear_state_f32*)state_ptr;
	float* previous_value = (float*)((char*)state_ptr + sizeof(linear_state_f32));

	float t = state->current_inference / output_samples_per_input;

	for (int i = 0; i < count; ++i)
		output[i] = state->initialized
			? previous_value[i] + t * (input[i] - previous_value[i])
			: input[i];

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
