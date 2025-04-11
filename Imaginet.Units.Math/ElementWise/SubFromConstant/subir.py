#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "subir"

def subir(a, b, output):
    np.subtract(a,b, out=output)

#pragma IMAGINET_FRAGMENT_END