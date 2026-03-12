#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "irfft_libfft_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../../libfft/fftsg_f32.c:rdft"
// input array  (shape = output.shape.replace(axis, n_bins).insert(0,2))
// output array (any shape >= 1D, real-valued)
// d0 = output.shape.step(axis)   = input.shape.step(axis+1) / 2
// d1 = output.shape.size(axis)   = N
// d2 = output.shape.slot(axis)   = input.shape.slot(axis+1)
static inline void irfft_libfft_f32(
    const float* restrict input,
    float* restrict output,
    int d0, int d1, int d2,
    int32_t* restrict temp_ip, float* restrict temp_w, float* restrict temp_a)
{
    void rdft(int n, int isgn, float* a, int* ip, float* w);

    // step between adjacent complex bins in the input spectrum
    int step = d0 * 2;
    // input batch stride = n_bins * step = (d1/2+1) * 2*d0 = (d1+2)*d0
    int a_in = (d1 + 2) * d0;
    // output batch stride
    int a_out = d0 * d1;

    for (int k = 0; k < d2; k++)
    {
        int off_in  = k * a_in;
        int off_out = k * a_out;

        for (int i = 0; i < d0; i++)
        {
            // base index into the input spectrum for this lane i
            int base = off_in + 2 * i;

            // Pack into ooura rdft format:
            //   temp_a[0]     = R[0]   = Re[0]
            //   temp_a[1]     = R[N/2] = Re[N/2]  (Nyquist, stored at bin index d1/2)
            //   temp_a[2k]    = R[k]   = Re[k]     for k = 1..N/2-1
            //   temp_a[2k+1]  = I[k]   = -Im[k]   (ooura sign convention: I[k] = -Im(X[k]))
            temp_a[0] = input[base];
            temp_a[1] = input[base + (d1 / 2) * step];

            for (int m = 2; m < d1; m += 2)
            {
                int idx = base + (m / 2) * step;
                temp_a[m]     =  input[idx];
                temp_a[m + 1] = -input[idx + 1];
            }

            rdft(d1, -1, temp_a, (int *)temp_ip, temp_w);

            // ooura IRDFT result = (N/2) * irfft; scale by 2/N to match numpy normalization
            float scale = 2.0f / d1;
            int base_out = off_out + i;
            for (int j = 0; j < d1; j++)
            {
                output[base_out + j * d0] = temp_a[j] * scale;
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END
