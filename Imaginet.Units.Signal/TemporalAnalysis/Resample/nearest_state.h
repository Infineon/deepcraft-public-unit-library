#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "nearest_state_f32"
typedef struct {
    int current_inference; // -1 = uninitialized; >= 0 = modulo cycle counter
} nearest_state_f32;
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "nearest_init_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "nearest_state_f32"

static inline int nearest_init_f32(int count, void* state_ptr)
{
	nearest_state_f32* state = (nearest_state_f32*)state_ptr;
	state->current_inference = -1;
	return 0;
}

#pragma IMAGINET_FRAGMENT_END
