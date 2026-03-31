#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "nearest_state_f32"
typedef struct {       
    int initialized;       // To handle first inference
    int current_inference; // Modulo cycle counter
} nearest_state_f32;
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "nearest_init_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "nearest_state_f32"

static inline int nearest_init_f32(int count, void* state_ptr)
{
	nearest_state_f32* state = (nearest_state_f32*)state_ptr;
	state->initialized = 0;
	state->current_inference = 0;
	return 0;
}

#pragma IMAGINET_FRAGMENT_END
