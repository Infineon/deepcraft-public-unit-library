#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "div"

def div(a, b, result):
    np.divide(a,b, out=output)

#pragma IMAGINET_FRAGMENT_END