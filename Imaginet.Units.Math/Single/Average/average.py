#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "average"

def average(input, axis, output):  
    result = np.average(input, axis=-axis-1)
    np.copyto(output, result)

#pragma IMAGINET_FRAGMENT_END

