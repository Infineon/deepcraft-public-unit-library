#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_preproc_types"

typedef float ifx_f32_t;

typedef struct {
    ifx_f32_t data[2];
} ifx_cf64_t;

typedef struct {
    uint16_t range_bin;
    uint16_t doppler_bin;
    float azimuth;
    float elevation;
    float value;
} slim_algo_detection_t;

typedef struct {
    int success;
    slim_algo_detection_t detection;
} slim_algo_output_t;

typedef struct {
    uint16_t n_channels;
    uint16_t n_chirps;
    uint16_t n_samples;
    uint16_t n_range_bins;
} frame_cfg_t;

typedef struct {
    frame_cfg_t cfg;
    ifx_cf64_t* x_range;
    ifx_f32_t* x_range_abs;
    ifx_f32_t* x_range_abs_mean;
    ifx_cf64_t* x_range_slice;
    ifx_cf64_t* x_doppler;
    ifx_f32_t* x_doppler_abs;
    ifx_f32_t* doppler_window;
    ifx_f32_t* doppler_profile;
    ifx_f32_t* range_profile;
    ifx_f32_t* range_window;
    ifx_f32_t* conv_scratch;
} preproc_work_arrays_t;

#ifdef _MSC_VER
/* Must stay <= the .imunit's "state_header" expression (currently 96), which
 * sizes the handle's header region on top of the actual work buffers. */
static_assert(sizeof(preproc_work_arrays_t) <= 96, "preproc_work_arrays_t is too big");
#endif

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_DEPENDENCY "gesture_preproc_types"

#pragma IMAGINET_FRAGMENT_BEGIN "preproc_work_arrays_bind"
static inline void preproc_work_arrays_bind(
    preproc_work_arrays_t* arr, int8_t* handle_bytes, int conv_len)
{
    char* mem = (char*)handle_bytes + (int)sizeof(preproc_work_arrays_t);
    const uint32_t len_hfr =
        (uint32_t)arr->cfg.n_channels * arr->cfg.n_chirps * arr->cfg.n_range_bins;
    const uint32_t len_img = (uint32_t)arr->cfg.n_chirps * arr->cfg.n_range_bins;
    const uint32_t len_cch = (uint32_t)arr->cfg.n_channels * arr->cfg.n_chirps;
    const uint32_t sz_f = (uint32_t)sizeof(ifx_f32_t);
    const uint32_t sz_c = (uint32_t)sizeof(ifx_cf64_t);

    arr->x_range = (ifx_cf64_t*)mem;
    mem += sz_c * len_hfr;

    arr->x_range_abs = (ifx_f32_t*)mem;
    mem += sz_f * len_hfr;

    arr->x_range_abs_mean = (ifx_f32_t*)mem;
    mem += sz_f * len_img;

    arr->x_range_slice = (ifx_cf64_t*)mem;
    mem += sz_c * len_cch;

    arr->x_doppler = (ifx_cf64_t*)mem;
    mem += sz_c * len_cch;

    arr->x_doppler_abs = (ifx_f32_t*)mem;
    mem += sz_f * len_cch;

    arr->doppler_profile = (ifx_f32_t*)mem;
    mem += sz_f * arr->cfg.n_chirps;

    arr->doppler_window = (ifx_f32_t*)mem;
    mem += sz_f * arr->cfg.n_chirps;

    arr->range_profile = (ifx_f32_t*)mem;
    mem += sz_f * arr->cfg.n_range_bins;

    arr->range_window = (ifx_f32_t*)mem;
    mem += sz_f * arr->cfg.n_samples;

    arr->conv_scratch = (ifx_f32_t*)mem;
    (void)conv_len;
}
#pragma IMAGINET_FRAGMENT_END
