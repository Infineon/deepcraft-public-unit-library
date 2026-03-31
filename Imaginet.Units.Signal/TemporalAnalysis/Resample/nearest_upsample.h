#pragma IMAGINET_FRAGMENT_BEGIN "nearest_upsample_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "nearest_state.h:nearest_state_f32"

static inline void nearest_upsample_f32(const float* restrict input, float* restrict output, int count, void* state_ptr, float output_samples_per_input)
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
