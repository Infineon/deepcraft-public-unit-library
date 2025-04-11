#pragma IMAGINET_FRAGMENT_BEGIN "subir_f32"
static inline void subir_f32(
	const float* restrict x,
	int count,
	float immediate,
	float* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = immediate - x[i];
	}
}
#pragma IMAGINET_FRAGMENT_END
