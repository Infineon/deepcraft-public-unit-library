#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "downscale_image_f32"
def downscale_image_f32(input, output, scale_factor, x_size, y_size, channels):
    new_x_size = x_size // scale_factor
    new_y_size = y_size // scale_factor

    for y in range(new_y_size):
        for x in range(new_x_size):
            for c in range(channels):
                sum_val = 0.0
                for ky in range(scale_factor):
                    for kx in range(scale_factor):
                        input_y = y * scale_factor + ky
                        input_x = x * scale_factor + kx
                        sum_val += input[(input_y * x_size + input_x) * channels + c]
                output[(y * new_x_size + x) * channels + c] = sum_val / (scale_factor * scale_factor)
#pragma IMAGINET_FRAGMENT_END