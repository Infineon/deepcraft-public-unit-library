#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "sum"

def sum(input, axis, output):
    np.sum(input, axis=-axis-1, out=output)

#pragma IMAGINET_FRAGMENT_END

