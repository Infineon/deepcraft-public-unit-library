#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "argmax"

def argmax(input, axis, output):
    np.argmax(input, axis=-1-axis, out=output)

#pragma IMAGINET_FRAGMENT_END