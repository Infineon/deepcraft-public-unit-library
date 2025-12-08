#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
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

#pragma IMAGINET_FRAGMENT_BEGIN "resize_nearest_uint8"

static inline void resize_nearest_uint8(const uint8_t* restrict input,
    int input_height, int input_width,
    int step_height, int step_width,
    int target_height, int target_width,
    int channels,
    uint8_t* restrict output)
{
    const float y_scale = (float)input_height / target_height;
    const float x_scale = (float)input_width / target_width;

    // Directly write to output buffer
    uint8_t* out_ptr = output;

    for (int y_out = 0; y_out < target_height; y_out++) {
        const int y_in = (int)(y_out * y_scale + 0.5f);
        const int y_clamped = y_in < 0 ? 0 : (y_in >= input_height ? input_height - 1 : y_in);

        for (int x_out = 0; x_out < target_width; x_out++) {
            const int x_in = (int)(x_out * x_scale + 0.5f);
            const int x_clamped = x_in < 0 ? 0 : (x_in >= input_width ? input_width - 1 : x_in);

            // Calculate source position
            const uint8_t* src = input +
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

#pragma IMAGINET_FRAGMENT_BEGIN "resize_bilinear_uint8"

static inline void resize_bilinear_uint8(const uint8_t* restrict input,
    int input_height, int input_width,
    int step_height, int step_width,
    int target_height, int target_width,
    int channels,
    uint8_t* restrict output)
{
    const float y_scale = (float)input_height / target_height;
    const float x_scale = (float)input_width / target_width;

    // Directly write to output buffer
    uint8_t* out_ptr = output;

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
            const uint8_t* src_00 = input + y0 * step_height + x0 * step_width;
            const uint8_t* src_01 = input + y0 * step_height + x1 * step_width;
            const uint8_t* src_10 = input + y1 * step_height + x0 * step_width;
            const uint8_t* src_11 = input + y1 * step_height + x1 * step_width;

            // Compute bilinear interpolation weights
            const float w00 = (1.0f - x_frac) * (1.0f - y_frac);
            const float w01 = x_frac * (1.0f - y_frac);
            const float w10 = (1.0f - x_frac) * y_frac;
            const float w11 = x_frac * y_frac;

            // Interpolate each channel
            for (int c = 0; c < channels; c++) {
                float result = w00 * src_00[c] + w01 * src_01[c] + w10 * src_10[c] + w11 * src_11[c];
                *out_ptr++ = (uint8_t)(result + 0.5f);
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "targeted_resize_bilinear_f32"
// This function is able to target specific areas in the output buffer
static inline void targeted_resize_bilinear_f32(const float* restrict input,
    int input_height, int input_width,
    int step_height, int step_width,
    int target_height, int target_width,
    int channels,
    float* restrict output,
    int output_step_height,
    int output_step_width)
{
    const float y_scale = (float)input_height / target_height;
    const float x_scale = (float)input_width / target_width;

    for (int y_out = 0; y_out < target_height; y_out++) {
        // Calculate output row position
        float* out_row = output + y_out * output_step_height;

        const float y_src = (y_out + 0.5f) * y_scale - 0.5f;
        const int y_in = (int)y_src;
        const float y_frac = y_src - y_in;

        const int y0 = y_in < 0 ? 0 : (y_in >= input_height ? input_height - 1 : y_in);
        const int y1 = (y_in + 1) >= input_height ? input_height - 1 : (y_in + 1);

        for (int x_out = 0; x_out < target_width; x_out++) {
            // Calculate output pixel position
            float* out_ptr = out_row + x_out * output_step_width;

            const float x_src = (x_out + 0.5f) * x_scale - 0.5f;
            const int x_in = (int)x_src;
            const float x_frac = x_src - x_in;

            const int x0 = x_in < 0 ? 0 : (x_in >= input_width ? input_width - 1 : x_in);
            const int x1 = (x_in + 1) >= input_width ? input_width - 1 : (x_in + 1);

            const float* src_00 = input + y0 * step_height + x0 * step_width;
            const float* src_01 = input + y0 * step_height + x1 * step_width;
            const float* src_10 = input + y1 * step_height + x0 * step_width;
            const float* src_11 = input + y1 * step_height + x1 * step_width;

            const float w00 = (1.0f - x_frac) * (1.0f - y_frac);
            const float w01 = x_frac * (1.0f - y_frac);
            const float w10 = (1.0f - x_frac) * y_frac;
            const float w11 = x_frac * y_frac;

            for (int c = 0; c < channels; c++) {
                out_ptr[c] = w00 * src_00[c] + w01 * src_01[c] + w10 * src_10[c] + w11 * src_11[c];
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "targeted_resize_bilinear_uint8"
// This function is able to target specific areas in the output buffer
static inline void targeted_resize_bilinear_uint8(const uint8_t* restrict input,
    int input_height, int input_width,
    int step_height, int step_width,
    int target_height, int target_width,
    int channels,
    uint8_t* restrict output,
    int output_step_height,
    int output_step_width)
{
    const float y_scale = (float)input_height / target_height;
    const float x_scale = (float)input_width / target_width;

    for (int y_out = 0; y_out < target_height; y_out++) {
        // Calculate output row position
        uint8_t* out_row = output + y_out * output_step_height;

        const float y_src = (y_out + 0.5f) * y_scale - 0.5f;
        const int y_in = (int)y_src;
        const float y_frac = y_src - y_in;

        const int y0 = y_in < 0 ? 0 : (y_in >= input_height ? input_height - 1 : y_in);
        const int y1 = (y_in + 1) >= input_height ? input_height - 1 : (y_in + 1);

        for (int x_out = 0; x_out < target_width; x_out++) {
            // Calculate output pixel position
            uint8_t* out_ptr = out_row + x_out * output_step_width;

            const float x_src = (x_out + 0.5f) * x_scale - 0.5f;
            const int x_in = (int)x_src;
            const float x_frac = x_src - x_in;

            const int x0 = x_in < 0 ? 0 : (x_in >= input_width ? input_width - 1 : x_in);
            const int x1 = (x_in + 1) >= input_width ? input_width - 1 : (x_in + 1);

            const uint8_t* src_00 = input + y0 * step_height + x0 * step_width;
            const uint8_t* src_01 = input + y0 * step_height + x1 * step_width;
            const uint8_t* src_10 = input + y1 * step_height + x0 * step_width;
            const uint8_t* src_11 = input + y1 * step_height + x1 * step_width;

            const float w00 = (1.0f - x_frac) * (1.0f - y_frac);
            const float w01 = x_frac * (1.0f - y_frac);
            const float w10 = (1.0f - x_frac) * y_frac;
            const float w11 = x_frac * y_frac;

            for (int c = 0; c < channels; c++) {
                float result = w00 * src_00[c] + w01 * src_01[c] + w10 * src_10[c] + w11 * src_11[c];
                out_ptr[c] = (uint8_t)(result + 0.5f);
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "resize_letterbox_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "targeted_resize_bilinear_f32"
static inline void resize_letterbox_f32(const float* restrict input,
    int input_height, int input_width,
    int step_height, int step_width,
    int target_height, int target_width,
    int channels,
    float* restrict output)
{
    // scale ratio (new / old)
    const float width_ratio = (float)target_width / input_width;
    const float height_ratio = (float)target_height / input_height;

    float scale_ratio = fminf(width_ratio, height_ratio);

    // compute new dimensions after scaling
    int width_new_unpad = (int)roundf(input_width * scale_ratio);
    int height_new_unpad = (int)roundf(input_height * scale_ratio);

    // compute padding
    int d_width = (target_width - width_new_unpad) / 2;
    int d_height = (target_height - height_new_unpad) / 2;

    // zero out the entire output buffer (0.0f padding)
    memset(output, 0, target_height * target_width * channels * sizeof(float));

    // calculate offset to where resized image should start
    float* output_offset = output + (d_height * target_width + d_width) * channels;

    // resize directly into the target region of the output buffer
    targeted_resize_bilinear_f32(input,
        input_height,
        input_width,
        step_height,
        step_width,
        height_new_unpad,
        width_new_unpad,
        channels,
        output_offset,
        target_width * channels,  
        channels);                 
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "resize_letterbox_uint8"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "targeted_resize_bilinear_uint8"
static inline void resize_letterbox_uint8(const uint8_t* restrict input,
    int input_height, int input_width,
    int step_height, int step_width,
    int target_height, int target_width,
    int channels,
    uint8_t* restrict output)
{
    // scale ratio (new / old)
    const float width_ratio = (float)target_width / input_width;
    const float height_ratio = (float)target_height / input_height;

    float scale_ratio = fminf(width_ratio, height_ratio);

    // compute new dimensions after scaling
    int width_new_unpad = (int)roundf(input_width * scale_ratio);
    int height_new_unpad = (int)roundf(input_height * scale_ratio);

    // compute padding
    int d_width = (target_width - width_new_unpad) / 2;
    int d_height = (target_height - height_new_unpad) / 2;

    // zero out the entire output buffer (0 padding)
    memset(output, 0, target_height * target_width * channels * sizeof(uint8_t));

    // calculate offset to where resized image should start
    uint8_t* output_offset = output + (d_height * target_width + d_width) * channels;

    // resize directly into the target region of the output buffer
    targeted_resize_bilinear_uint8(input,
        input_height,
        input_width,
        step_height,
        step_width,
        height_new_unpad,
        width_new_unpad,
        channels,
        output_offset,
        target_width * channels,  
        channels);                 
}

#pragma IMAGINET_FRAGMENT_END