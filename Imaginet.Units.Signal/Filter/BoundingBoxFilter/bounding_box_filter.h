#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "detectionfilter_confidence_detection_f32"

// Candidate detection data structure
typedef struct {
    int index;          // Original detection index
    float score;        // Confidence score
    int class_id;       // Best class index
    float box[4];       // Bounding box coordinates [x, y, w, h]
} candidate_detection_t;

// Optimized for [Confidence, Detection] layout (axis_order = 0)
static inline float get_confidence_cd(const float* restrict input, int detection_count, int confidence, int detection) {
    return input[confidence * detection_count + detection];
}

static inline void detectionfilter_confidence_detection_f32(const float* restrict input, float* restrict output, int detection_count, int confidence_count, int max_detections, float threshold, float iou_threshold, int include_detected_flag)
{
    // For [Confidence, Detection] layout: always format { x, y, w, h, class } (no object confidence)
    int class_offset = 4;
    int num_classes = confidence_count - class_offset;

    // First pass: Find indices of detections that pass threshold (memory optimization)
    int keep_indices[detection_count];
    int num_candidates = 0;
    
    for (int i = 0; i < detection_count; ++i) {
        float max_conf = 0;

        for (int j = 0; j < num_classes; ++j) {
            float current_conf = get_confidence_cd(input, detection_count, class_offset + j, i);
            if (current_conf > max_conf) {
                max_conf = current_conf;
            }
        }

        // Keep candidates that pass threshold
        if (max_conf >= threshold) {
            keep_indices[num_candidates] = i;
            num_candidates++;
        }
    }

    // Exit if no candidates
    if (num_candidates == 0) {
        int output_count = include_detected_flag ? confidence_count + 1 : confidence_count;
        for (int i = 0; i < max_detections * output_count; ++i)
            output[i] = 0.0f;
        return;
    }

    // Allocate candidate array sized to actual candidates (memory optimization)
    candidate_detection_t candidates[num_candidates];

    // Second pass: Process only the kept indices to fill candidate data
    for (int idx = 0; idx < num_candidates; ++idx) {
        int i = keep_indices[idx];
        int max_class = -1;
        float max_conf = 0;

        // Find the best class for this detection
        for (int j = 0; j < num_classes; ++j) {
            float current_conf = get_confidence_cd(input, detection_count, class_offset + j, i);
            if (current_conf > max_conf) {
                max_conf = current_conf;
                max_class = j;   // Track class index
            }
        }

        candidates[idx].index = i;
        candidates[idx].score = max_conf;
        candidates[idx].class_id = max_class;

        // Extract box coordinates
        for (int j = 0; j < 4; ++j) {
            candidates[idx].box[j] = get_confidence_cd(input, detection_count, j, i);
        }
    }

    // Exit if no candidates
    if (num_candidates == 0) {
        int output_count = include_detected_flag ? confidence_count + 1 : confidence_count;
        for (int i = 0; i < max_detections * output_count; ++i)
            output[i] = 0.0f;
        return;
    }

    // Sort candidates by descending score (simple selection sort)
    for (int i = 0; i < num_candidates - 1; ++i) {
        int max_idx = i;
        for (int j = i + 1; j < num_candidates; ++j) {
            if (candidates[j].score > candidates[max_idx].score)
                max_idx = j;
        }
        if (max_idx != i) {
            // Swap candidate structs
            candidate_detection_t temp = candidates[i];
            candidates[i] = candidates[max_idx];
            candidates[max_idx] = temp;
        }
    }

    // Apply NMS (only compare boxes of the same class)
    int suppressed[num_candidates];
    for (int i = 0; i < num_candidates; ++i)
        suppressed[i] = 0;

    for (int i = 0; i < num_candidates; ++i) {
        if (suppressed[i]) continue;
        float* box_i = candidates[i].box;
        int class_i = candidates[i].class_id;  // Get class of current candidate

        // Convert box to (x1, y1, x2, y2) format
        float ix = box_i[0];  // center x
        float iy = box_i[1];  // center y
        float iw = box_i[2];  // width
        float ih = box_i[3];  // height
        float x1_i = ix - iw / 2, y1_i = iy - ih / 2;
        float x2_i = ix + iw / 2, y2_i = iy + ih / 2;

        for (int j = i + 1; j < num_candidates; ++j) {
            if (suppressed[j] || candidates[j].class_id != class_i) continue;

            float* box_j = candidates[j].box;
            float jx = box_j[0];  // center x
            float jy = box_j[1];  // center y
            float jw = box_j[2];  // width
            float jh = box_j[3];  // height
            float x1_j = jx - jw / 2, y1_j = jy - jh / 2;
            float x2_j = jx + jw / 2, y2_j = jy + jh / 2;

            // Calculate intersection area
            float x1 = fmaxf(x1_i, x1_j), y1 = fmaxf(y1_i, y1_j);
            float x2 = fminf(x2_i, x2_j), y2 = fminf(y2_i, y2_j);
            float inter_area = (x2 > x1 && y2 > y1) ? (x2 - x1) * (y2 - y1) : 0.0f;

            // Calculate IoU
            float area_i = iw * ih;
            float area_j = jw * jh;
            float union_area = area_i + area_j - inter_area;
            float iou = (union_area > 0) ? (inter_area / union_area) : 0.0f;

            if (iou > iou_threshold)
                suppressed[j] = 1;
        }
    }

    int output_idx = 0;
    int output_count = include_detected_flag ? confidence_count + 1 : confidence_count;
    for (int i = 0; i < num_candidates && output_idx < max_detections; ++i) {
        if (!suppressed[i]) {
            int input_idx = candidates[i].index;
            
            // For [Confidence, Detection] layout: Format {x, y, w, h, class}
            for (int j = 0; j < confidence_count; ++j) {
                output[j * max_detections + output_idx] = get_confidence_cd(input, detection_count, j, input_idx);
            }
            
            // Add detected flag if enabled
            if (include_detected_flag) {
                output[confidence_count * max_detections + output_idx] = 1.0f;
            }
            output_idx++;
        }
    }

    // Fill remaining slots with zeros
    for (; output_idx < max_detections; ++output_idx) {
        for (int j = 0; j < output_count; ++j)
            output[j * max_detections + output_idx] = 0.0f;
    }
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "detectionfilter_detection_confidence_f32"

// Candidate detection data structure
typedef struct {
    int index;          // Original detection index
    float score;        // Confidence score
    int class_id;       // Best class index
    float box[4];       // Bounding box coordinates [x, y, w, h]
} candidate_detection_t;

// Optimized for [Detection, Confidence] layout (axis_order = 1)
static inline float get_confidence_dc(const float* restrict input, int confidence_count, int confidence, int detection) {
    return input[detection * confidence_count + confidence];
}

static inline void detectionfilter_detection_confidence_f32(const float* restrict input, float* restrict output, int detection_count, int confidence_count, int max_detections, float threshold, float iou_threshold, int include_detected_flag)
{
    // For [Detection, Confidence] layout: always format { x, y, w, h, oc, class } (with object confidence)
    int class_offset = 5;
    int num_classes = confidence_count - class_offset;

    // First pass: Find indices of detections that pass threshold (memory optimization)
    int keep_indices[detection_count];
    int num_candidates = 0;
    
    for (int i = 0; i < detection_count; ++i) {
        float max_conf = 0;

        for (int j = 0; j < num_classes; ++j) {
            float current_conf = get_confidence_dc(input, confidence_count, class_offset + j, i);
            if (current_conf > max_conf) {
                max_conf = current_conf;
            }
        }

        // For [Detection, Confidence] layout: multiply object confidence with class confidence
        float obj_score = get_confidence_dc(input, confidence_count, 4, i);
        float current_conf = obj_score * max_conf;

        // Keep candidates that pass threshold
        if (current_conf >= threshold) {
            keep_indices[num_candidates] = i;
            num_candidates++;
        }
    }

    // Exit if no candidates
    if (num_candidates == 0) {
        int output_confidence_count = confidence_count - 1;
        int output_count = include_detected_flag ? output_confidence_count + 1 : output_confidence_count;
        for (int i = 0; i < max_detections * output_count; ++i)
            output[i] = 0.0f;
        return;
    }

    // Allocate buffers sized to actual candidates (memory optimization)
    int candidate_indices[num_candidates];
    float candidate_scores[num_candidates];
    int candidate_classes[num_candidates];
    float candidate_boxes[num_candidates][4];

    // Second pass: Process only the kept indices to fill candidate buffers
    for (int idx = 0; idx < num_candidates; ++idx) {
        int i = keep_indices[idx];
        int max_class = -1;
        float max_conf = 0;

        // Find the best class for this detection
        for (int j = 0; j < num_classes; ++j) {
            float current_conf = get_confidence_dc(input, confidence_count, class_offset + j, i);
            if (current_conf > max_conf) {
                max_conf = current_conf;
                max_class = j;   // Track class index
            }
        }

        candidate_indices[idx] = i;
        candidate_scores[idx] = max_conf;
        candidate_classes[idx] = max_class;

        // Extract box coordinates
        for (int j = 0; j < 4; ++j) {
            candidate_boxes[idx][j] = get_confidence_dc(input, confidence_count, j, i);
        }
    }

    // Exit if no candidates
    if (num_candidates == 0) {
        int output_confidence_count = confidence_count - 1;
        int output_count = include_detected_flag ? output_confidence_count + 1 : output_confidence_count;
        for (int i = 0; i < max_detections * output_count; ++i)
            output[i] = 0.0f;
        return;
    }

    // Sort candidates by descending score (simple selection sort)
    for (int i = 0; i < num_candidates - 1; ++i) {
        int max_idx = i;
        for (int j = i + 1; j < num_candidates; ++j) {
            if (candidate_scores[j] > candidate_scores[max_idx])
                max_idx = j;
        }
        if (max_idx != i) {
            // Swap all candidate data
            int temp_idx = candidate_indices[i];
            candidate_indices[i] = candidate_indices[max_idx];
            candidate_indices[max_idx] = temp_idx;

            float temp_score = candidate_scores[i];
            candidate_scores[i] = candidate_scores[max_idx];
            candidate_scores[max_idx] = temp_score;

            int temp_class = candidate_classes[i];
            candidate_classes[i] = candidate_classes[max_idx];
            candidate_classes[max_idx] = temp_class;

            float temp_box[4];
            for (int k = 0; k < 4; ++k) {
                temp_box[k] = candidate_boxes[i][k];
                candidate_boxes[i][k] = candidate_boxes[max_idx][k];
                candidate_boxes[max_idx][k] = temp_box[k];
            }
        }
    }

    // Apply NMS (only compare boxes of the same class)
    int suppressed[num_candidates];
    for (int i = 0; i < num_candidates; ++i)
        suppressed[i] = 0;

    for (int i = 0; i < num_candidates; ++i) {
        if (suppressed[i]) continue;
        float* box_i = candidate_boxes[i];
        int class_i = candidate_classes[i];  // Get class of current candidate

        // Convert box to (x1, y1, x2, y2) format
        float ix = box_i[0];  // center x
        float iy = box_i[1];  // center y
        float iw = box_i[2];  // width
        float ih = box_i[3];  // height
        float x1_i = ix - iw / 2, y1_i = iy - ih / 2;
        float x2_i = ix + iw / 2, y2_i = iy + ih / 2;

        for (int j = i + 1; j < num_candidates; ++j) {
            if (suppressed[j] || candidate_classes[j] != class_i) continue;

            float* box_j = candidate_boxes[j];
            float jx = box_j[0];  // center x
            float jy = box_j[1];  // center y
            float jw = box_j[2];  // width
            float jh = box_j[3];  // height
            float x1_j = jx - jw / 2, y1_j = jy - jh / 2;
            float x2_j = jx + jw / 2, y2_j = jy + jh / 2;

            // Calculate intersection area
            float x1 = fmaxf(x1_i, x1_j), y1 = fmaxf(y1_i, y1_j);
            float x2 = fminf(x2_i, x2_j), y2 = fminf(y2_i, y2_j);
            float inter_area = (x2 > x1 && y2 > y1) ? (x2 - x1) * (y2 - y1) : 0.0f;

            // Calculate IoU
            float area_i = iw * ih;
            float area_j = jw * jh;
            float union_area = area_i + area_j - inter_area;
            float iou = (union_area > 0) ? (inter_area / union_area) : 0.0f;

            if (iou > iou_threshold)
                suppressed[j] = 1;
        }
    }

    int output_idx = 0;
    int output_confidence_count = confidence_count - 1;  // Remove object confidence dimension
    int output_count = include_detected_flag ? output_confidence_count + 1 : output_confidence_count;
    for (int i = 0; i < num_candidates && output_idx < max_detections; ++i) {
        if (!suppressed[i]) {
            int input_idx = candidate_indices[i];
            
            // For [Detection, Confidence] layout: Format {x, y, w, h, oc, class} -> output: {x, y, w, h, multiplied_class}
            // Get object confidence for multiplication
            float obj_conf = get_confidence_dc(input, confidence_count, 4, input_idx);
            
            // Copy bounding box coordinates (x, y, w, h)
            for (int j = 0; j < 4; ++j) {
                output[j * max_detections + output_idx] = get_confidence_dc(input, confidence_count, j, input_idx);
            }
            
            // Copy class confidences multiplied by object confidence (skip object confidence at index 4)
            for (int j = class_offset; j < confidence_count; ++j) {
                float class_conf = get_confidence_dc(input, confidence_count, j, input_idx);
                output[(j - 1) * max_detections + output_idx] = obj_conf * class_conf;  // j-1 to shift indices after removing oc
            }
            
            // Add detected flag if enabled
            if (include_detected_flag) {
                output[(output_confidence_count) * max_detections + output_idx] = 1.0f;
            }
            output_idx++;
        }
    }

    // Fill remaining slots with zeros
    for (; output_idx < max_detections; ++output_idx) {
        for (int j = 0; j < output_count; ++j)
            output[j * max_detections + output_idx] = 0.0f;
    }
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "detectionfilter_confidence_detection_i8"

// Candidate detection data structure
typedef struct {
    int index;          // Original detection index
    float score;        // Confidence score
    int class_id;       // Best class index
    float box[4];       // Bounding box coordinates [x, y, w, h]
} candidate_detection_t;

// Helper function to convert int8 to float for confidence_detection layout
static inline float get_confidence_cd_i8(const int8_t* restrict input, int detection_count, int confidence, int detection) {
    return ((float)input[confidence * detection_count + detection] + 128.0f) / 255.0f;
}

// Optimized for [Confidence, Detection] layout (axis_order = 0) - Int8 version
static inline void detectionfilter_confidence_detection_i8(const int8_t* restrict input, int8_t* restrict output, int detection_count, int confidence_count, int max_detections, float threshold, float iou_threshold, int include_detected_flag)
{
    // For [Confidence, Detection] layout: always format { x, y, w, h, class } (no object confidence)
    int class_offset = 4;
    int num_classes = confidence_count - class_offset;

    // First pass: Find indices of detections that pass threshold (memory optimization)
    int keep_indices[detection_count];
    int num_candidates = 0;
    
    for (int i = 0; i < detection_count; ++i) {
        float max_conf = 0;

        for (int j = 0; j < num_classes; ++j) {
            float current_conf = get_confidence_cd_i8(input, detection_count, class_offset + j, i);
            if (current_conf > max_conf) {
                max_conf = current_conf;
            }
        }

        // Keep candidates that pass threshold
        if (max_conf >= threshold) {
            keep_indices[num_candidates] = i;
            num_candidates++;
        }
    }

    // Exit if no candidates
    if (num_candidates == 0) {
        int output_count = include_detected_flag ? confidence_count + 1 : confidence_count;
        for (int i = 0; i < max_detections * output_count; ++i)
            output[i] = -128;  // Initialize with -128, converts to 0.0 when read
        return;
    }

    // Allocate buffers sized to actual candidates (memory optimization)
    int candidate_indices[num_candidates];
    float candidate_scores[num_candidates];
    int candidate_classes[num_candidates];
    float candidate_boxes[num_candidates][4];

    // Second pass: Process only the kept indices to fill candidate buffers
    for (int idx = 0; idx < num_candidates; ++idx) {
        int i = keep_indices[idx];
        int max_class = -1;
        float max_conf = 0;

        // Find the best class for this detection
        for (int j = 0; j < num_classes; ++j) {
            float current_conf = get_confidence_cd_i8(input, detection_count, class_offset + j, i);
            if (current_conf > max_conf) {
                max_conf = current_conf;
                max_class = j;   // Track class index
            }
        }

        candidate_indices[idx] = i;
        candidate_scores[idx] = max_conf;
        candidate_classes[idx] = max_class;

        // Extract box coordinates
        for (int j = 0; j < 4; ++j) {
            candidate_boxes[idx][j] = get_confidence_cd_i8(input, detection_count, j, i);
        }
    }

    // Exit if no candidates
    if (num_candidates == 0) {
        int output_count = include_detected_flag ? confidence_count + 1 : confidence_count;
        for (int i = 0; i < max_detections * output_count; ++i)
            output[i] = -128;  // Initialize with -128, converts to 0.0 when read
        return;
    }

    // Sort candidates by descending score (simple selection sort)
    for (int i = 0; i < num_candidates - 1; ++i) {
        int max_idx = i;
        for (int j = i + 1; j < num_candidates; ++j) {
            if (candidate_scores[j] > candidate_scores[max_idx])
                max_idx = j;
        }
        if (max_idx != i) {
            // Swap all candidate data
            int temp_idx = candidate_indices[i];
            candidate_indices[i] = candidate_indices[max_idx];
            candidate_indices[max_idx] = temp_idx;

            float temp_score = candidate_scores[i];
            candidate_scores[i] = candidate_scores[max_idx];
            candidate_scores[max_idx] = temp_score;

            int temp_class = candidate_classes[i];
            candidate_classes[i] = candidate_classes[max_idx];
            candidate_classes[max_idx] = temp_class;

            float temp_box[4];
            for (int k = 0; k < 4; ++k) {
                temp_box[k] = candidate_boxes[i][k];
                candidate_boxes[i][k] = candidate_boxes[max_idx][k];
                candidate_boxes[max_idx][k] = temp_box[k];
            }
        }
    }

    // Apply NMS (only compare boxes of the same class)
    int suppressed[num_candidates];
    for (int i = 0; i < num_candidates; ++i)
        suppressed[i] = 0;

    for (int i = 0; i < num_candidates - 1; ++i) {
        if (suppressed[i]) continue;

        float* box_i = candidate_boxes[i];
        int class_i = candidate_classes[i];

        for (int j = i + 1; j < num_candidates; ++j) {
            if (suppressed[j] || candidate_classes[j] != class_i) continue;

            float* box_j = candidate_boxes[j];

            // Calculate IoU between box_i and box_j
            float ix = box_i[0];  // center x
            float iy = box_i[1];  // center y
            float iw = box_i[2];  // width
            float ih = box_i[3];  // height
            float x1_i = ix - iw / 2, y1_i = iy - ih / 2;
            float x2_i = ix + iw / 2, y2_i = iy + ih / 2;

            float jx = box_j[0];  // center x
            float jy = box_j[1];  // center y
            float jw = box_j[2];  // width
            float jh = box_j[3];  // height
            float x1_j = jx - jw / 2, y1_j = jy - jh / 2;
            float x2_j = jx + jw / 2, y2_j = jy + jh / 2;

            // Calculate intersection area
            float x1 = fmaxf(x1_i, x1_j), y1 = fmaxf(y1_i, y1_j);
            float x2 = fminf(x2_i, x2_j), y2 = fminf(y2_i, y2_j);
            float inter_area = (x2 > x1 && y2 > y1) ? (x2 - x1) * (y2 - y1) : 0.0f;

            // Calculate IoU
            float area_i = iw * ih;
            float area_j = jw * jh;
            float union_area = area_i + area_j - inter_area;
            float iou = (union_area > 0) ? (inter_area / union_area) : 0.0f;

            if (iou > iou_threshold)
                suppressed[j] = 1;
        }
    }

    int output_idx = 0;
    int output_count = include_detected_flag ? confidence_count + 1 : confidence_count;
    for (int i = 0; i < num_candidates && output_idx < max_detections; ++i) {
        if (!suppressed[i]) {
            int input_idx = candidate_indices[i];
            
            // For [Confidence, Detection] layout: Format {x, y, w, h, class} -> output: {x, y, w, h, class}
            // Copy all data (bounding box coordinates and class confidences)
            for (int j = 0; j < confidence_count; ++j) {
                float val = get_confidence_cd_i8(input, detection_count, j, input_idx);
                // Convert back to int8 range: [0,1] -> [-128,127]
                int8_t int8_val = (int8_t)(val * 255.0f - 128.0f);
                if (int8_val > 127) int8_val = 127;
                else if (int8_val < -128) int8_val = -128;
                output[j * max_detections + output_idx] = int8_val;
            }
            
            // Add detected flag if enabled
            if (include_detected_flag) {
                output[confidence_count * max_detections + output_idx] = 1;  // Detected = 1
            }
            output_idx++;
        }
    }

    // Fill remaining slots with zeros
    for (; output_idx < max_detections; ++output_idx) {
        for (int j = 0; j < output_count; ++j) {
            if (include_detected_flag && j == confidence_count) {
                output[j * max_detections + output_idx] = 0;  // Detected flag = 0 (not detected)
            } else {
                output[j * max_detections + output_idx] = -128;  // Initialize with -128, converts to 0.0 when read
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "detectionfilter_detection_confidence_i8"

// Candidate detection data structure
typedef struct {
    int index;          // Original detection index
    float score;        // Confidence score
    int class_id;       // Best class index
    float box[4];       // Bounding box coordinates [x, y, w, h]
} candidate_detection_t;

// Helper function to convert int8 to float for detection_confidence layout
static inline float get_confidence_dc_i8(const int8_t* restrict input, int confidence_count, int confidence, int detection) {
    return ((float)input[detection * confidence_count + confidence] + 128.0f) / 255.0f;
}

// Optimized for [Detection, Confidence] layout (axis_order = 1) - Int8 version
static inline void detectionfilter_detection_confidence_i8(const int8_t* restrict input, int8_t* restrict output, int detection_count, int confidence_count, int max_detections, float threshold, float iou_threshold, int include_detected_flag)
{
    // For [Detection, Confidence] layout: always format { x, y, w, h, oc, class } (with object confidence)
    int class_offset = 5;
    int num_classes = confidence_count - class_offset;
    
    // Calculate output count early to avoid redeclaration
    int output_confidence_count = confidence_count - 1;  // Remove object confidence dimension
    int output_count = include_detected_flag ? output_confidence_count + 1 : output_confidence_count;

    // First pass: Find indices of detections that pass threshold (memory optimization)
    int keep_indices[detection_count];
    int num_candidates = 0;
    
    for (int i = 0; i < detection_count; ++i) {
        float max_conf = 0;

        for (int j = 0; j < num_classes; ++j) {
            float current_conf = get_confidence_dc_i8(input, confidence_count, class_offset + j, i);
            if (current_conf > max_conf) {
                max_conf = current_conf;
            }
        }

        // For [Detection, Confidence] layout: multiply object confidence with class confidence
        float obj_score = get_confidence_dc_i8(input, confidence_count, 4, i);
        float current_conf = obj_score * max_conf;

        // Keep candidates that pass threshold
        if (current_conf >= threshold) {
            keep_indices[num_candidates] = i;
            num_candidates++;
        }
    }

    // Exit if no candidates
    if (num_candidates == 0) {
        for (int i = 0; i < max_detections * output_count; ++i)
            output[i] = -128;  // Initialize with 0
        return;
    }

    // Allocate buffers sized to actual candidates (memory optimization)
    int candidate_indices[num_candidates];
    float candidate_scores[num_candidates];
    int candidate_classes[num_candidates];
    float candidate_boxes[num_candidates][4];

    // Second pass: Process only the kept indices to fill candidate buffers
    for (int idx = 0; idx < num_candidates; ++idx) {
        int i = keep_indices[idx];
        int max_class = -1;
        float max_conf = 0;

        // Find the best class for this detection
        for (int j = 0; j < num_classes; ++j) {
            float current_conf = get_confidence_dc_i8(input, confidence_count, class_offset + j, i);
            if (current_conf > max_conf) {
                max_conf = current_conf;
                max_class = j;   // Track class index
            }
        }

        candidate_indices[idx] = i;
        candidate_scores[idx] = max_conf;
        candidate_classes[idx] = max_class;

        // Extract box coordinates
        for (int j = 0; j < 4; ++j) {
            candidate_boxes[idx][j] = get_confidence_dc_i8(input, confidence_count, j, i);
        }
    }

    // Exit if no candidates
    if (num_candidates == 0) {
        for (int i = 0; i < max_detections * output_count; ++i)
            output[i] = -128;  // Initialize with 0
        return;
    }

    // Sort candidates by descending score (simple selection sort)
    for (int i = 0; i < num_candidates - 1; ++i) {
        int max_idx = i;
        for (int j = i + 1; j < num_candidates; ++j) {
            if (candidate_scores[j] > candidate_scores[max_idx])
                max_idx = j;
        }
        if (max_idx != i) {
            // Swap all candidate data
            int temp_idx = candidate_indices[i];
            candidate_indices[i] = candidate_indices[max_idx];
            candidate_indices[max_idx] = temp_idx;

            float temp_score = candidate_scores[i];
            candidate_scores[i] = candidate_scores[max_idx];
            candidate_scores[max_idx] = temp_score;

            int temp_class = candidate_classes[i];
            candidate_classes[i] = candidate_classes[max_idx];
            candidate_classes[max_idx] = temp_class;

            float temp_box[4];
            for (int k = 0; k < 4; ++k) {
                temp_box[k] = candidate_boxes[i][k];
                candidate_boxes[i][k] = candidate_boxes[max_idx][k];
                candidate_boxes[max_idx][k] = temp_box[k];
            }
        }
    }

    // Apply NMS (only compare boxes of the same class)
    int suppressed[num_candidates];
    for (int i = 0; i < num_candidates; ++i)
        suppressed[i] = 0;

    for (int i = 0; i < num_candidates - 1; ++i) {
        if (suppressed[i]) continue;

        float* box_i = candidate_boxes[i];
        int class_i = candidate_classes[i];

        for (int j = i + 1; j < num_candidates; ++j) {
            if (suppressed[j] || candidate_classes[j] != class_i) continue;

            float* box_j = candidate_boxes[j];

            // Calculate IoU between box_i and box_j
            float ix = box_i[0];  // center x
            float iy = box_i[1];  // center y
            float iw = box_i[2];  // width
            float ih = box_i[3];  // height
            float x1_i = ix - iw / 2, y1_i = iy - ih / 2;
            float x2_i = ix + iw / 2, y2_i = iy + ih / 2;

            float jx = box_j[0];  // center x
            float jy = box_j[1];  // center y
            float jw = box_j[2];  // width
            float jh = box_j[3];  // height
            float x1_j = jx - jw / 2, y1_j = jy - jh / 2;
            float x2_j = jx + jw / 2, y2_j = jy + jh / 2;

            // Calculate intersection area
            float x1 = fmaxf(x1_i, x1_j), y1 = fmaxf(y1_i, y1_j);
            float x2 = fminf(x2_i, x2_j), y2 = fminf(y2_i, y2_j);
            float inter_area = (x2 > x1 && y2 > y1) ? (x2 - x1) * (y2 - y1) : 0.0f;

            // Calculate IoU
            float area_i = iw * ih;
            float area_j = jw * jh;
            float union_area = area_i + area_j - inter_area;
            float iou = (union_area > 0) ? (inter_area / union_area) : 0.0f;

            if (iou > iou_threshold)
                suppressed[j] = 1;
        }
    }

    int output_idx = 0;
    for (int i = 0; i < num_candidates && output_idx < max_detections; ++i) {
        if (!suppressed[i]) {
            int input_idx = candidate_indices[i];
            
            // For [Detection, Confidence] layout: Format {x, y, w, h, oc, class} -> output: {x, y, w, h, multiplied_class}
            // Get object confidence for multiplication
            float obj_conf = get_confidence_dc_i8(input, confidence_count, 4, input_idx);
            
            // Copy bounding box coordinates (x, y, w, h)
            for (int j = 0; j < 4; ++j) {
                float val = get_confidence_dc_i8(input, confidence_count, j, input_idx);
                // Convert back to int8 range: [0,1] -> [-128,127]
                int8_t int8_val = (int8_t)(val * 255.0f - 128.0f);
                if (int8_val > 127) int8_val = 127;
                else if (int8_val < -128) int8_val = -128;
                output[j * max_detections + output_idx] = int8_val;
            }
            
            // Copy class confidences multiplied by object confidence (skip object confidence at index 4)
            for (int j = class_offset; j < confidence_count; ++j) {
                float class_conf = get_confidence_dc_i8(input, confidence_count, j, input_idx);
                float multiplied_conf = obj_conf * class_conf;
                // Convert back to int8 range: [0,1] -> [-128,127]
                int8_t int8_val = (int8_t)(multiplied_conf * 255.0f - 128.0f);
                if (int8_val > 127) int8_val = 127;
                else if (int8_val < -128) int8_val = -128;
                output[(j - 1) * max_detections + output_idx] = int8_val;  // j-1 to shift indices after removing oc
            }
            
            // Add detected flag if enabled
            if (include_detected_flag) {
                output[(output_confidence_count) * max_detections + output_idx] = 1;  // Detected = 1
            }
            output_idx++;
        }
    }

    // Fill remaining slots with zeros
    for (; output_idx < max_detections; ++output_idx) {
        for (int j = 0; j < output_count; ++j) {
            if (include_detected_flag && j == output_confidence_count) {
                output[j * max_detections + output_idx] = 0;  // Detected flag = 0 (not detected)
            } else {
                output[j * max_detections + output_idx] = -128;  // Initialize with -128, converts to 0.0 when read
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END