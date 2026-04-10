#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "nv12_to_rgb_uint8"
static inline void nv12_to_rgb_uint8(
	const uint8_t* restrict input,
	int height,
	int width,
	uint8_t* restrict output)
{
	const uint8_t* y_plane  = input;
	const uint8_t* uv_plane = input + height * width;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int Y = (int)y_plane[y * width + x];
			int U = (int)uv_plane[(y >> 1) * width + (x & ~1)];
			int V = (int)uv_plane[(y >> 1) * width + (x & ~1) + 1];

			int r = Y + (int)(1.402f   * (V - 128));
			int g = Y - (int)(0.344136f * (U - 128)) - (int)(0.714136f * (V - 128));
			int b = Y + (int)(1.772f   * (U - 128));

			int idx = (y * width + x) * 3;
			output[idx + 0] = r < 0 ? 0 : (r > 255 ? 255 : (uint8_t)r);
			output[idx + 1] = g < 0 ? 0 : (g > 255 ? 255 : (uint8_t)g);
			output[idx + 2] = b < 0 ? 0 : (b > 255 ? 255 : (uint8_t)b);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "nv12_to_bgr_uint8"
static inline void nv12_to_bgr_uint8(
	const uint8_t* restrict input,
	int height,
	int width,
	uint8_t* restrict output)
{
	const uint8_t* y_plane  = input;
	const uint8_t* uv_plane = input + height * width;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int Y = (int)y_plane[y * width + x];
			int U = (int)uv_plane[(y >> 1) * width + (x & ~1)];
			int V = (int)uv_plane[(y >> 1) * width + (x & ~1) + 1];

			int r = Y + (int)(1.402f   * (V - 128));
			int g = Y - (int)(0.344136f * (U - 128)) - (int)(0.714136f * (V - 128));
			int b = Y + (int)(1.772f   * (U - 128));

			int idx = (y * width + x) * 3;
			output[idx + 0] = b < 0 ? 0 : (b > 255 ? 255 : (uint8_t)b);
			output[idx + 1] = g < 0 ? 0 : (g > 255 ? 255 : (uint8_t)g);
			output[idx + 2] = r < 0 ? 0 : (r > 255 ? 255 : (uint8_t)r);
		}
	}
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "nv12_to_yuv_uint8"
static inline void nv12_to_yuv_uint8(
	const uint8_t* restrict input,
	int height,
	int width,
	uint8_t* restrict output)
{
	const uint8_t* y_plane  = input;
	const uint8_t* uv_plane = input + height * width;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int idx = (y * width + x) * 3;
			output[idx + 0] = y_plane[y * width + x];
			output[idx + 1] = uv_plane[(y >> 1) * width + (x & ~1)];
			output[idx + 2] = uv_plane[(y >> 1) * width + (x & ~1) + 1];
		}
	}
}
#pragma IMAGINET_FRAGMENT_END
