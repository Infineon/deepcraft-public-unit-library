DEEPCRAFT Studio / Imaginet Custom Unit: Battery.SOH.Features14

Files:
- Battery_SOH_Features14.immunit
- soh_features_imaginet.c
- soh_features_imaginet.h

How to use (high level):
1) Put these files into a folder that is added to "Imaginet include directories" in your project.
2) Restart / Rebuild preprocessing graph. The unit should appear under /Battery/SOH as "SOH Features (14D)".
3) Feed a 1D float32 array of length 300 into the layer; it outputs float32[14].

Notes:
- This implementation hard-codes n=300 in the fragment to match your "5 minutes (300s)" requirement.
- If you want variable length, we can add an Int32Option (n) and relax the contract.
