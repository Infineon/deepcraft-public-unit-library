#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "hamming_mul"

def hamming_mul(a, b, output):
    np.multiply(a, b, out=output)

#pragma IMAGINET_FRAGMENT_END
