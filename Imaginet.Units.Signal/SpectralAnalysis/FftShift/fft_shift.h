#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END


// input array (any shape >= 2D)
// output array (shape = input.shape)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
#pragma IMAGINET_FRAGMENT_BEGIN "fft_shift_f32"
static inline void fft_shift_f32(
    const float* restrict input, 
    float* restrict output,
    int d0, int d1, int d2)
{
    int d3 = d0 * d1;
    int d1_div2 = d1 / 2;

    for (int k = 0; k < d2; k++)
    {
        for (int i = 0; i < d0; i++)
        {                	           
            for (int j = 0; j < d1_div2; j++)
            {
                // Shift assuming d1 is even
                int index0 = j * d0 + i;
                int index1 = index0 + d1_div2 * d0;
                output[index0] = input[index1];
                output[index1] = input[index0];
            }
        }
        output += d3;
        input += d3;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "fft_shift_q31"
static inline void fft_shift_q31(
    const q31_t* restrict input,
    q31_t* restrict output,
    int d0, int d1, int d2)
{
    int d3 = d0 * d1;
    int d1_div2 = d1 / 2;

    for (int k = 0; k < d2; k++)
    {
        for (int i = 0; i < d0; i++)
        {
            for (int j = 0; j < d1_div2; j++)
            {
                // Shift assuming d1 is even
                int index0 = j * d0 + i;
                int index1 = index0 + d1_div2 * d0;
                output[index0] = input[index1];
                output[index1] = input[index0];
            }
        }
        output += d3;
        input += d3;
    }
}
#pragma IMAGINET_FRAGMENT_END


#pragma IMAGINET_FRAGMENT_BEGIN "fft_shift_q15"
static inline void fft_shift_q15(
    const q15_t* restrict input,
    q15_t* restrict output,
    int d0, int d1, int d2)
{
    int d3 = d0 * d1;
    int d1_div2 = d1 / 2;

    for (int k = 0; k < d2; k++)
    {
        for (int i = 0; i < d0; i++)
        {
            for (int j = 0; j < d1_div2; j++)
            {
                // Shift assuming d1 is even
                int index0 = j * d0 + i;
                int index1 = index0 + d1_div2 * d0;
                output[index0] = input[index1];
                output[index1] = input[index0];
            }
        }
        output += d3;
        input += d3;
    }
}
#pragma IMAGINET_FRAGMENT_END
