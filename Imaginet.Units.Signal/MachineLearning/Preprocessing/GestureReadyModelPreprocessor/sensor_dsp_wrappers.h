#pragma IMAGINET_INCLUDES_BEGIN
#include <assert.h>
#include <complex.h>
#include <stdbool.h>
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "ifx_sensor_dsp_types"
#ifndef IFX_SENSOR_DSP_STATUS_OK
#define IFX_SENSOR_DSP_STATUS_OK        (0)
#endif
#ifndef IFX_SENSOR_DSP_ARGUMENT_ERROR
#define IFX_SENSOR_DSP_ARGUMENT_ERROR   (1)
#endif
#ifndef CIMAG_F32
#define CIMAG_F32(x)   (((float32_t *)&(x))[1])
#endif
#ifndef CFLOAT32_T_DEFINED
#define CFLOAT32_T_DEFINED
typedef _Complex float cfloat32_t;
#endif
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "ifx_mean_removal_f32"
static inline void ifx_mean_removal_f32(float32_t* v, uint32_t len)
{
    assert(v != NULL);

    float32_t mean;
    arm_mean_f32(v, len, &mean);
    arm_offset_f32(v, -mean, v, len);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "ifx_cmplx_mean_removal_f32"
static inline void ifx_cmplx_mean_removal_f32(cfloat32_t* v, uint32_t len)
{
    assert(v != NULL);

    cfloat32_t sum = 0.0f;
    cfloat32_t* p_src = v;
    uint32_t cnt = len;
    while (cnt > 0U) {
        sum += *p_src++;
        cnt--;
    }

    sum = sum / (float32_t)len;

    cnt = len;
    p_src = v;
    while (cnt > 0U) {
        *p_src++ -= sum;
        cnt--;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "ifx_range_fft_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "sensor_dsp_wrappers.h:ifx_sensor_dsp_types"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "ifx_mean_removal_f32"
static inline int32_t ifx_range_fft_f32(
    float32_t* frame,
    cfloat32_t* range,
    bool mean_removal,
    const float32_t* win,
    uint16_t num_samples_per_chirp,
    uint16_t num_chirps_per_frame)
{
    assert(frame != NULL);
    assert(range != NULL);

    static arm_rfft_fast_instance_f32 rfft = { 0 };
    if (rfft.fftLenRFFT != num_samples_per_chirp) {
        if (arm_rfft_fast_init_f32(&rfft, num_samples_per_chirp) != ARM_MATH_SUCCESS) {
            return IFX_SENSOR_DSP_ARGUMENT_ERROR;
        }
    }

    for (uint32_t chirp_idx = 0; chirp_idx < num_chirps_per_frame; ++chirp_idx) {
        if (mean_removal) {
            ifx_mean_removal_f32(frame, num_samples_per_chirp);
        }

        if (win != NULL) {
            arm_mult_f32(frame, win, frame, num_samples_per_chirp);
        }

        arm_rfft_fast_f32(&rfft, frame, (float32_t*)range, 0);
        CIMAG_F32(range[0]) = 0.0f;

        frame += num_samples_per_chirp;
        range += (num_samples_per_chirp / 2U);
    }

    return IFX_SENSOR_DSP_STATUS_OK;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "ifx_doppler_cfft_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "sensor_dsp_wrappers.h:ifx_sensor_dsp_types"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "ifx_cmplx_mean_removal_f32"
static inline int32_t ifx_doppler_cfft_f32(
    cfloat32_t* range,
    cfloat32_t* doppler,
    bool mean_removal,
    const float32_t* win,
    uint16_t num_range_bins,
    uint16_t num_chirps_per_frame)
{
    assert(range != NULL);
    assert(doppler != NULL);

    static arm_cfft_instance_f32 cfft = { 0 };
    if (cfft.fftLen != num_chirps_per_frame) {
        if (arm_cfft_init_f32(&cfft, num_chirps_per_frame) != ARM_MATH_SUCCESS) {
            return IFX_SENSOR_DSP_ARGUMENT_ERROR;
        }
    }

    arm_matrix_instance_f32 range_matrix = {
        num_chirps_per_frame,
        num_range_bins,
        (float32_t*)range
    };
    arm_matrix_instance_f32 doppler_matrix = {
        num_range_bins,
        num_chirps_per_frame,
        (float32_t*)doppler
    };

    (void)arm_mat_cmplx_trans_f32(&range_matrix, &doppler_matrix);

    for (uint32_t range_idx = 0; range_idx < num_range_bins; ++range_idx) {
        if (mean_removal) {
            ifx_cmplx_mean_removal_f32(doppler, num_chirps_per_frame);
        }

        if (win != NULL) {
            arm_cmplx_mult_real_f32(
                (float32_t*)doppler, win, (float32_t*)doppler, num_chirps_per_frame);
        }

        arm_cfft_f32(&cfft, (float32_t*)doppler, 0, 1);

        doppler += num_chirps_per_frame;
    }

    return IFX_SENSOR_DSP_STATUS_OK;
}
#pragma IMAGINET_FRAGMENT_END
