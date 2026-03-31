#pragma IMAGINET_FRAGMENT_BEGIN "linear_downsample_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "linear_state.h:linear_state_f32"

static inline void linear_downsample_f32(const float* restrict input, float* restrict output, int count, void* state_ptr, float output_samples_per_input)
{
	linear_state_f32* state = (linear_state_f32*)state_ptr;
	float* previous_value = (float*)((char*)state_ptr + sizeof(linear_state_f32));

	int input_samples_per_output = (int)(1.0f / output_samples_per_input + 0.5f);
	int emit = (state->current_inference == input_samples_per_output / 2);

	if (emit)
	{
		if (state->initialized)
		{
			for (int i = 0; i < count; ++i)
				output[i] = (previous_value[i] + input[i]) * 0.5f;
		}
		else
		{
			memcpy(output, input, count * sizeof(float));
		}
	}
	else
	{
		memcpy(output, previous_value, count * sizeof(float));
	}

	memcpy(previous_value, input, count * sizeof(float));

	state->current_inference++;
	if (state->current_inference >= input_samples_per_output)
		state->current_inference = 0;

	state->initialized = 1;
}

#pragma IMAGINET_FRAGMENT_END
