#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "min"

def min_py(input, axis, output):
    np.min(input, axis=-1-axis, out=output)

#pragma IMAGINET_FRAGMENT_END