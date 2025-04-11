#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "add"

def add(a, b, output):
    np.add(a,b, out=output)

#pragma IMAGINET_FRAGMENT_END

