#ifndef SOH_FEATURES_IMAGINET_H
#define SOH_FEATURES_IMAGINET_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SOH_FEATURE_COUNT      14
#define SOH_INPUT_SECONDS      300   // 5 minutes @ 1 Hz

/* Fragment entry point (declared here for clarity; defined in soh_features_imaginet.c) */
void soh_features14_f32(const float* input, float* output);

#ifdef __cplusplus
}
#endif

#endif
