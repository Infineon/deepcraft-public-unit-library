#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#include "soh_features.h"

/*
  NOTE:
  - Your original file uses `auto inline` (C++-style). DEEPCRAFT/Imaginet C backend expects C.
  - This file provides a C-compatible implementation + an Imaginet fragment entry point.
*/

#ifndef SOH_ENTROPY_BINS
#define SOH_ENTROPY_BINS 64
#endif

#define EPSD 1e-15

static const float soh_feature_min[SOH_FEATURE_COUNT] = {
    0.00039999999999995595f,   // dV_0_30  (drop_30)
    0.000700000000000145f,     // dV_0_60  (drop_60)
    0.001000000000000334f,     // dV_0_90  (drop_90)
    0.0012999999999996348f,    // dV_0_120 (drop_120)
    2.906578908590249f,        // se_0_30  (entropy_30)
    3.6060067350798213f,       // se_0_60  (entropy_60)
    4.082774989630446f,        // se_0_90  (entropy_90)
    4.382205872785203f,        // se_0_120 (entropy_120)
    4.187448f,                 // mean_all
    4.184f,                    // min_all
    4.1961f,                   // max_all
    3.2607555125868145e-07f,   // var_all
    0.32722424783147f,         // skew_all
    -1.0356039296367694f       // kurt_all
};

static const float soh_feature_max[SOH_FEATURE_COUNT] = {
    0.005399999999999849f,     // dV_0_30  (drop_30)
    0.006917647058823029f,     // dV_0_60  (drop_60)
    0.00792941176470574f,      // dV_0_90  (drop_90)
    0.008999999999999453f,     // dV_0_120 (drop_120)
    3.2544190928869763f,       // se_0_30  (entropy_30)
    3.947394763441653f,        // se_0_60  (entropy_60)
    4.288018259799295f,        // se_0_90  (entropy_90)
    4.610499439765967f,        // se_0_120 (entropy_120)
    4.197705802047783f,        // mean_all
    4.1969f,                   // min_all
    4.1991f,                   // max_all
    6.800962666666626e-06f,    // var_all
    0.8597378869111623f,       // skew_all
    0.14917725027817808f       // kurt_all
};

static inline double entropy64(const double *x, int m)
{
    if (m <= 0) return 0.0;

    double xmin = DBL_MAX;
    for (int i = 0; i < m; ++i) {
        if (x[i] < xmin) xmin = x[i];
    }

    double s = 0.0;
    for (int i = 0; i < m; ++i) {
        double v = x[i] - xmin;
        if (v > 0.0) s += v;
    }

    if (s <= 1e-12) return 0.0;

    double H = 0.0;
    for (int i = 0; i < m; ++i) {
        double v = x[i] - xmin;
        if (v <= 0.0) continue;

        double p = v / s;
        if (p < 1e-12) p = 1e-12;
        else if (p > 1.0) p = 1.0;

        H -= p * log(p);
    }

    return H;
}

/* Original algorithm (kept 1:1 in math) */
static inline void soh_features_from_voltage_dynamic_impl(const float *v, int n, float outfeat[SOH_FEATURE_COUNT])
{
    if (v == NULL || n <= 0) {
        for (int i = 0; i < SOH_FEATURE_COUNT; ++i) outfeat[i] = 0.0f;
        return;
    }

    static double vd[2000];
    int len = (n > 2000) ? 2000 : n;
    for (int i = 0; i < len; ++i) vd[i] = (double)v[i];

    const int t30  = (n - 1 <  30) ? (n - 1) :  30;
    const int t60  = (n - 1 <  60) ? (n - 1) :  60;
    const int t90  = (n - 1 <  90) ? (n - 1) :  90;
    const int t120 = (n - 1 < 120) ? (n - 1) : 120;

    double f0 = vd[0] - vd[t30];
    double f1 = vd[0] - vd[t60];
    double f2 = vd[0] - vd[t90];
    double f3 = vd[0] - vd[t120];

    double f4 = entropy64(vd, t30  + 1);
    double f5 = entropy64(vd, t60  + 1);
    double f6 = entropy64(vd, t90  + 1);
    double f7 = entropy64(vd, t120 + 1);

    double mn = DBL_MAX, mx = -DBL_MAX, s = 0.0;
    for (int i = 0; i < n; ++i) {
        if (vd[i] < mn) mn = vd[i];
        if (vd[i] > mx) mx = vd[i];
        s += vd[i];
    }
    double meanv = s / (double)n;

    double m2 = 0.0, m3 = 0.0, m4 = 0.0;
    for (int i = 0; i < n; ++i) {
        double d = vd[i] - meanv;
        m2 += d * d;
        m3 += d * d * d;
        m4 += d * d * d * d;
    }

    double var = m2 / (double)n;
    double std = sqrt(var);

    double skew = 0.0;
    double kurt = 0.0;

    if (std >= 1e-12) {
        double cm3 = m3 / (double)n;
        double cm4 = m4 / (double)n;
        skew = cm3 / (std * std * std);
        kurt = cm4 / (var * var) - 3.0;
    }

    double outd[SOH_FEATURE_COUNT] = {
        f0, f1, f2, f3, f4, f5, f6, f7,
        meanv, mn, mx, var, skew, kurt
    };

    for (int i = 0; i < SOH_FEATURE_COUNT; ++i) {
        double xmin = (double)soh_feature_min[i];
        double xmax = (double)soh_feature_max[i];
        double range = xmax - xmin;

        double norm = 0.0;
        if (range > 1e-12) {
            norm = (outd[i] - xmin) / range;
        }

        if (norm < 0.0) norm = 0.0;
        if (norm > 1.0) norm = 1.0;

        outfeat[i] = (float)norm;
    }
}

/*
  Imaginet fragment: entry point used by the custom Function unit.
  - Input:  float32[300]  (5 minutes @ 1 Hz)
  - Output: float32[14]
*/
#pragma IMAGINET_FRAGMENT_BEGIN "soh_features14_f32"
static inline void soh_features14_f32(const float* restrict input, float* restrict output)
{
    // hard-coded 300 samples to match your requirement
    soh_features_from_voltage_dynamic_impl(input, 300, output);
}
#pragma IMAGINET_FRAGMENT_END
