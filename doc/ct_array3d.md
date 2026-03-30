doc/ct_array3d.md
# CTool Array3D Module

This module provides a type-safe, dynamic 3D array container (`ct::array::Array3D<T>`). It is designed for volumetric data, time-series of images, or tensor operations.

## Usage Examples

### 1. Construction

```cpp
#include "ct_array3d.hpp"
#include <iostream>

int main() {
    // Create a 3D volume: 2 slices, 3 rows, 4 columns
    ct::array::Array3D<double> volume(2, 3, 4);

    // Fill with data
    volume.fill(0.0);
    volume(0, 0, 0) = 1.5; // First element
    volume(1, 2, 3) = 3.14; // Last element

    std::cout << volume << "\n";

    // Extract a slice (2D array)
    ct::array::Array2D<double> slice0 = volume.slice(0);
    std::cout << "Slice 0:\n" << slice0 << "\n";

    return 0;
}
```

### 2. Math Operations

```cpp
ct::array::Array3D<float> data(2, 2, 2);
data.fill(1.0f);

// Scale entire volume
data.scale(2.0f); // All elements become 2.0f

// Add offset
data.add(10.0f); // All elements become 12.0f
```

### 3. Performance Note

- **Memory Layout**: `Array3D` uses a nested `vector` structure (`depth` -> `rows` -> `cols`).
- **Contiguity**: Unlike `Array` (which is also nested), 3D arrays are **not** fully contiguous in memory. For high-performance linear algebra on 3D data (e.g., FFT), consider flattening to a 1D `std::vector` with manual indexing, but `Array3D` offers safety and clarity for general use.

## Error Handling

- **`std::out_of_range`**: Thrown by `at()` or `slice()` if indices exceed dimensions.
- **`std::invalid_argument`**: Thrown during construction if input data is jagged.
- **`std::runtime_error`**: Thrown by `min()`/`max()` on empty arrays.
