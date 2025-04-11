#pragma IMAGINET_INCLUDES_BEGIN
#include <float.h>
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "mel_cmsis_f32"
// input array (any shape >= 1D)
// output array (same shape as input array except with 0 replaced with num_filter)
// size = input.shape.size(0)
// slot = input.shape.slot(0)
static inline void mel_cmsis_f32(
	const float* restrict input,
	const short* restrict filter_points,
	const float* restrict filter_coefs,
	int size,
	int slot, 
	int num_filter, 
	float* restrict output)
{	
	for (int k = 0; k < slot; k++) {
		const float *ip = input + k * size;
		const float* coefs = filter_coefs;
		for (int i = 0; i < num_filter; i++) {
			
			short n0 = filter_points[i];
			short n2 = filter_points[i + 2];
			short len = n2 - n0 + 1;
			arm_dot_prod_f32(ip + n0, coefs, len, output++);
			coefs += len;
		}
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "mel_cmsis_q31"
static inline void mel_cmsis_q31(
	const q31_t* restrict input,
	const short* restrict filter_points,
	const q31_t* restrict filter_coefs,
	int size,
	int slot,
	int num_filter,
	q31_t* restrict output,
	int shift)
{
	for (int k = 0; k < slot; k++) {
		const q31_t* ip = input + k * size;
		const q31_t* coefs = filter_coefs;
		for (int i = 0; i < num_filter; i++) {
			short n0 = filter_points[i];
			short n2 = filter_points[i + 2];
			short len = n2 - n0 + 1;
			q63_t sum;
			arm_dot_prod_q31(ip + n0, coefs, len, &sum);
			*output++ = sum >> (17 + shift); // 16.48 -> (1+shift).(31-shift)
			coefs += len;
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "mel_cmsis_q15"
static inline void mel_cmsis_q15(
	const q15_t* restrict input,
	const short* restrict filter_points,
	const q15_t* restrict filter_coefs,
	int size,
	int slot,
	int num_filter,
	q15_t* restrict output,
	int shift)
{
	for (int k = 0; k < slot; k++) {
		const q15_t* ip = input + k * size;
		const q15_t* coefs = filter_coefs;
		for (int i = 0; i < num_filter; i++) {
			short n0 = filter_points[i];
			short n2 = filter_points[i + 2];
			short len = n2 - n0 + 1;
			q63_t sum;
			arm_dot_prod_q15(ip + n0, coefs, len, &sum);
			*output++ = sum >> (15 + shift); // 34.30 -> (1+shift).(15-shift)
			coefs += len;
		}
	}
}
#pragma IMAGINET_FRAGMENT_END
