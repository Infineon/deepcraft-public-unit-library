#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "cfar_2d_f32"
static inline void cfar_2d_f32(const float* restrict input, float* restrict output, int antenna_mode, int window_size_range, int window_size_doppler, int gap_cells_range, int gap_cells_doppler, float bias, int num_targets, int count_antennas, int count_samples, int count_chirps, int output_amplitude)
{
    // Initialize tracking arrays for top targets
    float max_values[3] = { 0.0f };
    int max_range_indices[3] = { -1, -1, -1 };
    int max_doppler_indices[3] = { -1, -1, -1 };

    // Define processing bounds
    int range_start = gap_cells_range + window_size_range;
    int range_limit = count_samples - gap_cells_range - window_size_range;
    int doppler_start = gap_cells_doppler + window_size_doppler;
    int doppler_limit = count_chirps - gap_cells_doppler - window_size_doppler;

    // Process all cells in the 2D range-Doppler map
    for (int doppler = doppler_start; doppler < doppler_limit; doppler++) {
        for (int range = range_start; range < range_limit; range++) {
            // Get cell value (average or max across antennas)
            float cell_value = 0.0f;
            float antenna_values[4] = { 0.0f }; // Store values for each antenna

            // Average mode
            if (antenna_mode == 0) {
                for (int k = 0; k < count_antennas; k++) {
                    antenna_values[k] = input[doppler * count_samples * count_antennas + range * count_antennas + k];
                    cell_value += antenna_values[k];
                }
                cell_value /= count_antennas;
            }

            // Max mode
            else {
                for (int k = 0; k < count_antennas; k++) {
                    float val = input[doppler * count_samples * count_antennas + range * count_antennas + k];
                    antenna_values[k] = val;
                    cell_value = (val > cell_value) ? val : cell_value;
                }
            }

            // Calculate 2D reference window average
            float window_sum = 0.0f;
            int window_count = 0;

            // Sum all reference cells in the 2D window (excluding gap cells around CUT)
            // Iterate through all cells in the reference window
            for (int d_offset = -window_size_doppler - gap_cells_doppler; d_offset <= window_size_doppler + gap_cells_doppler; d_offset++) {
                for (int r_offset = -window_size_range - gap_cells_range; r_offset <= window_size_range + gap_cells_range; r_offset++) {
                    // Skip the gap cells around the CUT (center cell)
                    if (abs(d_offset) <= gap_cells_doppler && abs(r_offset) <= gap_cells_range) {
                        continue;
                    }

                    int ref_doppler = doppler + d_offset;
                    int ref_range = range + r_offset;

                    // Check bounds
                    if (ref_doppler >= 0 && ref_doppler < count_chirps && ref_range >= 0 && ref_range < count_samples) {
                        for (int k = 0; k < count_antennas; k++) {
                            window_sum += input[ref_doppler * count_samples * count_antennas + ref_range * count_antennas + k];
                            window_count++;
                        }
                    }
                }
            }

            // Apply CFAR threshold
            float threshold = (window_sum / window_count) * bias;
            if (cell_value > threshold) {
                // Insert into sorted top targets array if qualifies
                for (int k = 0; k < num_targets; k++) {
                    if (cell_value > max_values[k]) {
                        // Shift lower values down
                        for (int m = num_targets - 1; m > k; m--) {
                            max_values[m] = max_values[m - 1];
                            max_range_indices[m] = max_range_indices[m - 1];
                            max_doppler_indices[m] = max_doppler_indices[m - 1];
                        }
                        max_values[k] = cell_value;
                        max_range_indices[k] = range;
                        max_doppler_indices[k] = doppler;
                        break;
                    }
                }
            }
        }
    }

    // Fill output array: [range_idx1, doppler_idx1, value1, range_idx2, doppler_idx2, value2, ...] if amplitude enabled, or [range_idx1, doppler_idx1, range_idx2, doppler_idx2, ...] if disabled
    int target_count = 0;
    int stride = (output_amplitude == 1) ? 3 : 2;  // 3 elements per target if amplitude enabled, 2 if disabled

    for (int i = 0; i < num_targets; i++) {
        if (max_range_indices[i] >= 0 && max_doppler_indices[i] >= 0) {
            output[target_count * stride] = (float)max_range_indices[i];
            output[target_count * stride + 1] = (float)max_doppler_indices[i];
            if (output_amplitude == 1) {
                output[target_count * stride + 2] = max_values[i];
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
