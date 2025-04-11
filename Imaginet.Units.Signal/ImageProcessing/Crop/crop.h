#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "crop_f32"

static inline void crop_f32(const float* restrict input, float* restrict output,
    int start_width, int count_width,
    int start_height, int count_height,
    int input_width, int input_height,
    int input_channels)
{
    for (int h = 0; h < count_height; h++) {
        for (int w = 0; w < count_width; w++) {
            for (int c = 0; c < input_channels; c++) {
                output[(h * count_width + w) * input_channels + c] = input[((h + start_height) * input_width + (w + start_width)) * input_channels + c];
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END