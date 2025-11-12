#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "cfar_1d_f32"
static inline void cfar_1d_f32(const float* restrict input, float* restrict output, int dimensions, int antenna_mode, int window_size, int gap_cells, float bias, int num_targets, int count_antennas, int count_samples, int count_chirps, int output_amplitude)
{
    // Initialize tracking arrays for top targets
    float max_values[3] = { 0.0f };
    int max_indices[3] = { -1, -1, -1 };

    // Define loop bounds and index calculations based on dimension
    int outer_limit = (dimensions == 0) ? count_chirps : count_samples;
    int inner_start = gap_cells + window_size;
    int inner_limit = (dimensions == 0) ? count_samples - gap_cells - window_size : count_chirps - gap_cells - window_size;

    // Process along specified dimension
    for (int outer = 0; outer < outer_limit; outer++) {
        for (int inner = inner_start; inner < inner_limit; inner++) {
            // Calculate indices based on dimension
            int i = (dimensions == 0) ? outer : inner;  // chirp index
            int j = (dimensions == 0) ? inner : outer;  // sample index

            // Get cell value (average or max across antennas)
            float cell_value = 0.0f;
            float antenna_values[4] = { 0.0f }; // Store values for each antenna for phase calculation

            // Average mode
            if (antenna_mode == 0) { 
                for (int k = 0; k < count_antennas; k++) {
                    antenna_values[k] = input[i * count_samples * count_antennas + j * count_antennas + k];
                    cell_value += antenna_values[k];
                }
                cell_value /= count_antennas;
            }

            // Max mode
            else {
                for (int k = 0; k < count_antennas; k++) {
                    float val = input[i * count_samples * count_antennas + j * count_antennas + k];
                    antenna_values[k] = val;
                    cell_value = (val > cell_value) ? val : cell_value;
                }
            }

            // Calculate reference window average (leading + trailing)
            float window_sum = 0.0f;
            int window_count = 0;

            // Leading and trailing windows based on dimension
            for (int w = inner - window_size - gap_cells; w < inner - gap_cells; w++) {
                for (int k = 0; k < count_antennas; k++) {
                    int idx_i = (dimensions == 0) ? i : w;
                    int idx_j = (dimensions == 0) ? w : j;
                    window_sum += input[idx_i * count_samples * count_antennas + idx_j * count_antennas + k];
                    window_count++;
                }
            }

            for (int w = inner + gap_cells + 1; w <= inner + gap_cells + window_size; w++) {
                for (int k = 0; k < count_antennas; k++) {
                    int idx_i = (dimensions == 0) ? i : w;
                    int idx_j = (dimensions == 0) ? w : j;
                    window_sum += input[idx_i * count_samples * count_antennas + idx_j * count_antennas + k];
                    window_count++;
                }
            }

            // Apply CFAR threshold
            float threshold = (window_sum / window_count) * bias;
            if (cell_value > threshold) {
                // Calculate target index based on dimension
                int target_index = (dimensions == 0) ? j : i * count_samples + j;

                // Insert into sorted top targets array if qualifies
                for (int k = 0; k < num_targets; k++) {
                    if (cell_value > max_values[k]) {
                        // Shift lower values down
                        for (int m = num_targets - 1; m > k; m--) {
                            max_values[m] = max_values[m - 1];
                            max_indices[m] = max_indices[m - 1];
                        }
                        max_values[k] = cell_value;
                        max_indices[k] = target_index;
                        break;
                    }
                }
            }
        }
    }

    // Fill output array: [index1, value1, index2, value2, ...] if amplitude enabled, or [index1, index2, ...] if disabled
    int target_count = 0;
    int stride = (output_amplitude == 1) ? 2 : 1;  // 2 elements per target if amplitude enabled, 1 if disabled

    for (int i = 0; i < num_targets; i++) {
        if (max_indices[i] >= 0) {
            output[target_count * stride] = (float)max_indices[i];
            if (output_amplitude == 1) {
                output[target_count * stride + 1] = max_values[i];
            }
            target_count++;
        }
    }

    // Zero out remaining output if fewer targets found
    for (int i = target_count * stride; i < num_targets * stride; i++) {
        output[i] = 0.0f;
    }
}
#pragma IMAGINET_FRAGMENT_END