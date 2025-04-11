#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
from scipy.fftpack import dct as ddct
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "dct"

def dct(input, output, axis, output_count):
	ddct(input, axis=-axis-1, n=output_count).take(indices=range(0, output_count), axis=-axis-1, out=output) 
    
#pragma IMAGINET_FRAGMENT_END

