#pragma IMAGINET_FRAGMENT_BEGIN "addi_f32"
static inline void addi_f32(
	const float* restrict x,
	int count,
	float immediate,
	float* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = x[i] + immediate;
	}
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "addi_i8"
static inline void addi_i8(
	const int8_t* restrict x,
	int count,
	int8_t immediate,
	int8_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = x[i] + immediate;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "addi_i16"
static inline void addi_i16(
	const int16_t* restrict x,
	int count,
	int16_t immediate,
	int16_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = x[i] + immediate;
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "addi_i32"
static inline void addi_i32(
	const int32_t* restrict x,
	int count,
	int32_t immediate,
	int32_t* restrict output)
{
	for (int i = 0; i < count; i++) {
		output[i] = x[i] + immediate;
	}
}
#pragma IMAGINET_FRAGMENT_END



