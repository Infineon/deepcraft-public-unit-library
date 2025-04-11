#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "dct_ndim_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../../libfft/fftsg_f32.c:ddct"
// input array (any shape)
// output array (shape = input.shape)
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void dct_ndim_f32(
    const float* restrict input, 
    float* restrict output,
    int output_size,
    int d0, int d1, int d2,
    int* restrict temp_ip, float* restrict temp_w, float* restrict temp_a)
{
    void ddct(int n, int isgn, float* a, int* ip, float* w);

    int d3 = d0 * d1;
  
    for (int k = 0; k < d2; k++)
    {
        int dk = k * d3;
       
        for (int i = 0; i < d0; i++)
        {                	           
            for (int j = 0; j < d1; j++)
            {
                temp_a[j] = input[dk + j * d0 + i];               
            }
        	
            ddct(d1, -1, temp_a, temp_ip, temp_w);

            for (int j = 0; j < d1; j++)
            {
                output[dk + j * d0 + i] = temp_a[j] * 2;
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END

