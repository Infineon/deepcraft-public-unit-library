#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "subavg"

def subavg(input, axis, output):  
    avg_shape = list(input.shape)
    avg_shape[-axis-1] = 1
    avg = np.average(input, axis=-axis-1).reshape(avg_shape)
    np.copyto(output, input - avg)

#pragma IMAGINET_FRAGMENT_END

