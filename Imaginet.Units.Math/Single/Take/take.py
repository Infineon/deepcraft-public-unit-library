#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "take"

def take(input, index, axis):
    return np.take(input, index, axis=-axis-1)

#pragma IMAGINET_FRAGMENT_END

