#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_postproc_init"
/* Handle-backed state is always passed in as a raw int8_t* byte pointer by
 * the code generator, regardless of the "type" declared on the <Handle> in
 * the .imunit (that attribute only affects allocated size, not the C
 * parameter type). It also packs consecutive Handles back-to-back with no
 * padding, so a Float32 handle can end up at a byte offset that isn't
 * 4-byte aligned (e.g. right after a 1-byte and a 6-byte Handle). Directly
 * dereferencing such an address as float* is undefined behavior and can
 * hard-fault on cores whose FPU load/store instructions require natural
 * alignment (unlike plain integer loads, which many Cortex-M cores tolerate
 * unaligned). So all access to gesture_confidence_detected goes through
 * memcpy-based helpers, which are alignment-safe on every architecture.
 *
 * The generated model.c emits fragment functions ordered alphabetically by
 * fragment name, not by their order in this file, so "gesture_postproc_f32"
 * is emitted before "gesture_postproc_init". These helpers are therefore
 * defined identically (guarded) in both fragments, so whichever one the
 * generator emits first provides them for both. */
#ifndef GESTURE_POSTPROC_F32_HELPERS
#define GESTURE_POSTPROC_F32_HELPERS
static inline float gpp_load_f32(const int8_t* restrict bytes, int i) {
    float v;
    memcpy(&v, bytes + (size_t)i * sizeof(float), sizeof(float));
    return v;
}
static inline void gpp_store_f32(int8_t* restrict bytes, int i, float v) {
    memcpy(bytes + (size_t)i * sizeof(float), &v, sizeof(float));
}
#endif

static inline int gesture_postproc_init(
    int8_t* restrict prev_detected_gesture_bytes,
    int8_t* restrict gesture_count_bytes,
    int8_t* restrict gesture_confidence_detected_bytes,
    int num_classes,
    int none_class_index)
{
    uint8_t* restrict prev_detected_gesture = (uint8_t*)prev_detected_gesture_bytes;
    uint8_t* restrict gesture_count = (uint8_t*)gesture_count_bytes;

    *prev_detected_gesture = (uint8_t)none_class_index;
    for (int i = 0; i < num_classes; i++) {
        gesture_count[i] = 0;
        gpp_store_f32(gesture_confidence_detected_bytes, i, 0.0f);
    }
    return 0;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "gesture_postproc_f32"
/* Debounced argmax classifier with per-class confidence/count hysteresis.
 *
 * Each call finds the highest-confidence class in `input`. A class other
 * than `none_class_index` is only reported once its accumulated confidence
 * or its consecutive-frame count crosses a threshold, and only if either
 * the "no gesture" class has been seen recently or the class differs from
 * the previously reported one. This debounces the raw per-frame model
 * output into discrete, non-repeating gesture events.
 *
 * `output` is a one-hot vector, same length as `input`: 1.0 at the
 * confirmed class index (or at `none_class_index` if nothing is confirmed
 * this frame) and 0.0 elsewhere, so it is a drop-in replacement for the
 * raw per-class model output. */
#ifndef GESTURE_POSTPROC_F32_HELPERS
#define GESTURE_POSTPROC_F32_HELPERS
static inline float gpp_load_f32(const int8_t* restrict bytes, int i) {
    float v;
    memcpy(&v, bytes + (size_t)i * sizeof(float), sizeof(float));
    return v;
}
static inline void gpp_store_f32(int8_t* restrict bytes, int i, float v) {
    memcpy(bytes + (size_t)i * sizeof(float), &v, sizeof(float));
}
#endif

static inline void gesture_postproc_f32(
    const float* restrict input,
    float* restrict output,
    int num_classes,
    int none_class_index,
    float min_detect_confidence,
    float confidence_threshold,
    int count_threshold,
    int reset_count,
    int none_count,
    int8_t* restrict prev_detected_gesture_bytes,
    int8_t* restrict gesture_count_bytes,
    int8_t* restrict gesture_confidence_detected_bytes)
{
    uint8_t* restrict prev_detected_gesture = (uint8_t*)prev_detected_gesture_bytes;
    uint8_t* restrict gesture_count = (uint8_t*)gesture_count_bytes;

    float max_confidence = 0.0f;
    int max_confidence_class = none_class_index;

    /* Find the highest-confidence class, tracking each class' best-ever
     * confidence seen since its last reset. */
    for (int i = 0; i < num_classes; i++) {
        if (input[i] > max_confidence) {
            max_confidence = input[i];
            if (max_confidence > gpp_load_f32(gesture_confidence_detected_bytes, i)) {
                gpp_store_f32(gesture_confidence_detected_bytes, i, max_confidence);
            }
            max_confidence_class = i;
        }
    }

    /* Update the consecutive-frame counters. */
    if (max_confidence > min_detect_confidence && max_confidence_class != none_class_index) {
        gesture_count[max_confidence_class]++;
    } else {
        gesture_count[none_class_index]++;

        if (gesture_count[none_class_index] >= reset_count) {
            for (int i = 0; i < num_classes; i++) {
                if (i != none_class_index) {
                    gesture_count[i] = 0;
                }
                gpp_store_f32(gesture_confidence_detected_bytes, i, 0.0f);
            }
        }
    }

    /* Decide whether the current best class should be reported. */
    int confirmed_class = none_class_index;
    if (max_confidence_class != none_class_index) {
        int i = max_confidence_class;
        if (gpp_load_f32(gesture_confidence_detected_bytes, i) > confidence_threshold ||
            gesture_count[i] >= count_threshold) {
            if (gesture_count[none_class_index] >= none_count ||
                i != (int)*prev_detected_gesture) {
                for (int j = 0; j < num_classes; j++) {
                    gesture_count[j] = 0;
                    gpp_store_f32(gesture_confidence_detected_bytes, j, 0.0f);
                }
                *prev_detected_gesture = (uint8_t)i;
                confirmed_class = i;
            }
        }
    }

    for (int j = 0; j < num_classes; j++) {
        output[j] = 0.0f;
    }
    output[confirmed_class] = 1.0f;
}
#pragma IMAGINET_FRAGMENT_END
