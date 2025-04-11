#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "resize_nearest_f32"

static inline void resize_nearest_f32(const float* restrict input,
    int input_height, int input_width,
    int step_height, int step_width,
    int target_height, int target_width,
    int channels,
    float* restrict output)
{
    const float y_scale = (float)input_height / target_height;
    const float x_scale = (float)input_width / target_width;

    // Directly write to output buffer
    float* out_ptr = output;

    for (int y_out = 0; y_out < target_height; y_out++) {
        const int y_in = (int)(y_out * y_scale + 0.5f);
        const int y_clamped = y_in < 0 ? 0 : (y_in >= input_height ? input_height - 1 : y_in);

        for (int x_out = 0; x_out < target_width; x_out++) {
            const int x_in = (int)(x_out * x_scale + 0.5f);
            const int x_clamped = x_in < 0 ? 0 : (x_in >= input_width ? input_width - 1 : x_in);

            // Calculate source position
            const float* src = input +
                y_clamped * step_height +
                x_clamped * step_width;

            // Copy channels directly to output
            for (int c = 0; c < channels; c++) {
                *out_ptr++ = src[c];
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END