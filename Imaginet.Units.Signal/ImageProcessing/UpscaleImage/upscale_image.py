#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "upscale_image_py"

def upscale_image(input, output, scale_factor, x_size, y_size, channels):
    output_index = 0
    for y in range(y_size):
        for sy in range(scale_factor):
            for x in range(x_size):
                for sx in range(scale_factor):
                    for c in range(channels):
                        output[output_index] = input[(y * x_size + x) * channels + c]
                        output_index += 1
#pragma IMAGINET_FRAGMENT_END
