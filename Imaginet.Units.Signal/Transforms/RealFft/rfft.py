#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "rfft"

def rfft(input, output, axis):
    result = np.fft.rfft(input, axis=-axis-1)                  # compute FFT
    np.stack((result.real, result.imag), axis=-1, out=output)  # flatten complex array (e.g. complex [4,3,6] to float [4,3,6,2])

#pragma IMAGINET_FRAGMENT_END

