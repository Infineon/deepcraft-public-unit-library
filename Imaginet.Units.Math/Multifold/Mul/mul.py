﻿#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "mul"

def mul(a, b, output):
    np.multiply(a, b, out=output)

#pragma IMAGINET_FRAGMENT_END
