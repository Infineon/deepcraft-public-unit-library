# Battery SOH Feature Extraction SDK  
**for Deepcraft Studio**

---

## Overview

The Battery SOH Feature Extraction SDK provides a deterministic and embedded-ready
feature extraction function for battery State-of-Health (SOH) applications.

The SDK converts **5 minutes (300 seconds) of battery voltage data**
into a **14-dimensional normalized feature vector**, designed to be consumed
by downstream machine learning models deployed with Deepcraft Studio.

This SDK is intended for **edge-AI and embedded deployments**, with strict control
over memory usage, numerical stability, and input/output shapes.

---

## Key Capabilities

- Fixed-shape, deterministic feature extraction
- Feature-wise normalization to `[0, 1]`
- No dynamic memory allocation
- Stateless and reentrant execution
- Quantization-friendly preprocessing
- Fully compatible with Deepcraft Studio preprocessing graphs

---

## Function Summary

| Item | Value |
|----|----|
| Function Name | `Battery.SOH.Features14` |
| Type | Imaginet Custom Function |
| Category | `/Battery/SOH` |
| Input | Float32[300] |
| Output | Float32[14] |

---

## Input Specification

### Input Tensor

| Name | Type | Shape |
|----|----|----|
| `v` | Float32 | `[300]` |

**Description**
- Battery voltage samples
- Sampling rate: **1 Hz**
- Duration: **300 seconds**
- Unit: **Volts**

**Constraints**
- Input must be a 1D tensor
- Length must be exactly 300 samples
- Enforced by Deepcraft contracts

---

## Output Specification

### Output Tensor

| Name | Type | Shape |
|----|----|----|
| `feat` | Float32 | `[14]` |

**Description**
- 14-dimensional SOH feature vector
- Each feature normalized to `[0, 1]`

---

## Feature Definition

| Index | Feature | Description |
|----|----|----|
| 0–3 | dV_0_x | Voltage drop over 30 / 60 / 90 / 120 seconds |
| 4–7 | se_0_x | Shannon entropy over 30 / 60 / 90 / 120 seconds |
| 8 | mean_all | Mean voltage |
| 9 | min_all | Minimum voltage |
|10 | max_all | Maximum voltage |
|11 | var_all | Variance |
|12 | skew_all | Skewness |
|13 | kurt_all | Excess kurtosis |

---

## Feature Normalization

All features are normalized using feature-wise min-max normalization:

