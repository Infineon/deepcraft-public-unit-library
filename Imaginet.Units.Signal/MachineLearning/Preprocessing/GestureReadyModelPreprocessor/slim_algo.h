#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#include <stdint.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "slim_algo_helpers"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_preproc_types.h:gesture_preproc_types"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../../../sensor_dsp/source/ifx_range_fft_f32.c:ifx_range_fft_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../../../sensor_dsp/source/ifx_doppler_cfft_f32.c:ifx_doppler_cfft_f32"
#ifndef GESTURE_PREPROC_PI
#define GESTURE_PREPROC_PI 3.14159265358979323846f
#endif

#define GESTURE_ADC_RESOLUTION (12ul)
#define GESTURE_ADC_NORMALIZATION ((1 << GESTURE_ADC_RESOLUTION) - 1)
#define GESTURE_FREQ_CENTER (60000000000.0f)
#define GESTURE_ANTENNA_DISTANCE (0.0025f)
#define GESTURE_C0 (299792458.0f)

#define GESTURE_PREPROC_MAX(a, b) (((a) >= (b)) ? (a) : (b))

typedef struct {
    uint16_t n_chirps;
    uint16_t n_samples;
    bool remove_mean;
    ifx_f32_t* window;
} range_transform_cfg_t;

static inline void range_transform(
    ifx_f32_t* x, ifx_cf64_t* out, range_transform_cfg_t* cfg)
{
    uint32_t status = ifx_range_fft_f32(
        (float32_t*)x, (cfloat32_t*)out, cfg->remove_mean, (float32_t*)cfg->window,
        cfg->n_samples, cfg->n_chirps);
    if (status == IFX_SENSOR_DSP_ARGUMENT_ERROR) {
        return;
    }

    for (int chirp = 0; chirp < cfg->n_chirps; chirp++) {
        ((ifx_cf64_t*)out + (chirp * cfg->n_samples / 2))->data[1] = 0.0f;
    }
}

static inline void build_complex_range_image(
    ifx_f32_t* raw_frame, ifx_cf64_t* out, frame_cfg_t* f_cfg, ifx_f32_t* window,
    int apply_adc_normalization)
{
    uint16_t src_idx = 0;
    uint16_t dst_idx = 0;
    range_transform_cfg_t range_transf_cfg = {
        .n_chirps = f_cfg->n_chirps,
        .n_samples = f_cfg->n_samples,
        .remove_mean = true,
        .window = window
    };

    if (apply_adc_normalization) {
        uint16_t frame_size = f_cfg->n_channels * f_cfg->n_chirps * f_cfg->n_samples;
        arm_scale_f32(
            (float32_t*)raw_frame, 1.0f / (float32_t)GESTURE_ADC_NORMALIZATION,
            (float32_t*)raw_frame, frame_size);
    }

    for (int ch = 0; ch < f_cfg->n_channels; ++ch) {
        range_transform(raw_frame + src_idx, out + dst_idx, &range_transf_cfg);
        src_idx += f_cfg->n_chirps * f_cfg->n_samples;
        dst_idx += f_cfg->n_chirps * f_cfg->n_range_bins;
    }
}

static inline void remove_mean_cf64(cfloat32_t* src, uint16_t n_el, uint16_t step_size)
{
    cfloat32_t sum = 0.0f;
    for (int i = 0; i < n_el; ++i) {
        sum += src[i * step_size];
    }
    for (int i = 0; i < n_el; ++i) {
        src[i * step_size] -= sum / n_el;
    }
}

static inline void remove_mean_3d_cf64(
    ifx_cf64_t* src, uint16_t axis, uint16_t n_ch, uint16_t n_rows, uint16_t n_cols)
{
    cfloat32_t* arr = (cfloat32_t*)src;
    if (axis == 0) {
        for (int el = 0; el < n_rows * n_cols; ++el) {
            remove_mean_cf64(arr + el, n_ch, n_rows * n_cols);
        }
    } else if (axis == 1) {
        for (int ch = 0; ch < n_ch; ++ch) {
            for (int col = 0; col < n_cols; ++col) {
                remove_mean_cf64(arr + ch * n_rows * n_cols + col, n_rows, n_cols);
            }
        }
    } else if (axis == 2) {
        for (int ch = 0; ch < n_ch; ++ch) {
            for (int row = 0; row < n_rows; ++row) {
                remove_mean_cf64(arr + ch * n_rows * n_cols + row * n_cols, n_cols, 1);
            }
        }
    }
}

static inline void mean_rdi_channel_f32(
    ifx_f32_t* abs_rdi, ifx_f32_t* mean, frame_cfg_t* f_cfg)
{
    uint16_t len = f_cfg->n_chirps * f_cfg->n_range_bins;
    for (int i = 0; i < len; ++i) {
        mean[i] = 0.0f;
        for (int ch = 0; ch < f_cfg->n_channels; ++ch) {
            mean[i] += abs_rdi[ch * len + i];
        }
    }
    arm_scale_f32(
        (float32_t*)mean, 1.0f / f_cfg->n_channels, (float32_t*)mean, len);
}

static inline void slice_3d_col_cf64(
    ifx_cf64_t* src, ifx_cf64_t* dst, uint16_t col, uint16_t n_ch,
    uint16_t n_rows, uint16_t n_cols)
{
    for (uint16_t ch = 0; ch < n_ch; ++ch) {
        for (uint16_t row = 0; row < n_rows; ++row) {
            *dst = src[ch * n_rows * n_cols + row * n_cols + col];
            ++dst;
        }
    }
}

static inline uint32_t filter_range_profile(
    ifx_f32_t* range_profile, int32_t len, uint32_t peak_range, ifx_f32_t* conv_out)
{
    ifx_f32_t threshold;
    const float32_t weights[] = {
        1.33830625e-04f, 4.43186162e-03f, 5.39911274e-02f, 2.41971446e-01f,
        3.98943469e-01f, 2.41971446e-01f, 5.39911274e-02f, 4.43186162e-03f,
        1.33830625e-04f
    };
    threshold = GESTURE_PREPROC_MAX(0.1f * range_profile[peak_range], 1e-4f);

    arm_conv_f32(range_profile, (uint32_t)len, weights, 9, conv_out);

    float32_t* p_conv_out = (float32_t*)&conv_out[4];

    for (int i = 0; i < len; i++) {
        if (p_conv_out[i] < threshold) {
            p_conv_out[i] = 0.0f;
        }
    }

    int32_t peak_idx = -1;
    for (int32_t i = 1; i < len - 1; i++) {
        if ((p_conv_out[i - 1] < p_conv_out[i]) && (p_conv_out[i + 1] < p_conv_out[i])) {
            peak_idx = i;
            break;
        }
    }

    if (peak_idx > -1) {
        return (uint32_t)peak_idx;
    }
    return peak_range;
}

static inline int angle(ifx_f32_t re, ifx_f32_t im, float* out)
{
    float32_t tmp_out;
    arm_status status = arm_atan2_f32((float32_t)im, (float32_t)re, &tmp_out);
    *out = (float)tmp_out;
    return (int)status;
}

static inline float deg2rad(float deg)
{
    return deg * GESTURE_PREPROC_PI / 180.0f;
}

static inline float get_phase_difference(float phase0, float phase1)
{
    float d_phase = phase1 - phase0;
    float n_periods = rintf(d_phase / (2.f * GESTURE_PREPROC_PI));
    phase1 -= n_periods * 2.f * GESTURE_PREPROC_PI;
    d_phase = phase1 - phase0;
    return d_phase;
}

static inline float phase_monopulse(float phase0, float phase1)
{
    float d_phase = get_phase_difference(phase0, phase1);
    return asinf(GESTURE_C0 * d_phase / (2.f * GESTURE_PREPROC_PI * GESTURE_FREQ_CENTER * GESTURE_ANTENNA_DISTANCE));
}

static inline void fftshift_cf64(ifx_cf64_t* in, uint32_t len)
{
    int half = (int)len / 2;
    ifx_cf64_t tmp;
    for (int i = 0; i < half; ++i) {
        tmp = in[i];
        in[i] = in[i + half];
        in[i + half] = tmp;
    }
}

static inline void get_range_profile(
    ifx_cf64_t* x_range, preproc_work_arrays_t* arr, frame_cfg_t* f_cfg,
    uint16_t min_range_bin)
{
    uint16_t size = f_cfg->n_channels * f_cfg->n_chirps * f_cfg->n_range_bins;
    arm_cmplx_mag_f32((float32_t*)x_range, (float32_t*)arr->x_range_abs, size);
    mean_rdi_channel_f32(arr->x_range_abs, arr->x_range_abs_mean, f_cfg);
    for (int idx_rb = min_range_bin; idx_rb < f_cfg->n_range_bins; ++idx_rb) {
        ifx_f32_t sum = 0.f;
        for (int idx_chirp = 1; idx_chirp < f_cfg->n_chirps; ++idx_chirp) {
            sum += arr->x_range_abs_mean[(idx_chirp * f_cfg->n_range_bins) + idx_rb];
        }
        arr->range_profile[idx_rb - min_range_bin] = sum / (f_cfg->n_chirps - 1);
    }
}

static inline void get_single_range_bin_doppler(
    ifx_cf64_t* x_range, preproc_work_arrays_t* arr, uint32_t range_bin,
    frame_cfg_t* f_cfg)
{
    slice_3d_col_cf64(
        x_range, arr->x_range_slice, (uint16_t)range_bin, f_cfg->n_channels,
        f_cfg->n_chirps, f_cfg->n_range_bins);
    for (uint16_t idx_ch = 0; idx_ch < f_cfg->n_channels; ++idx_ch) {
        (void)ifx_doppler_cfft_f32(
            (cfloat32_t*)(arr->x_range_slice + idx_ch * f_cfg->n_chirps),
            (cfloat32_t*)(arr->x_doppler + idx_ch * f_cfg->n_chirps), false,
            arr->doppler_window, 1, f_cfg->n_chirps);
        fftshift_cf64(arr->x_doppler + idx_ch * f_cfg->n_chirps, f_cfg->n_chirps);
    }
}

static inline void get_doppler_profile(
    ifx_cf64_t* x_doppler, preproc_work_arrays_t* arr, frame_cfg_t* f_cfg)
{
    arm_cmplx_mag_f32(
        (float32_t*)x_doppler, (float32_t*)arr->x_doppler_abs,
        f_cfg->n_channels * f_cfg->n_chirps);
    for (uint16_t idx_chirp = 0; idx_chirp < f_cfg->n_chirps; ++idx_chirp) {
        ifx_f32_t sum = 0.f;
        for (int idx_ch = 0; idx_ch < f_cfg->n_channels; ++idx_ch) {
            sum += arr->x_doppler_abs[idx_ch * f_cfg->n_chirps + idx_chirp];
        }
        arr->doppler_profile[idx_chirp] = sum / f_cfg->n_channels;
    }
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_DEPENDENCY "slim_algo_helpers"

#pragma IMAGINET_FRAGMENT_BEGIN "slim_algo_f32"
static inline int slim_algo_f32(
    slim_algo_output_t* out,
    ifx_f32_t* x_frame,
    frame_cfg_t* f_cfg,
    uint16_t min_range_bin,
    preproc_work_arrays_t* arr,
    int apply_adc_normalization)
{
    build_complex_range_image(
        x_frame, arr->x_range, f_cfg, arr->range_window, apply_adc_normalization);
    remove_mean_3d_cf64(
        arr->x_range, 1, f_cfg->n_channels, f_cfg->n_chirps, f_cfg->n_range_bins);
    get_range_profile(arr->x_range, arr, f_cfg, min_range_bin);

    uint32_t idx_peak_range;
    ifx_f32_t val_peak_range;
    const int profile_len = f_cfg->n_range_bins - min_range_bin;
    arm_max_f32(
        arr->range_profile, (uint32_t)profile_len, &val_peak_range, &idx_peak_range);

    idx_peak_range = filter_range_profile(
        arr->range_profile, profile_len, idx_peak_range, arr->conv_scratch);
    idx_peak_range += min_range_bin;

    get_single_range_bin_doppler(arr->x_range, arr, idx_peak_range, f_cfg);
    get_doppler_profile(arr->x_doppler, arr, f_cfg);

    uint32_t idx_peak_doppler;
    ifx_f32_t val_peak_doppler;
    arm_max_f32(
        arr->doppler_profile, f_cfg->n_chirps, &val_peak_doppler, &idx_peak_doppler);

    float phases[3];
    for (int i = 0; i < 3; ++i) {
        ifx_f32_t re =
            arr->x_doppler[i * f_cfg->n_chirps + idx_peak_doppler].data[0];
        ifx_f32_t im =
            arr->x_doppler[i * f_cfg->n_chirps + idx_peak_doppler].data[1];
        if (angle(re, im, phases + i) != ARM_MATH_SUCCESS) {
            out->success = 0;
            return -1;
        }
    }

    float azimuth = phase_monopulse(phases[2], phases[0]);
    float elevation = phase_monopulse(phases[2], phases[1]);
    azimuth += deg2rad(8.0f);
    elevation += deg2rad(24.0f);

    out->success = 1;
    out->detection.range_bin = (uint16_t)idx_peak_range;
    out->detection.doppler_bin = (uint16_t)idx_peak_doppler;
    out->detection.azimuth = azimuth;
    out->detection.elevation = elevation;
    out->detection.value = val_peak_doppler;
    return 0;
}
#pragma IMAGINET_FRAGMENT_END
