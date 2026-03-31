#pragma IMAGINET_FRAGMENT_BEGIN "nearest_downsample_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "nearest_state.h:nearest_state_f32"

static inline void nearest_downsample_f32(const float* restrict input, float* restrict output, int count, void* state_ptr, float output_samples_per_input)
{
	nearest_state_f32* state = (nearest_state_f32*)state_ptr;
	float* previous_value = (float*)((char*)state_ptr + sizeof(nearest_state_f32));

	int input_samples_per_output = (int)(1.0f / output_samples_per_input + 0.5f);
	int emit = (state->current_inference == input_samples_per_output / 2);

	const float* out_src = emit ? input : previous_value;
	memcpy(output, out_src, count * sizeof(float));
	memcpy(previous_value, input, count * sizeof(float));

	state->current_inference++;
	if (state->current_inference >= input_samples_per_output)
		state->current_inference = 0;

	state->initialized = 1;
}

#pragma IMAGINET_FRAGMENT_END
