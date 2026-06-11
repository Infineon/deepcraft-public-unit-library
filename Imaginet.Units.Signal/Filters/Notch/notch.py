# pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
from scipy import signal
# pragma IMAGINET_INCLUDES_END

# pragma IMAGINET_FRAGMENT_BEGIN "notch_filter"


def notch_filter(input, output, freq, frequency, Q):
    b, a = signal.iirnotch(frequency, Q, freq)
    result = signal.filtfilt(b, a, input, axis=0)
    np.copyto(output, result)


# pragma IMAGINET_FRAGMENT_END
