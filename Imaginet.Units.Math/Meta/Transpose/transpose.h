#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "transpose_f32"

static inline void transpose_f32(const float* restrict input, float* restrict output, int rows, int cols)
{
    int count = rows * cols;
    for (int i = 0; i < count; ++i)
    {
        int original_row = i / cols;
        int original_col = i % cols;
        int transposed_index = original_col * rows + original_row;
        output[transposed_index] = input[i];
    }
}

#pragma IMAGINET_FRAGMENT_END