# DPS Preprocessor - Studio Unit

## What It Does

This DEEPCRAFT Studio unit (`Imaginet.Units.Signal.DPSPreprocessor`) takes a raw pressure +
temperature sensor data window and computes **19 numerical features** that a neural network
can use to classify events captured by a DPS barometric pressure and temperature sensor.

It replaces raw sensor values, which vary between sessions due to weather, altitude, and
sensor warm-up, with stable, relative statistics that describe *what happened* inside each
window, independent of absolute ambient conditions.

## Why It Exists

Models trained directly on raw DPS pressure and temperature readings often fail to generalise
across sessions. Two root causes are common:

1. **Absolute pressure offset**: each session records at a different ambient pressure. The model
   learns the offset rather than the event shape.
2. **Temperature drift**: the sensor heats up over time, so raw temperature values are dominated
   by a slow upward ramp rather than the brief thermal signature of an event.

This unit removes both problems by computing *relative* statistics within each window, making
the features invariant to session-to-session ambient differences.

## Input / Output

| | Shape | Type | Description |
|---|---|---|---|
| **Input** | `[n_samples, 2]` | Float32 | Sensor window. Column 0 = pressure (hPa), column 1 = temperature (°C) |
| **Output** | `[19]` | Float32 | Feature vector |

The recommended upstream configuration is a **Contextual Window** with 50% overlap so that
the model receives one prediction per stride. The sampling rate is read automatically from
the input stream metadata.

## The 19 Features

Features are grouped into five families, each targeting a specific physical aspect of the
pressure and temperature signal.

### Group 1 - Pressure Shape (indices 0-4)

Shape statistics of the pressure waveform after subtracting the window mean (baseline removal).

| # | Name | Formula | Description |
|---|---|---|---|
| 0 | `p_std` | Standard deviation of centred pressure | Measures the overall magnitude of pressure excursions within the window. |
| 1 | `p_range` | max(P) − min(P) | Peak-to-peak pressure amplitude. |
| 2 | `p_skew` | Third standardised moment | Asymmetry of the pressure pulse. Positive for sharp rising events, negative for falling ones. |
| 3 | `p_kurtosis` | Fourth standardised moment − 3 | Peakedness of the pressure waveform. High for brief, sharp pulses; low for gradual changes. |
| 4 | `p_energy` | mean(P_centred²) | Total signal energy after baseline removal. |

> **Baseline removal** (`P_centred = P − mean(P)`) makes all pressure features invariant to
> the absolute ambient pressure, which can differ by tens of hPa between recording sessions.

### Group 2 - Pressure Dynamics (indices 5-10)

First derivative of pressure dP/dt, computed as consecutive-sample differences scaled by the
sampling rate (units: hPa/s).

| # | Name | Formula | Description |
|---|---|---|---|
| 5 | `dp_mean` | mean(dP/dt) | Net pressure trend direction over the window. |
| 6 | `dp_std` | std(dP/dt) | Variability of the pressure rate-of-change. High during rapid pressure events, low during quiet periods. |
| 7 | `dp_max` | max(dP/dt) | Fastest positive pressure swing. |
| 8 | `dp_min` | min(dP/dt) | Fastest negative pressure swing. |
| 9 | `dp_range` | dp_max − dp_min | Total dynamic range of the pressure derivative. |
| 10 | `dp_abs_mean` | mean(\|dP/dt\|) | Average absolute rate of pressure change. High during any pressure event, low during idle. |

### Group 3 - Temperature (indices 11-13)

Temperature statistics focused on *change* rather than absolute value, to be robust to sensor
self-heating.

| # | Name | Formula | Description |
|---|---|---|---|
| 11 | `t_std` | std(T − mean(T)) | Temperature variability within the window. |
| 12 | `t_range` | max(T) − min(T) | Peak-to-peak temperature excursion. |
| 13 | `t_slope` | Linear regression slope × Fs | Rate of temperature change in °C/s. Captures sustained thermal events (e.g. warm airflow) vs. stable or cooling conditions. |

### Group 4 - Temperature Dynamics (indices 14-16)

First derivative of temperature dT/dt, analogous to Group 2 for pressure.

| # | Name | Formula | Description |
|---|---|---|---|
| 14 | `dt_mean` | mean(dT/dt) | Average temperature derivative. |
| 15 | `dt_std` | std(dT/dt) | Temperature derivative variability. |
| 16 | `dt_abs_mean` | mean(\|dT/dt\|) | Average absolute temperature change rate. |

### Group 5 - Cross-Channel & Trend (indices 17-18)

| # | Name | Formula | Description |
|---|---|---|---|
| 17 | `p_t_corr` | Pearson correlation of centred P and T | Captures whether pressure and temperature change together (correlated) or independently (uncorrelated). |
| 18 | `p_slope` | Linear regression slope of pressure × Fs | Overall pressure trend in hPa/s. Less sensitive to noise than `dp_mean`. |

## How It Is Computed

The implementation makes **three passes** over the sample window with zero dynamic memory
allocation:

1. **Pass 1** - Accumulate sums for means, track min/max, and gather linear-regression
   accumulators (Σi·P, Σi·T) for slope computation.
2. **Pass 2** - Using the means from Pass 1, compute centred variance, skewness, kurtosis,
   energy, and cross-channel covariance in a single loop.
3. **Pass 3** - Compute consecutive-sample derivatives and accumulate their statistics
   (mean, std, min, max, abs-mean) without allocating a derivative array.

All arithmetic uses `double` (64-bit) intermediates for numerical stability; the final 19
values are written as `float32`.

## Files

| File | Purpose |
|---|---|
| `FeatureExtraction.imunit` | DEEPCRAFT Studio unit definition (XML). Declares input/output shapes, parameters, contracts, and links to the code fragments. |
| `feature_extraction.py` | Python implementation (`dps_feature_extraction` fragment). Used during training in Studio and for Python unit tests. |
| `feature_extraction.h` | C implementation (`dps_feature_extraction_f32` fragment). No dynamic allocation; only `math.h` and `float.h` dependencies. |

## How to Use in DEEPCRAFT Studio

1. In the preprocessing pipeline, place a **DPS sensor unit** followed by a **Contextual
   Window**, then this unit.
2. The sampling rate is read automatically from the input stream, no manual configuration
   needed.
3. The NN input layer should expect shape `[19]`.
