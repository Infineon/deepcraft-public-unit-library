#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "dott"

def dott(a, b, output):
    np.copyto(output, np.dot(a, b.T).T)

#pragma IMAGINET_FRAGMENT_END