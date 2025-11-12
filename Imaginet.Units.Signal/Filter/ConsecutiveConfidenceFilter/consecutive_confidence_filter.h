#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "consecutive_confidence_filter_f32"

static inline void consecutive_confidence_filter_f32(const float* restrict input, float* restrict output, int8_t* gesture_count, int8_t* prev_detected_gesture, int8_t* restrict gesture_confidence_detected_bytes, int count, float threshold, float min_detection_threshold, int count_threshold, int reset_count, int null_count, int use_confidence_tracking)
{
    // Cast the byte buffer to float array for accessing confidence tracking state
    float* gesture_confidence_detected = (float*)gesture_confidence_detected_bytes;
    
    // Find the gesture with maximum confidence
    int max_gesture = 0;
    float max_confidence = input[0];
    for (int i = 1; i < count; i++) {
        if (input[i] > max_confidence) {
            max_confidence = input[i];
            max_gesture = i;
        }
    }

    // Update gesture_confidence_detected if tracking is enabled
    if (use_confidence_tracking && input[max_gesture] > gesture_confidence_detected[max_gesture]) {
        gesture_confidence_detected[max_gesture] = input[max_gesture];
    }

    // Update gesture counts based on minimum detection threshold
    if (max_confidence > min_detection_threshold && max_gesture != 0) {
        gesture_count[max_gesture]++;
    }
    else {
        gesture_count[0]++; // Increment "no gesture" count
        if (gesture_count[0] >= reset_count) {
            // Reset counts for all gestures except "no gesture" (index 0)
            for (int i = 1; i < count; i++) {
                gesture_count[i] = 0;
            }
            // Reset confidence tracking if enabled
            if (use_confidence_tracking) {
                for (int i = 0; i < count; i++) {
                    gesture_confidence_detected[i] = 0.0f;
                }
            }
        }
    }

    // Determine confidence to use for detection based on toggle
    float confidence_to_check = use_confidence_tracking ? gesture_confidence_detected[max_gesture] : max_confidence;

    // Check detection conditions
    if (max_gesture > 0 &&
        (confidence_to_check > threshold || gesture_count[max_gesture] >= count_threshold) &&
        (gesture_count[0] >= null_count || max_gesture != prev_detected_gesture[0])) {
        // Gesture detected
        for (int i = 0; i < count; i++) {
            output[i] = (i == max_gesture) ? 1.0f : 0.0f; // Set output to 1 for detected gesture
        }
        // Reset all counts and confidence tracking if enabled
        for (int i = 0; i < count; i++) {
            gesture_count[i] = 0;
            if (use_confidence_tracking) {
                gesture_confidence_detected[i] = 0.0f;
            }
        }
        prev_detected_gesture[0] = max_gesture; // Update previous gesture
    }
    else {
        // No gesture detected
        for (int i = 0; i < count; i++) {
            output[i] = (i == 0) ? 1.0f : 0.0f; // Indicate "no gesture" (index 0)
        }
    }
}

#pragma IMAGINET_FRAGMENT_END