#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "clip"

def clip(input, min, max, output):  
    np.clip(input, min, max, out=output)

#pragma IMAGINET_FRAGMENT_END

