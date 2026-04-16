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

#pragma IMAGINET_FRAGMENT_BEGIN "subir_i8"
static inline void subir_i8(
	const int8_t* restrict x,
	int count,
	int8_t immediate,
	int8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = immediate - x[i];
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "subir_i16"
static inline void subir_i16(
	const int16_t* restrict x,
	int count,
	int16_t immediate,
	int16_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = immediate - x[i];
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "subir_i32"
static inline void subir_i32(
	const int32_t* restrict x,
	int count,
	int32_t immediate,
	int32_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = immediate - x[i];
	}
}
#pragma IMAGINET_FRAGMENT_END
