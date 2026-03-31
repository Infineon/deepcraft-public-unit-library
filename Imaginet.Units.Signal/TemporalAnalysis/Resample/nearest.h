#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "data_struct_f32"
typedef struct {       
    int initialized;       // To handle first inference
} nearest_state_f32;
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "nearest_init_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "data_struct_f32"

static inline void nearest_init_f32(int count, float* state_ptr)
{
	nearest_state_f32* state = (nearest_state_f32*)state_ptr;
	state->initialized = 0;
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "nearest_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "data_struct_f32"

static inline void nearest_f32(const float* restrict input, float* restrict output, int count, float* state_ptr, float output_samples_per_input)
{
	nearest_state_f32 state = (nearest_state_f32*)state_ptr;
	float* previous_value = (float*)((char*)handle + sizeof(nearest_state_f32));

	for (int i = 0; i < count; ++i)
		
		*(output++) = *(input++);


	
}

#pragma IMAGINET_FRAGMENT_END