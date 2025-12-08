#pragma IMAGINET_FRAGMENT_BEGIN "sub_f32"
static inline void sub_f32(
	const float* restrict a,
	const float* restrict b,
	int l, int g1, int m, int g2, int r,
	float* restrict output)
{
	int index = 0;
	for (int x = 0; x < l; x++) {
		for (int i = 0; i < g1; i++) {
			for (int y = 0; y < m; y++) {
				for (int j = 0; j < g2; j++) {
					for (int z = 0; z < r; z++) {
						output[index] = a[index] - b[x * m * r + y * r + z];
						index++;
					}
				}
			}
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "sub_i8"
static inline void sub_i8(
	const int8_t* restrict a,
	const int8_t* restrict b,
	int l, int g1, int m, int g2, int r,
	int8_t* restrict output)
{
	int index = 0;
	for (int x = 0; x < l; x++) {
		for (int i = 0; i < g1; i++) {
			for (int y = 0; y < m; y++) {
				for (int j = 0; j < g2; j++) {
					for (int z = 0; z < r; z++) {
						output[index] = a[index] - b[x * m * r + y * r + z];
						index++;
					}
				}
			}
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "sub_i32"
static inline void sub_i32(
	const int32_t* restrict a,
	const int32_t* restrict b,
	int l, int g1, int m, int g2, int r,
	int32_t* restrict output)
{
	int index = 0;
	for (int x = 0; x < l; x++) {
		for (int i = 0; i < g1; i++) {
			for (int y = 0; y < m; y++) {
				for (int j = 0; j < g2; j++) {
					for (int z = 0; z < r; z++) {
						output[index] = a[index] - b[x * m * r + y * r + z];
						index++;
					}
				}
			}
		}
	}
}
#pragma IMAGINET_FRAGMENT_END