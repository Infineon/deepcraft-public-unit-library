# Math Units

## Element Wise
The 'Element Wise' directory contains units that perform operations on each element of a tensor individually. The shape (dimensions) of the input tensor is retained in the output tensor. 
- Abs: Calculates the absolute value of each element
- AddConstant: Adds a consistent value to each element
- Clip: Values outside the interval are clamped to the interval edges
- Log: Computes the logarithm of each element
- Pow: Raises each element to the power of a constant
- Scale: Scales each element by a constant factor
- SubFromConstant: Subtracts each element from a constant

## Fixed Point
The Fixed Point directory contains units related to fixed-point quantization, converting floating-point numbers into fixed-point numbers. 
- BitUtilization: Measures bit usage
- Dequantize: Converts data from quantized form (fixed point) to a floating-point form
- QShift: Performs bit shift operation in fixed point format
- Quantize: Converts data from floating point form to a fixed point form
The supported fixed point formats include Q31, Q15, and Q7.

## Meta
The Meta directory contains units that operate on the meta-data of a tensor, such as its shape, frequency, or identifiers, without performing data operations. 
- Reshape: Changes the shape of a tensor
- TrimShape: Adjusts the shape of a tensor, typically by removing unnecessary dimensions

## Multifold
The Multifold directory houses units that operate on multiple tensors along a given axis. These operations typically involve combining or performing other operations on several tensors.
- Add: Adds tensors
- Concat: Concatenates tensors
- Div: Divides tensors
- DotT: Performs the dot product of tensors
- Mul: Multiplies tensors
- Stack: Stacks tensors along a new axis
- Sub: Subtracts tensors

## Single
The Single directory contains units that perform operations on a single tensor along a specified axis. These operations typically involve statistical computations or extracting specific information from a tensor.
- Argmax: Finds the indices of maximum values
- Average: Calculates the average
- Max: Identifies the maximum values
- Min: Identifies the minimum values
- Norm: Computes the norm
- Prod: Calculates the product
- Sum: Computes the sum
- Take: Extracts elements