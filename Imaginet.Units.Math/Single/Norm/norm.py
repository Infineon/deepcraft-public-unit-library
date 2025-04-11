#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "norm"

def norm(input, axis, output):
    axis = len(input.shape) - axis - 1          # reverse order
    np.copyto(output, np.linalg.norm(input, axis=axis))
    
#pragma IMAGINET_FRAGMENT_END

