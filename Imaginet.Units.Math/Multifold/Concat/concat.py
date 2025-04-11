#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "concat"

def concat(arrays, output, axis):
    np.concatenate(arrays, axis=-1-axis, out=output)

#pragma IMAGINET_FRAGMENT_END