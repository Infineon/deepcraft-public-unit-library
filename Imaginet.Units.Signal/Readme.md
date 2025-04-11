# Signal Processing Units

## Transforms
The 'Transforms' directory contains units that convert signals from the time domain to the frequency domain:
- Dct: Implements the Discrete Cosine Transform
- Fft: Implements the Fast Fourier Transform
- RealFft: Implements the Fast Fourier Transform for real numbers only

## Window Functions
The 'Window Functions' directory contains units that modulate the input signal in the time domain before transforming it into the frequency domain:
- Hamming: Applies the Hamming Window function
- Hann: Applies the Hann Window function

## Spectral Analysis
The 'Spectral Analysis' directory contains units used for analyzing the frequency spectrum of a signal:
- FftShift: Shifts the zero-frequency component to the center of the spectrum
- MelFilterbank: Applies a Mel-scale filterbank to the spectrum
- PowToDb: Converts the power spectrum to a decibel scale

## Temporal Analysis
The Temporal Analysis directory contains units that analyze the time-domain representation of a signal:
- SlidingWindow: Applies a sliding window function to the time-domain signal

## Common
The 'Common' directory contains compound units that cover common use cases in signal processing:
- MelSpectrogram: Generates a Mel spectrogram, representing the spectral content of a signal on the Mel scale.