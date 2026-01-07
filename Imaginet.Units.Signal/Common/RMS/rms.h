#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "rms"

static inline void rms_f32(const float* restrict input, float* restrict rms_out,
    int count, int to_dB) {
    float temp_rms = 0;
    int temp_clip = 0;
    float x;
    float y;
    float highest_value = 0;

    // calculate the rms value & clipping
    for (int i = 0; i < count; i++) {
        x = (float)input[i];
        y = x;

        x = x * x;     // square value
        temp_rms += x; // add values
    }
    // compute the RMS value
    float rms_value = sqrt(temp_rms / count);
    // add a small threshold to avoid infinity or log(0) issues
    const float MINIMUM_RMS = 1e-12; // Small, positive value close to zero
    if (rms_value < MINIMUM_RMS) {
        rms_value = MINIMUM_RMS;
    }
    // convert RMS to dB if to_dB is 1
    if (to_dB == 1) {
        *rms_out = 20 * log10(rms_value);
    }
    else {
        *rms_out = rms_value;
    }
}

#pragma IMAGINET_FRAGMENT_END
