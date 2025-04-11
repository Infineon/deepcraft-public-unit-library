#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "max"

def max_py(input, axis, output):
    np.max(input, axis=-1-axis, out=output)

#pragma IMAGINET_FRAGMENT_END