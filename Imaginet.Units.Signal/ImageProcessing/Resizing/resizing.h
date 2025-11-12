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

#pragma IMAGINET_FRAGMENT_BEGIN "resize_bilinear_f32"

static inline void resize_bilinear_f32(const float* restrict input,
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
        // Calculate source coordinates with fractional parts
        const float y_src = (y_out + 0.5f) * y_scale - 0.5f;
        const int y_in = (int)y_src;
        const float y_frac = y_src - y_in;

        // Clamp y coordinates
        const int y0 = y_in < 0 ? 0 : (y_in >= input_height ? input_height - 1 : y_in);
        const int y1 = (y_in + 1) >= input_height ? input_height - 1 : (y_in + 1);

        for (int x_out = 0; x_out < target_width; x_out++) {
            // Calculate source coordinates with fractional parts
            const float x_src = (x_out + 0.5f) * x_scale - 0.5f;
            const int x_in = (int)x_src;
            const float x_frac = x_src - x_in;

            // Clamp x coordinates
            const int x0 = x_in < 0 ? 0 : (x_in >= input_width ? input_width - 1 : x_in);
            const int x1 = (x_in + 1) >= input_width ? input_width - 1 : (x_in + 1);

            // Get pointers to the four neighboring pixels
            const float* src_00 = input + y0 * step_height + x0 * step_width;
            const float* src_01 = input + y0 * step_height + x1 * step_width;
            const float* src_10 = input + y1 * step_height + x0 * step_width;
            const float* src_11 = input + y1 * step_height + x1 * step_width;

            // Compute bilinear interpolation weights
            const float w00 = (1.0f - x_frac) * (1.0f - y_frac);
            const float w01 = x_frac * (1.0f - y_frac);
            const float w10 = (1.0f - x_frac) * y_frac;
            const float w11 = x_frac * y_frac;

            // Interpolate each channel
            for (int c = 0; c < channels; c++) {
                *out_ptr++ = w00 * src_00[c] + w01 * src_01[c] + w10 * src_10[c] + w11 * src_11[c];
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END