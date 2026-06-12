# CTool Array3D Module

This module provides a type-safe, dynamic 3D array container (`ctool::array::Array3D<T>`). It is designed for volumetric data, time-series, or tensor operations, offering contiguous memory storage for high-performance access.

## Class Reference: `ctool::array::Array3D<T>`

### Features
- **Safety**: Bounds-checked access via `at(depth, row, col)`.
- **Flexibility**: Resizable dimensions and 2D slice extraction.
- **Operations**: Bulk filling by depth (slice), row, or column across all slices.
- **Math**: In-place scaling and addition operations.
- **Performance**: Depth-major contiguous memory layout for optimal cache-locality.

## Usage Examples

### 1. Construction

```cpp
#include "src/ctool/array3D.hpp"
#include <iostream>
#include <vector>

int main() {
    // Create a 3D volume: 2 slices, 3 rows, 4 columns
    ctool::array::Array3D<double> volume(2, 3, 4, 0.0);

    // From nested vectors (Depth > Rows > Cols)
    std::vector<std::vector<std::vector<int>>> data = {
        {{1, 2}, {3, 4}}, 
        {{5, 6}, {7, 8}}
    };
    ctool::array::Array3D<int> tensor(data);

    std::cout << "Tensor size: " << tensor.size() << " elements.\n";

    return 0;
}
```

### 2. Access and Manipulation

```cpp
ctool::array::Array3D<double> vol(10, 10, 10);

// Safe access (throws std::out_of_range)
vol.at(0, 5, 2) = 12.3;

// Fast unchecked access
vol(1, 1, 1) = 3.14;

// Extraction: Get a 2D slice (layer) as Array2D<T>
ctool::array::Array2D<double> layer2 = vol.slice(2);
```

### 3. Bulk Operations

```cpp
ctool::array::Array3D<float> cube(4, 4, 4);

// Fill a specific slice (depth index 1)
cube.fillDepth(1, 1.0f);

// Fill row 2 in ALL slices
cube.fillRow(2, 5.0f);

// Fill column 0 in ALL slices
cube.fillColumn(0, -1.0f);

// Math operations
cube.scale(2.0).add(10.0);
```

### 4. Memory and Interop

```cpp
ctool::array::Array3D<double> data(5, 5, 5);

// The volume is stored contiguously in memory
// Indexing: (d * rows * cols) + (r * cols) + c
double* raw_ptr = data.data();

// Convert to nested vectors
auto vec = data.toVector();
```

## Performance Note

- **Memory Layout**: `Array3D` uses a single `std::vector<T>` for contiguous layout. This provides excellent cache-locality for volumetric filtering, FFT, or large-scale tensor math.
- **Resize**: Resizing with different row/column counts triggers a re-mapping of data, which is an $O(N)$ operation.

## Error Handling

- **`std::out_of_range`**: Thrown by `at()`, `slice()`, `fillDepth()`, etc., if indices exceed dimensions.
- **`std::invalid_argument`**: Thrown during construction if input data is non-rectangular (inconsistent dimensions).
- **`std::runtime_error`**: Thrown by `min()`/`max()` on empty arrays.
