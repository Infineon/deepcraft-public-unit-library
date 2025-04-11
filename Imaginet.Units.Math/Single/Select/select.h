#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "select_f32"

static inline void select_f32(const float* restrict input, int d0, int d1, int d1o, int d2, int offset, float* restrict output)
{
	const int d3 = d0 * d1;
	const int d3o = d0 * d1o;
	
	const float* ip = input;
	float* op = output;

	offset *= d0;
 
	for (int j = 0; j < d2; j++) {
		for (int i = 0; i < d0; i++) {
			for (int k = 0; k < d1o; k++) {
				int index = k * d0 + i;
				op[index] = ip[index + offset];
			}
		}

		ip += d3;
		op += d3o;
	}
}


#pragma IMAGINET_FRAGMENT_END