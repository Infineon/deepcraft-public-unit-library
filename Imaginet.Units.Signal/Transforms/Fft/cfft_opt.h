#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "cdft_ndim_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../../libfft/fftsg_f32.c:cdft"
// input array (any shape >= 2D)
// output array (shape = input.shape)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void cdft_ndim_f32(
    const float* restrict input, 
    float* restrict output, 
    int d0,
    int d1,
    int d2,
    int* restrict temp_ip,
    float* restrict temp_w,
    float* restrict temp_a)
{
    void cdft(int n, int isgn, float* a, int* ip, float* w);

    int d1x2 = 2 * d1;
    int d0_div2 = d0 / 2;
    int d3 = d0 * d1;

    for (int k = 0; k < d2; k++)
    {
        int dk = k * d3;

        // Adjusted loops over i,j to handle complex outer dimension
        for (int i = 0; i < d0; i = i + 2)
        {
            for (int j = 0; j < d1x2; j = j + 2)
            {
                temp_a[j] = input[dk + j * d0_div2 + i];
                temp_a[j + 1] = input[dk + j * d0_div2 + 1 + i];
            }

            cdft(d1x2, -1, temp_a, temp_ip, temp_w);

            for (int j = 0; j < d1x2; j = j + 2)
            {
                output[dk + j * d0_div2 + i] = temp_a[j];
                output[dk + j * d0_div2 + 1 + i] = temp_a[j + 1];
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END
