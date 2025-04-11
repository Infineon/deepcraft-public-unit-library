#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "cfft"


def cfft(input, output, axis):
    # Shape input to complex array. Assumes that the right most/outer dimension is the imaginary part.
    input_complex = np.squeeze(input.view(dtype=np.complex64))

    # Compute FFT
    axis_complex = axis - 1
    axis_right_to_left = -axis_complex - 1
    result = np.fft.fft(input_complex, axis=axis_right_to_left)

    # Flatten complex array (e.g. complex [4,3,6] to float [4,3,6,2])
    np.stack((result.real, result.imag), axis=-1, out=output)

#pragma IMAGINET_FRAGMENT_END
