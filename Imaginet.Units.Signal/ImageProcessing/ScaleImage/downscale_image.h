#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "downscale_image_f32"

static inline void downscale_image_f32(
    const float* restrict input,
    float* restrict output,
    int scale_factor,
    int x_size,
    int y_size,
    int channels)
{
    int new_x_size = x_size / scale_factor;
    int new_y_size = y_size / scale_factor;

    for (int y = 0; y < new_y_size; ++y) {
        for (int x = 0; x < new_x_size; ++x) {
            for (int c = 0; c < channels; ++c) {
                float sum = 0.0f;
                for (int ky = 0; ky < scale_factor; ++ky) {
                    for (int kx = 0; kx < scale_factor; ++kx) {
                        int input_y = y * scale_factor + ky;
                        int input_x = x * scale_factor + kx;
                        sum += input[(input_y * x_size + input_x) * channels + c];
                    }
                }
                output[(y * new_x_size + x) * channels + c] = sum / (scale_factor * scale_factor);
            }
        }
    }
}


#pragma IMAGINET_FRAGMENT_END