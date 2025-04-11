#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#include <assert.h>
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "[Imaginet.Units.Base.Error]/error.h:print_error"

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_check_static_size_q15"
static_assert(sizeof(arm_rfft_instance_q15) <= 48, "Data structure 'arm_rfft_instance_q15' is too big");
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_32_q15"
static inline int rfft_cmsis_init_32_q15(void* handle)
{
    if (arm_rfft_init_32_q15((arm_rfft_instance_q15*)handle, 0, 1) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_init_32_q15");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_64_q15"
static inline int rfft_cmsis_init_64_q15(void* handle)
{
    if (arm_rfft_init_64_q15((arm_rfft_instance_q15*)handle, 0, 1) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_init_64_q15");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_128_q15"
static inline int rfft_cmsis_init_128_q15(void* handle)
{
    if (arm_rfft_init_128_q15((arm_rfft_instance_q15*)handle, 0, 1) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_init_128_q15");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_256_q15"
static inline int rfft_cmsis_init_256_q15(void* handle)
{
    if (arm_rfft_init_256_q15((arm_rfft_instance_q15*)handle, 0, 1) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_init_256_q15");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_512_q15"
static inline int rfft_cmsis_init_512_q15(void* handle)
{
    if (arm_rfft_init_512_q15((arm_rfft_instance_q15*)handle, 0, 1) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_init_512_q15");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_1024_q15"
static inline int rfft_cmsis_init_1024_q15(void* handle)
{
    if (arm_rfft_init_1024_q15((arm_rfft_instance_q15*)handle, 0, 1) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_init_1024_q15");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_2048_q15"
static inline int rfft_cmsis_init_2048_q15(void* handle)
{
    if (arm_rfft_init_2048_q15((arm_rfft_instance_q15*)handle, 0, 1) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_init_2048_q15");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_4096_q15"
static inline int rfft_cmsis_init_4096_q15(void* handle)
{
    if (arm_rfft_init_4096_q15((arm_rfft_instance_q15*)handle, 0, 1) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_init_4096_q15");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END



#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_q15"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "rfft_cmsis_check_static_size_q15"
// input array (any shape >= 1D)
// output array (shape = input.shape.replace(axis, n).insert(0,2))
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void rfft_cmsis_q15(
    void* handle,
    const q15_t* restrict input,
    q15_t* restrict output,
    int d0, int d1, int d2, q15_t* restrict temp_a, q15_t* restrict temp_b)
{
    int step = d0 * 2;
    int a0 = d0 * d1;
    int a1 = a0 + step;

    for (int k = 0; k < d2; k++)
    {
        int m0 = k * a0;
        int m1 = k * a1;
        for (int i = 0; i < d0; i++)
        {
            int as = m0 + i;
            for (int j = 0; j < d1; j++)
            {
                temp_a[j] = input[as];
                as += d0;
            }

            arm_rfft_q15((arm_rfft_instance_q15*)handle, temp_a, temp_b);

            int index = i * 2 + m1;     
            q15_t* tp = temp_b;
            for (int j = 0; j < d1; j += 2)
            {      
                output[index] = *tp++;
                output[index + 1] = *tp++;
                index += step;
            }
            output[index] = *tp++;
            output[index + 1] = *tp++;
        }
    }
}
#pragma IMAGINET_FRAGMENT_END


// ---- Basic d0=1 d2=1 ----

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_d0_d2_q15"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "rfft_cmsis_check_static_size_q15"
static inline void rfft_cmsis_d0_d2_q15(
    void* handle,
    const q15_t* restrict input,
    q15_t* restrict output,
    int d1, q15_t* restrict temp_a, q15_t* restrict temp_b)
{
    for (int i = 0; i < d1; i++) 
    {
        temp_a[i] = input[i];
    }

    // The temp_a buffer is modified by this function.
    // The temp_b buffer must have length 2N.
    arm_rfft_q15((arm_rfft_instance_q15*)handle, temp_a, temp_b);

    for (int i = 0; i < d1; i+=2) {
        *output++ = *temp_b++;
        *output++ = *temp_b++;
    }
    *output++ = *temp_b++;
    *output++ = *temp_b++;
}
#pragma IMAGINET_FRAGMENT_END

