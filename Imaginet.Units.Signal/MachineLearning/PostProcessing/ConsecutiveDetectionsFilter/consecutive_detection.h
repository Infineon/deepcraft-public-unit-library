#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "consecutive_detection"
static inline void consecutive_detection(const float* restrict input, float* restrict output, float confidence_threshold, int8_t* detect_count, int8_t min_consecutive_count, int8_t* last_detected_class, int default_class_index, int num_classes) {
    
  // Find the class with the highest confidence
    int max_confidence_class = 0;
    float max_confidence = input[0];

    for (int i = 1; i < num_classes; i++) {
        if (input[i] > max_confidence) {
            max_confidence = input[i];
            max_confidence_class = i;
        }
    }

    // If confidence is below the threshold, reset to the "no prediction" class (default class)
    if (max_confidence < confidence_threshold) {
        max_confidence_class = default_class_index;
    }

    // Check if the current highest-confidence class matches the previous one
    if (max_confidence_class != *last_detected_class) {
        // Reset the detection count since the class changed
        *detect_count = 1;
    }

    else {
        // Increment the detection count for the matching class
        (*detect_count)++;
    }

    // Update the last detected class
    *last_detected_class = max_confidence_class;

    // Check if we should produce a detection output
    if (*detect_count >= min_consecutive_count && max_confidence_class != default_class_index) {
        // Detection confirmed for the class
        for (int i = 0; i < num_classes; i++) {
            output[i] = (i == max_confidence_class) ? 1.0f : 0.0f; // Activate the detected class
        }
    }

    else {
        // No detection, output "no prediction" state
        for (int i = 0; i < num_classes; i++) {
            output[i] = (i == default_class_index) ? 1.0f : 0.0f; // No class is detected
        }
    }
}

#pragma IMAGINET_FRAGMENT_END
