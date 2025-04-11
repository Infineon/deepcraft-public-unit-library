#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "standard_deviation_f32"

static inline void standard_deviation_f32(const float* restrict input, float* restrict output, int count)
{
    if (count <= 0) {
        *output = 0.0f;
        return;
    }
    
    // Calculate mean
    float sum = 0.0f;
    for (int i = 0; i < count; i++) {
        sum += input[i];
    }
    float mean = sum / count;
    
    // Calculate variance
    float variance = 0.0f;
    for (int i = 0; i < count; i++) {
        float diff = input[i] - mean;
        variance += diff * diff;
    }
    variance /= count;
    
    // Standard deviation is square root of variance
    *output = sqrtf(variance);
}

#pragma IMAGINET_FRAGMENT_END