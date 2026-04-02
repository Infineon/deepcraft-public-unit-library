#pragma IMAGINET_FRAGMENT_BEGIN "nearest_downsample_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "nearest_state.h:nearest_state_f32"

static inline void nearest_downsample_f32(const float* restrict input, float* restrict output, int count, void* state_ptr, int input_samples_per_output)
{
	nearest_state_f32* state = (nearest_state_f32*)state_ptr;
	float* previous_value = (float*)((char*)state_ptr + sizeof(nearest_state_f32));

	if (state->current_inference < 0)
	{
		memcpy(previous_value, input, count * sizeof(float));
		state->current_inference = 0;
	}

	int emit = (state->current_inference == input_samples_per_output / 2);

	const float* out_src = emit ? input : previous_value;
	memcpy(output, out_src, count * sizeof(float));
	memcpy(previous_value, input, count * sizeof(float));

	state->current_inference++;
	if (state->current_inference >= input_samples_per_output)
		state->current_inference = 0;
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "nearest_downsample_i8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "nearest_state.h:nearest_state_i8"

static inline void nearest_downsample_i8(const int8_t* restrict input, int8_t* restrict output, int count, void* state_ptr, int input_samples_per_output)
{
	nearest_state_i8* state = (nearest_state_i8*)state_ptr;
	int8_t* previous_value = (int8_t*)((char*)state_ptr + sizeof(nearest_state_i8));

	if (state->current_inference < 0)
	{
		memcpy(previous_value, input, count * sizeof(int8_t));
		state->current_inference = 0;
	}

	int emit = (state->current_inference == input_samples_per_output / 2);

	const int8_t* out_src = emit ? input : previous_value;
	memcpy(output, out_src, count * sizeof(int8_t));
	memcpy(previous_value, input, count * sizeof(int8_t));

	state->current_inference++;
	if (state->current_inference >= input_samples_per_output)
		state->current_inference = 0;
}

#pragma IMAGINET_FRAGMENT_END
