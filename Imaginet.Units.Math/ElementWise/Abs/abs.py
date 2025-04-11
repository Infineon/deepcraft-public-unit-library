#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "abs"

def abs(input,output):
    np.absolute(input, out=output)

#pragma IMAGINET_FRAGMENT_END