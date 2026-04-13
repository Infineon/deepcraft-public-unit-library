# Release Notes  
Battery SOH Feature Extraction SDK

---

## v1.0.0 – Initial Public Release

### Features
- 14-dimensional SOH feature extraction from battery voltage
- Fixed input length: 300 samples (5 minutes @ 1 Hz)
- Feature-wise min-max normalization to `[0, 1]`
- Deterministic, stateless execution
- No dynamic memory allocation
- Compatible with Deepcraft Studio preprocessing graphs

### Technical Details
- Input type: Float32[300]
- Output type: Float32[14]
- C implementation with Imaginet fragment interface
- Enforced input validation via contracts

### Known Limitations
- Input length is fixed and not configurable
- Sampling rate is assumed to be 1 Hz
- Not designed for streaming or sliding-window execution

### Compatibility
- Deepcraft Studio: Supported
- Imaginet Backend: Supported
- Quantized ML pipelines: Supported

---

End of Release Notes

