#pragma IMAGINET_FRAGMENT_BEGIN "linear_upsample_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "linear_state.h:linear_state_f32"

static inline void linear_upsample_f32(const float* restrict input, float* restrict output, int count, void* state_ptr, float output_samples_per_input)
{
	linear_state_f32* state = (linear_state_f32*)state_ptr;
	float* previous_value = (float*)((char*)state_ptr + sizeof(linear_state_f32));

	if (state->current_inference < 0)
	{
		memcpy(previous_value, input, count * sizeof(float));
		state->current_inference = 0;
	}

	float t = state->current_inference / output_samples_per_input;
	for (int i = 0; i < count; ++i)
		output[i] = previous_value[i] + t * (input[i] - previous_value[i]);

	state->current_inference++;
	if (state->current_inference >= (int)output_samples_per_input)
	{
		memcpy(previous_value, input, count * sizeof(float));
		state->current_inference = 0;
	}
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "linear_upsample_i8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "linear_state.h:linear_state_i8"

static inline void linear_upsample_i8(const int8_t* restrict input, int8_t* restrict output, int count, void* state_ptr, float output_samples_per_input)
{
	linear_state_i8* state = (linear_state_i8*)state_ptr;
	int8_t* previous_value = (int8_t*)((char*)state_ptr + sizeof(linear_state_i8));

	if (state->current_inference < 0)
	{
		memcpy(previous_value, input, count * sizeof(int8_t));
		state->current_inference = 0;
	}

	float t = state->current_inference / output_samples_per_input;
	for (int i = 0; i < count; ++i)
	{
		float val = previous_value[i] + t * (input[i] - previous_value[i]);
		output[i] = (int8_t)(val < -128.0f ? -128 : (val > 127.0f ? 127 : val));
	}

	state->current_inference++;
	if (state->current_inference >= (int)output_samples_per_input)
	{
		memcpy(previous_value, input, count * sizeof(int8_t));
		state->current_inference = 0;
	}
}

#pragma IMAGINET_FRAGMENT_END
