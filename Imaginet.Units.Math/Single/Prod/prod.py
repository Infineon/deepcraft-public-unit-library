#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "prod"

def prod(input, axis, output):
    np.prod(input, axis=-axis-1, out=output)

#pragma IMAGINET_FRAGMENT_END

