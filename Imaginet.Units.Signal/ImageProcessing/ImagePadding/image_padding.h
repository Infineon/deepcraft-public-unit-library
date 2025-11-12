#pragma IMAGINET_FRAGMENT_BEGIN "image_padding_f32"

static inline void image_padding_f32(const float* restrict input,
    int input_height, int input_width,
    int target_height, int target_width,
    int channels,
    float* restrict output)
{
    // Initialize output buffer to 0 (black)
    for (int i = 0; i < target_height * target_width * channels; i++) {
        output[i] = 0.0f;
    }

    // Calculate padding offsets to center the image
    int offset_y = (target_height - input_height) / 2;
    int offset_x = (target_width - input_width) / 2;

    // If input is larger than target, calculate crop starting points
    int input_start_y = 0;
    int input_start_x = 0;
    int copy_height = input_height;
    int copy_width = input_width;

    // Adjust for cropping if input is larger than target
    if (input_height > target_height) {
        input_start_y = (input_height - target_height) / 2;
        copy_height = target_height;
        offset_y = 0;  // No padding needed when cropping
    }
    else {
        offset_y = offset_y < 0 ? 0 : offset_y;
    }

    if (input_width > target_width) {
        input_start_x = (input_width - target_width) / 2;
        copy_width = target_width;
        offset_x = 0;  // No padding needed when cropping
    }
    else {
        offset_x = offset_x < 0 ? 0 : offset_x;
    }

    // Copy input image to centered position in output
    for (int y = 0; y < copy_height; y++) {
        for (int x = 0; x < copy_width; x++) {
            for (int c = 0; c < channels; c++) {
                int input_idx = ((y + input_start_y) * input_width + (x + input_start_x)) * channels + c;
                int output_idx = ((y + offset_y) * target_width + (x + offset_x)) * channels + c;
                output[output_idx] = input[input_idx];
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "image_padding_int8"

static inline void image_padding_int8(const int8_t* restrict input,
    int input_height, int input_width,
    int target_height, int target_width,
    int channels,
    int8_t* restrict output)
{
    // Initialize output buffer to 0 (black)
    for (int i = 0; i < target_height * target_width * channels; i++) {
        output[i] = 0;
    }

    // Calculate padding offsets to center the image
    int offset_y = (target_height - input_height) / 2;
    int offset_x = (target_width - input_width) / 2;

    // If input is larger than target, calculate crop starting points
    int input_start_y = 0;
    int input_start_x = 0;
    int copy_height = input_height;
    int copy_width = input_width;

    // Adjust for cropping if input is larger than target
    if (input_height > target_height) {
        input_start_y = (input_height - target_height) / 2;
        copy_height = target_height;
        offset_y = 0;  // No padding needed when cropping
    }
    else {
        offset_y = offset_y < 0 ? 0 : offset_y;
    }

    if (input_width > target_width) {
        input_start_x = (input_width - target_width) / 2;
        copy_width = target_width;
        offset_x = 0;  // No padding needed when cropping
    }
    else {
        offset_x = offset_x < 0 ? 0 : offset_x;
    }

    // Copy input image to centered position in output
    for (int y = 0; y < copy_height; y++) {
        for (int x = 0; x < copy_width; x++) {
            for (int c = 0; c < channels; c++) {
                int input_idx = ((y + input_start_y) * input_width + (x + input_start_x)) * channels + c;
                int output_idx = ((y + offset_y) * target_width + (x + offset_x)) * channels + c;
                output[output_idx] = input[input_idx];
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "image_padding_uint8"

static inline void image_padding_uint8(const uint8_t* restrict input,
    int input_height, int input_width,
    int target_height, int target_width,
    int channels,
    uint8_t* restrict output)
{
    // Initialize output buffer to 0 (black)
    for (int i = 0; i < target_height * target_width * channels; i++) {
        output[i] = 0;
    }

    // Calculate padding offsets to center the image
    int offset_y = (target_height - input_height) / 2;
    int offset_x = (target_width - input_width) / 2;

    // If input is larger than target, calculate crop starting points
    int input_start_y = 0;
    int input_start_x = 0;
    int copy_height = input_height;
    int copy_width = input_width;

    // Adjust for cropping if input is larger than target
    if (input_height > target_height) {
        input_start_y = (input_height - target_height) / 2;
        copy_height = target_height;
        offset_y = 0;  // No padding needed when cropping
    }
    else {
        offset_y = offset_y < 0 ? 0 : offset_y;
    }

    if (input_width > target_width) {
        input_start_x = (input_width - target_width) / 2;
        copy_width = target_width;
        offset_x = 0;  // No padding needed when cropping
    }
    else {
        offset_x = offset_x < 0 ? 0 : offset_x;
    }

    // Copy input image to centered position in output
    for (int y = 0; y < copy_height; y++) {
        for (int x = 0; x < copy_width; x++) {
            for (int c = 0; c < channels; c++) {
                int input_idx = ((y + input_start_y) * input_width + (x + input_start_x)) * channels + c;
                int output_idx = ((y + offset_y) * target_width + (x + offset_x)) * channels + c;
                output[output_idx] = input[input_idx];
            }
        }
    }
}

#pragma IMAGINET_FRAGMENT_END