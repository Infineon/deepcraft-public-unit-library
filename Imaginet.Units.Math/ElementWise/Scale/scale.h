#pragma IMAGINET_FRAGMENT_BEGIN "scale_f32"
static inline void scale_f32(
	const float* restrict x,
	int count,
	float scale,
	float* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = x[i] * scale;
	}
}
#pragma IMAGINET_FRAGMENT_END
