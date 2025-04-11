#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "dct_naive_f32"

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

// input array (any shape)
// output array (shape = input.shape)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void dct_naive_f32(
    const float* restrict input,
    float* restrict output,
    int output_size,
    int d0, int d1, int d2)
{
    float factor = M_PI / d1;
    int d3 = d0 * d1;

    for (int k = 0; k < d2; k++)
    {
        int dk = k * d3;
        for (int i = 0; i < d0; i++)
        {
        	// The actual DCT start here!
            for (int j0 = 0; j0 < output_size; j0++)
            {
                float sum = 0;
                for (int j1 = 0; j1 < d1; j1++)
                {
                    sum += input[dk + j1 * d0 + i] * cosf((j1 + 0.5) * j0 * factor) * 2;
                }
            	
                output[dk + j0 * d0 + i] = sum;
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END