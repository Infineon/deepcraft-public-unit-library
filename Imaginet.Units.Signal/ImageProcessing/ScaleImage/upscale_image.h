#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "upscale_image_f32"

static inline void upscale_image_f32(
	const float* restrict input,
	float* restrict output,
	int scale_factor,
	int x_size,
	int y_size,
	int channels)
{
	for (int y = 0; y < y_size; ++y)
		for (int sy = 0; sy < scale_factor; ++sy)
			for (int x = 0; x < x_size; ++x)
				for (int sx = 0; sx < scale_factor; ++sx)
					for (int c = 0; c < channels; ++c)  
						*(output++) = input[(y*x_size + x)*channels+c];
}

#pragma IMAGINET_FRAGMENT_END