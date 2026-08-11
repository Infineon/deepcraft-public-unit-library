#pragma IMAGINET_INCLUDES_BEGIN
#include "arm_math.h"
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_preproc_types.h:gesture_preproc_types"

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_window_tables"

static const ifx_f32_t GESTURE_HANN_S64[64] = {
    0.f,          0.0024846124f, 0.0099137565f, 0.022213597f, 0.039261892f,
    0.060889214f, 0.08688061f,   0.11697778f,   0.15088159f,  0.1882551f,
    0.22872686f,  0.27189466f,   0.3173295f,    0.36457977f,  0.4131759f,
    0.46263495f,  0.51246536f,   0.5621719f,    0.6112605f,   0.65924335f,
    0.70564353f,  0.75f,         0.79187185f,   0.8308429f,   0.86652595f,
    0.89856625f,  0.92664546f,   0.95048445f,   0.9698463f,   0.9845386f,
    0.9944154f,   0.99937844f,   0.99937844f,   0.9944154f,   0.9845386f,
    0.9698463f,   0.95048445f,   0.92664546f,   0.89856625f,  0.86652595f,
    0.8308429f,   0.79187185f,   0.75f,         0.70564353f,  0.65924335f,
    0.6112605f,   0.5621719f,    0.51246536f,   0.46263495f,  0.4131759f,
    0.36457977f,  0.3173295f,    0.27189466f,   0.22872686f,  0.1882551f,
    0.15088159f,  0.11697778f,   0.08688061f,   0.060889214f, 0.039261892f,
    0.022213597f, 0.0099137565f, 0.0024846124f, 0.f
};

static const ifx_f32_t GESTURE_KAISER_B25_S32[32] = {
    1.73173351e-10f, 1.61998003e-07f, 4.30531964e-06f, 4.76741225e-05f,
    3.23684915e-04f, 1.57319242e-03f, 5.93495090e-03f, 1.82437934e-02f,
    4.71659079e-02f, 1.04815245e-01f, 2.03354374e-01f, 3.48355740e-01f,
    5.31276107e-01f, 7.25665569e-01f, 8.91398251e-01f, 9.87333238e-01f,
    9.87333238e-01f, 8.91398251e-01f, 7.25665569e-01f, 5.31276107e-01f,
    3.48355740e-01f, 2.03354374e-01f, 1.04815245e-01f, 4.71659079e-02f,
    1.82437934e-02f, 5.93495090e-03f, 1.57319242e-03f, 3.23684915e-04f,
    4.76741225e-05f, 4.30531964e-06f, 1.61998003e-07f, 1.73173351e-10f
};

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_window_tables"

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_copy_normalized_window"
static inline void gesture_copy_normalized_window(
    const ifx_f32_t* restrict window, ifx_f32_t* restrict out, uint16_t size)
{
    ifx_f32_t sum = 0.f;
    for (uint16_t i = 0; i < size; ++i) {
        sum += window[i];
    }
    arm_scale_f32(
        (float32_t*)window, 1.0f / (float32_t)sum, (float32_t*)out, size);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_init_windows"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_copy_normalized_window"
static inline void gesture_init_windows(preproc_work_arrays_t* arr)
{
    gesture_copy_normalized_window(
        GESTURE_HANN_S64, arr->range_window, arr->cfg.n_samples);

    if (arr->cfg.n_chirps >= 16) {
        gesture_copy_normalized_window(
            GESTURE_KAISER_B25_S32, arr->doppler_window, arr->cfg.n_chirps);
    }
}
#pragma IMAGINET_FRAGMENT_END
