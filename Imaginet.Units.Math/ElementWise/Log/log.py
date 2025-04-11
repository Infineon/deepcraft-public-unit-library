#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "log"

def log(input, output, base):
    log_data = np.log(input) 
    if base != 0:
        log_data /= np.log(base)
    np.copyto(output, log_data)

#pragma IMAGINET_FRAGMENT_END