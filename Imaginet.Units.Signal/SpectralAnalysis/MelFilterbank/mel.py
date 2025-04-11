#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "mel"

def mel(input, filter_points, output, nsize):  
   
    filters = np.zeros((len(filter_points)-2, nsize), dtype=np.float32)
    for n in range(len(filter_points)-2):
        n0 = int(filter_points[n])
        n1 = int(filter_points[n + 1])
        n2 = int(filter_points[n + 2])
        filters[n, n0 : n1] = np.linspace(0, 1, n1 - n0, endpoint=False)
        filters[n, n1 : n2] = np.linspace(1, 0, n2 - n1, endpoint=False)

    np.dot(filters, input, out=output)
    
#pragma IMAGINET_FRAGMENT_END