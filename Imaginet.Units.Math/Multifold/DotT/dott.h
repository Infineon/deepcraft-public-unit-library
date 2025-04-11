#pragma IMAGINET_FRAGMENT_BEGIN "dott_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "mac.h:dot_mac_f32"

static inline void dott_f32(const float *restrict a, const float *restrict b, float *restrict out, int d0, int d1, int d2)
{
	for (int i = 0; i < d2; i++) {
		float* op = out;
		for (int j = 0; j < d1; j++) {
			*op++ = dot_mac_f32(a + j * d0, b, d0);
		}
		out += d1;
		b += d0;
	}
}

#pragma IMAGINET_FRAGMENT_END
