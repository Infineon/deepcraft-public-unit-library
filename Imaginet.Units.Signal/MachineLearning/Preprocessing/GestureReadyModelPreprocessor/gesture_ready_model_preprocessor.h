#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_preproc_types.h:gesture_preproc_types"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_preproc_types.h:preproc_work_arrays_bind"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_windows.h:gesture_init_windows"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "slim_algo.h:slim_algo_f32"

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_preproc_init_f32"
static inline int gesture_preproc_init_f32(
    int8_t* restrict handle,
    int n_channels,
    int n_chirps,
    int n_samples,
    int n_range_bins,
    int min_range_bin)
{
    preproc_work_arrays_t* arr = (preproc_work_arrays_t*)handle;
    arr->cfg.n_channels = (uint16_t)n_channels;
    arr->cfg.n_chirps = (uint16_t)n_chirps;
    arr->cfg.n_samples = (uint16_t)n_samples;
    arr->cfg.n_range_bins = (uint16_t)n_range_bins;

    const int profile_len = n_range_bins - min_range_bin;
    preproc_work_arrays_bind(arr, handle, profile_len + 8);
    gesture_init_windows(arr);
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_deinterleave_f32"
/* Interleaved FIFO / Imagimob [chirps, samples, channels] layout: these are
 * the same flat element ordering -- antennas interleaved per (chirp, sample),
 * as read directly from the BGT60 SPI FIFO, and as streamed by DEEPCRAFT
 * Studio / Imagimob device recording. */
static inline void gesture_deinterleave_f32(
    const float* restrict input, float* restrict out, const frame_cfg_t* cfg)
{
    const int n_ch = cfg->n_channels;
    const int n_c  = cfg->n_chirps;
    const int n_s  = cfg->n_samples;

    for (int ch = 0; ch < n_ch; ch++) {
        for (int c = 0; c < n_c; c++) {
            for (int s = 0; s < n_s; s++) {
                out[ch * n_c * n_s + c * n_s + s] =
                    input[c * n_s * n_ch + s * n_ch + ch];
            }
        }
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_copy_channel_major_f32"
/* Channel-major layout: [channels, chirps, samples], already de-interleaved. */
static inline void gesture_copy_channel_major_f32(
    const float* restrict input, float* restrict out, const frame_cfg_t* cfg)
{
    const size_t count = (size_t)cfg->n_channels * cfg->n_chirps * cfg->n_samples;
    memcpy(out, input, count * sizeof(float));
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_deinterleave_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_copy_channel_major_f32"

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_prepare_input_f32"
static inline void gesture_prepare_input_f32(
    const float* restrict input, float* restrict out, const frame_cfg_t* cfg,
    int input_layout)
{
    switch (input_layout) {
        case 1:
            gesture_copy_channel_major_f32(input, out, cfg);
            break;
        case 0:
        default:
            gesture_deinterleave_f32(input, out, cfg);
            break;
    }
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_prepare_input_f32"

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_apply_norm_f32"
/* Training-derived z-score normalization. */
static inline void gesture_apply_norm_f32(
    const slim_algo_output_t* restrict res, float* restrict output)
{
    static const float norm_mean[5] = {
        9.26814552650607f, 4.391583164927378f, 0.27332462978312866f,
        -0.02838213175529301f, 0.00026668613549266876f
    };
    static const float norm_scale[5] = {
        5.801363069954616f, 7.547439540930497f, 0.5629401789624862f,
        0.41502512890635995f, 0.0007474111364241666f
    };

    output[0] = ((float)res->detection.range_bin - norm_mean[0]) / norm_scale[0];
    output[1] = ((float)res->detection.doppler_bin - norm_mean[1]) / norm_scale[1];
    output[2] = (res->detection.azimuth - norm_mean[2]) / norm_scale[2];
    output[3] = (res->detection.elevation - norm_mean[3]) / norm_scale[3];
    output[4] = (res->detection.value - norm_mean[4]) / norm_scale[4];
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_apply_norm_f32"

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_preproc_f32"
static inline void gesture_preproc_f32(
    const float* restrict input,
    float* restrict output,
    int8_t* restrict handle,
    int min_range_bin,
    float* restrict input_scratch,
    int input_layout,
    int apply_adc_normalization)
{
    preproc_work_arrays_t* arr = (preproc_work_arrays_t*)handle;

    gesture_prepare_input_f32(input, input_scratch, &arr->cfg, input_layout);

    slim_algo_output_t res = { 0 };
    (void)slim_algo_f32(
        &res, (ifx_f32_t*)input_scratch, &arr->cfg, (uint16_t)min_range_bin, arr,
        apply_adc_normalization);

    gesture_apply_norm_f32(&res, output);
}
#pragma IMAGINET_FRAGMENT_END

