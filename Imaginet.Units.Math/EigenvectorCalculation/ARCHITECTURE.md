# Architecture Overview  
Battery SOH Feature Extraction SDK

---

## High-Level Architecture

Battery Voltage (300 samples)
│
▼
SOH Feature Extraction
│
▼
Min-Max Normalization
│
▼
Feature Vector [14]
│
▼
ML Model (SOH Estimation)


---

## Design Principles

### Deterministic Execution
- Fixed input/output shapes
- No runtime memory allocation
- No dependency on historical state

### Embedded-Friendly
- Suitable for MCUs and edge processors
- Predictable memory and execution time
- Quantization-ready feature scaling

### Training / Deployment Consistency
- Feature definitions identical between training and inference
- Normalization constants embedded in the SDK
- Eliminates preprocessing mismatch risks

---

## Data Flow Description

1. Voltage samples are received as a fixed-length vector
2. Statistical and temporal features are computed
3. Each feature is normalized using predefined bounds
4. The normalized feature vector is passed to the ML model

---

## Extensibility Considerations

The current SDK is intentionally fixed-shape for stability.
Possible future extensions include:
- Configurable input length
- Support for different sampling rates
- Streaming or sliding-window feature extraction
- Direct int8 feature output

---

End of Architecture Document
