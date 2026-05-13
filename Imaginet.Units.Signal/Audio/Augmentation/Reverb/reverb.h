#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "reverb_state_t"
typedef struct {
	int comb_pos[4];
	int ap_pos[2];
} reverb_state_t;
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_DEPENDENCY "reverb_state_t"

#pragma IMAGINET_FRAGMENT_BEGIN "reverb_f32"
static inline void reverb_f32(const float* restrict input, int8_t* restrict state_bytes,
	float* restrict output, int count,
	int c0, int c1, int c2, int c3, int a0, int a1,
	float feedback, float damp1, float wet, float dry)
{
	reverb_state_t* state = (reverb_state_t*)state_bytes;
	float* mem = (float*)(state_bytes + sizeof(reverb_state_t));

	float* comb_lp = mem;

	int comb_len[4];
	comb_len[0] = c0;
	comb_len[1] = c1;
	comb_len[2] = c2;
	comb_len[3] = c3;

	int ap_len[2];
	ap_len[0] = a0;
	ap_len[1] = a1;

	float* comb_buf[4];
	int offset = 4 * count;
	comb_buf[0] = mem + offset; offset += c0 * count;
	comb_buf[1] = mem + offset; offset += c1 * count;
	comb_buf[2] = mem + offset; offset += c2 * count;
	comb_buf[3] = mem + offset; offset += c3 * count;

	float* ap_buf[2];
	ap_buf[0] = mem + offset; offset += a0 * count;
	ap_buf[1] = mem + offset;

	float damp2 = 1.0f - damp1;

	for (int i = 0; i < count; i++) {
		output[i] = 0.0f;
	}

	for (int c = 0; c < 4; c++) {
		int pos = state->comb_pos[c];
		int cap = comb_len[c] * count;
		float* buf = comb_buf[c];
		float* lp = comb_lp + c * count;

		for (int i = 0; i < count; i++) {
			float delayed = buf[pos + i];
			lp[i] = delayed * damp2 + lp[i] * damp1;
			buf[pos + i] = input[i] + lp[i] * feedback;
			output[i] += delayed;
		}

		pos += count;
		if (pos >= cap) {
			pos = 0;
		}
		state->comb_pos[c] = pos;
	}

	for (int i = 0; i < count; i++) {
		output[i] *= 0.25f;
	}

	for (int a = 0; a < 2; a++) {
		int pos = state->ap_pos[a];
		int cap = ap_len[a] * count;
		float* buf = ap_buf[a];

		for (int i = 0; i < count; i++) {
			float delayed = buf[pos + i];
			float in_val = output[i];
			output[i] = delayed - in_val;
			buf[pos + i] = in_val + delayed * 0.5f;
		}

		pos += count;
		if (pos >= cap) {
			pos = 0;
		}
		state->ap_pos[a] = pos;
	}

	for (int i = 0; i < count; i++) {
		output[i] = dry * input[i] + wet * output[i];
	}
}
#pragma IMAGINET_FRAGMENT_END
