#pragma IMAGINET_FRAGMENT_BEGIN "nearest_upsample_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "nearest_state.h:nearest_state_f32"

static inline void nearest_upsample_f32(const float* restrict input, float* restrict output, int count, void* state_ptr, float output_samples_per_input)
{
	nearest_state_f32* state = (nearest_state_f32*)state_ptr;
	float* previous_value = (float*)((char*)state_ptr + sizeof(nearest_state_f32));

	if (state->current_inference < 0)
	{
		memcpy(previous_value, input, count * sizeof(float));
		state->current_inference = 0;
	}

	int use_previous = (state->current_inference < (int)(output_samples_per_input / 2.0f));

	const float* src = use_previous ? previous_value : input;
	memcpy(output, src, count * sizeof(float));

	state->current_inference++;
	if (state->current_inference >= (int)output_samples_per_input)
	{
		memcpy(previous_value, input, count * sizeof(float));
		state->current_inference = 0;
	}
}

#pragma IMAGINET_FRAGMENT_END
