#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
from scipy import signal
# pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "highpass_filter"
def highpass_filter(input, output, cutoff_freq, freq, order=1):
    b, a = signal.butter(order, cutoff_freq, "highpass", fs=freq)
    result = signal.filtfilt(b, a, input, axis=0)
    np.copyto(output, result)


#pragma IMAGINET_FRAGMENT_END
