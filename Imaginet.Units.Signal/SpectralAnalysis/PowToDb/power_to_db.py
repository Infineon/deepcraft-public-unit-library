#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "power_to_db"

def power_to_db(input, output, ref, amin, topdb):
    log_spec = 10.0 * np.log10(np.maximum(amin, input)) - 10.0 * np.log10(np.maximum(amin, ref))
    log_spec = np.maximum(log_spec, log_spec.max() - topdb)
    np.copyto(output, log_spec)

#pragma IMAGINET_FRAGMENT_END