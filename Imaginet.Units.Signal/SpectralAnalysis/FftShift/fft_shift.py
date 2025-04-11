#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "fft_shift_py"

def fft_shift(input_data, output_data, axis):
    input_shifted = np.fft.fftshift(input_data, axes=(-axis-1))
    np.copyto(output_data, input_shifted)

#pragma IMAGINET_FRAGMENT_END
