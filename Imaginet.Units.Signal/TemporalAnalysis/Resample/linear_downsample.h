#pragma IMAGINET_FRAGMENT_BEGIN "linear_downsample_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "linear_state.h:linear_state_f32"

static inline void linear_downsample_f32(const float* restrict input, float* restrict output, int count, void* state_ptr, int input_samples_per_output)
{
	linear_state_f32* state = (linear_state_f32*)state_ptr;
	float* previous_value = (float*)((char*)state_ptr + sizeof(linear_state_f32));

	if (state->current_inference < 0)
	{
		memcpy(previous_value, input, count * sizeof(float));
		state->current_inference = 0;
	}

	int emit = (state->current_inference == input_samples_per_output / 2);

	if (emit)
	{
		for (int i = 0; i < count; ++i)
			output[i] = (previous_value[i] + input[i]) * 0.5f;
	}
	else
	{
		memcpy(output, previous_value, count * sizeof(float));
	}

	memcpy(previous_value, input, count * sizeof(float));

	state->current_inference++;
	if (state->current_inference >= input_samples_per_output)
		state->current_inference = 0;
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "linear_downsample_i8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "linear_state.h:linear_state_i8"

static inline void linear_downsample_i8(const int8_t* restrict input, int8_t* restrict output, int count, void* state_ptr, int input_samples_per_output)
{
	linear_state_i8* state = (linear_state_i8*)state_ptr;
	int8_t* previous_value = (int8_t*)((char*)state_ptr + sizeof(linear_state_i8));

	if (state->current_inference < 0)
	{
		memcpy(previous_value, input, count * sizeof(int8_t));
		state->current_inference = 0;
	}

	int emit = (state->current_inference == input_samples_per_output / 2);

	if (emit)
	{
		for (int i = 0; i < count; ++i)
			output[i] = (int8_t)(((int)previous_value[i] + (int)input[i]) / 2);
	}
	else
	{
		memcpy(output, previous_value, count * sizeof(int8_t));
	}

	memcpy(previous_value, input, count * sizeof(int8_t));

	state->current_inference++;
	if (state->current_inference >= input_samples_per_output)
		state->current_inference = 0;
}

#pragma IMAGINET_FRAGMENT_END
