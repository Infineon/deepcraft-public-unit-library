#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#include <assert.h>
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "[Imaginet.Units.Base.Error]/error.h:print_error"

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_check_static_size_f32"
static_assert (sizeof(arm_rfft_fast_instance_f32) <= 48, "Data structure 'arm_rfft_fast_instance_f32' is too big");
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_32_f32"
static inline int rfft_cmsis_init_32_f32(void* handle)
{
    if (arm_rfft_fast_init_32_f32((arm_rfft_fast_instance_f32*)handle) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_fast_init_32_f32");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_64_f32"
static inline int rfft_cmsis_init_64_f32(void* handle)
{
    if (arm_rfft_fast_init_64_f32((arm_rfft_fast_instance_f32*)handle) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_fast_init_64_f32");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_128_f32"
static inline int rfft_cmsis_init_128_f32(void* handle)
{
    if (arm_rfft_fast_init_128_f32((arm_rfft_fast_instance_f32*)handle) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_fast_init_128_f32");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_256_f32"
static inline int rfft_cmsis_init_256_f32(void* handle)
{
    if (arm_rfft_fast_init_256_f32((arm_rfft_fast_instance_f32*)handle) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_fast_init_256_f32");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_512_f32"
static inline int rfft_cmsis_init_512_f32(void* handle)
{
    if (arm_rfft_fast_init_512_f32((arm_rfft_fast_instance_f32*)handle) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_fast_init_512_f32");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_1024_f32"
static inline int rfft_cmsis_init_1024_f32(void* handle)
{
    if (arm_rfft_fast_init_1024_f32((arm_rfft_fast_instance_f32*)handle) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_fast_init_1024_f32");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_2048_f32"
static inline int rfft_cmsis_init_2048_f32(void* handle)
{
    if (arm_rfft_fast_init_2048_f32((arm_rfft_fast_instance_f32*)handle) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_fast_init_2048_f32");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_init_4096_f32"
static inline int rfft_cmsis_init_4096_f32(void* handle)
{
    if (arm_rfft_fast_init_4096_f32((arm_rfft_fast_instance_f32*)handle) != ARM_MATH_SUCCESS) {
        print_error("[FAILED] arm_rfft_fast_init_4096_f32");
        return -2;
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END



#pragma IMAGINET_FRAGMENT_BEGIN "rfft_cmsis_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "rfft_cmsis_check_static_size_f32"
// input array (any shape >= 1D)
// output array (shape = input.shape.replace(axis, n).insert(0,2))
// d0 = input.shape.step(axis)
// d1 = input.shape.size(axis)
// d2 = input.shape.slot(axis)
static inline void rfft_cmsis_f32(
    void* handle,
    const float* restrict input,
    float* restrict output,
    int d0, int d1, int d2, float* restrict temp_a, float* restrict temp_b)
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

            arm_rfft_fast_f32((arm_rfft_fast_instance_f32*)handle, temp_a, temp_b, 0);
         
            int index =  i * 2 + m1;
            output[index] = temp_b[0];
            output[index + 1] = 0;
            index += step;
            for (int j = 2; j < d1; j += 2)
            {
                output[index] = temp_b[j];
                output[index + 1] = temp_b[j + 1];
                index += step;
            }
            output[index] = temp_b[1];
            output[index + 1] = 0;
        }
    }
}

#pragma IMAGINET_FRAGMENT_END

