#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "linear_state_f32"
typedef struct {
    int current_inference; // -1 = uninitialized; >= 0 = modulo cycle counter
} linear_state_f32;
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "linear_init_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "linear_state_f32"

static inline int linear_init_f32(int count, void* state_ptr)
{
	linear_state_f32* state = (linear_state_f32*)state_ptr;
	state->current_inference = -1;
	return 0;
}

#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "linear_state_i8"
typedef struct {
    int current_inference; // -1 = uninitialized; >= 0 = modulo cycle counter
} linear_state_i8;
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "linear_init_i8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "linear_state_i8"

static inline int linear_init_i8(int count, void* state_ptr)
{
	linear_state_i8* state = (linear_state_i8*)state_ptr;
	state->current_inference = -1;
	return 0;
}

#pragma IMAGINET_FRAGMENT_END
