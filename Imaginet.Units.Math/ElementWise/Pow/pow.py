#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "pow"

def pow(input, output, exponent):
    np.power(input, exponent, out=output)

#pragma IMAGINET_FRAGMENT_END