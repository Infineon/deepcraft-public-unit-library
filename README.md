
# Deepcraft™ Studio public unit library

DEEPCRAFT™ Studio (formerly Imagimob Studio) is an end-to-end platform for developing AI / Machine Learning (ML) models for the edge. The platform is designed to support users in creating robust and high-quality models that are ready for deployment in commercial products. 

The repository contains the DEEPCRAFT™ Studio unit library. Use the units as reference code for you own custom units or create a pull request with your own units to contribute to the library. 

## Signal Processing Units

### Transforms
The 'Transforms' directory contains units that convert signals from the time domain to the frequency domain:
- Dct: Implements the Discrete Cosine Transform
- Fft: Implements the Fast Fourier Transform
- RealFft: Implements the Fast Fourier Transform for real numbers only

### Window Functions
The 'Window Functions' directory contains units that modulate the input signal in the time domain before transforming it into the frequency domain:
- Hamming: Applies the Hamming Window function
- Hann: Applies the Hann Window function

### Spectral Analysis
The 'Spectral Analysis' directory contains units used for analyzing the frequency spectrum of a signal:
- FftShift: Shifts the zero-frequency component to the center of the spectrum
- MelFilterbank: Applies a Mel-scale filterbank to the spectrum
- PowToDb: Converts the power spectrum to a decibel scale

### Temporal Analysis
The Temporal Analysis directory contains units that analyze the time-domain representation of a signal:
- SlidingWindow: Applies a sliding window function to the time-domain signal

### Common
The 'Common' directory contains compound units that cover common use cases in signal processing:
- MelSpectrogram: Generates a Mel spectrogram, representing the spectral content of a signal on the Mel scale.

## Math Units

### Element Wise
The 'Element Wise' directory contains units that perform operations on each element of a tensor individually. The shape (dimensions) of the input tensor is retained in the output tensor. 
- Abs: Calculates the absolute value of each element
- AddConstant: Adds a consistent value to each element
- Clip: Values outside the interval are clamped to the interval edges
- Log: Computes the logarithm of each element
- Pow: Raises each element to the power of a constant
- Scale: Scales each element by a constant factor
- SubFromConstant: Subtracts each element from a constant

### Fixed Point
The Fixed Point directory contains units related to fixed-point quantization, converting floating-point numbers into fixed-point numbers. 
- BitUtilization: Measures bit usage
- Dequantize: Converts data from quantized form (fixed point) to a floating-point form
- QShift: Performs bit shift operation in fixed point format
- Quantize: Converts data from floating point form to a fixed point form
The supported fixed point formats include Q31, Q15, and Q7.

### Meta
The Meta directory contains units that operate on the meta-data of a tensor, such as its shape, frequency, or identifiers, without performing data operations. 
- Reshape: Changes the shape of a tensor
- TrimShape: Adjusts the shape of a tensor, typically by removing unnecessary dimensions

### Multifold
The Multifold directory houses units that operate on multiple tensors along a given axis. These operations typically involve combining or performing other operations on several tensors.
- Add: Adds tensors
- Concat: Concatenates tensors
- Div: Divides tensors
- DotT: Performs the dot product of tensors
- Mul: Multiplies tensors
- Stack: Stacks tensors along a new axis
- Sub: Subtracts tensors

### Single
The Single directory contains units that perform operations on a single tensor along a specified axis. These operations typically involve statistical computations or extracting specific information from a tensor.
- Argmax: Finds the indices of maximum values
- Average: Calculates the average
- Max: Identifies the maximum values
- Min: Identifies the minimum values
- Norm: Computes the norm
- Prod: Calculates the product
- Sum: Computes the sum
- Take: Extracts elements