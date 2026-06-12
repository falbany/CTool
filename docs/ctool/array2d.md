# CTool Array2D Module

This module provides a type-safe, dynamic 2D array container (`ctool::array::Array2D<T>`). It offers bounds-checked access, dimension management, and robust mathematical operations, acting as a high-performance alternative to `std::vector<std::vector<T>>`.

## Class Reference: `ctool::array::Array2D<T>`

### Features
- **Safety**: Bounds-checked access via `at(row, col)`.
- **Flexibility**: Resizable dimensions, dynamic row addition, and sub-array extraction.
- **Search**: Find exact values or nearest neighbors with coordinate returns.
- **Math**: In-place scaling, addition, and matrix transposition.
- **Performance**: Contiguous memory layout for cache-locality.

## Usage Examples

### 1. Construction

```cpp
#include "src/ctool/array2D.hpp"
#include <iostream>
#include <vector>

int main() {
    // Empty array
    ctool::array::Array2D<double> arr1;

    // Fixed size (3x4) initialized to 0.0
    ctool::array::Array2D<double> arr2(3, 4); 

    // Fixed size with custom value
    ctool::array::Array2D<int> arr3(2, 2, 10);

    // From vector of vectors
    std::vector<std::vector<int>> data = {{1, 2}, {3, 4}, {5, 6}};
    ctool::array::Array2D<int> arr4(data);

    // Print
    std::cout << "arr4: " << arr4 << "\n";

    return 0;
}
```

### 2. Access and Modification

```cpp
ctool::array::Array2D<double> matrix(3, 3);

// Safe access (throws std::out_of_range if out of bounds)
matrix.at(0, 0) = 1.0; 

// Fast unchecked access
matrix(1, 1) = 2.5;

// Resize (preserves existing data, fills new cells with 0.0)
matrix.resize(4, 4, 0.0); 

// Push a new row
matrix.pushRow(4, -1.0);
```

### 3. Math & Transformations

```cpp
ctool::array::Array2D<double> data(2, 3);
data.fill(1.0);

// Transpose: 2x3 -> 3x2
ctool::array::Array2D<double> T = data.transpose();

// Sub-array extraction: top-left (0,0), 2 rows, 2 columns
ctool::array::Array2D<double> sub = data.subArray(0, 0, 2, 2);

// Math chain
data.scale(2.0).add(5.0); // (1.0 * 2.0) + 5.0 = 7.0

// Get min/max
double max_val = data.max(); // 7.0
double min_val = data.min(); // 7.0
```

### 4. Search and Analysis

```cpp
ctool::array::Array2D<int> grid(5, 5, 0);
grid(2, 3) = 42;

// Find first occurrence
try {
    std::pair<size_t, size_t> pos = grid.findValue(42);
    // pos.first = 2, pos.second = 3
} catch (const std::runtime_error& e) {
    // Value not found
}

// Find nearest value (useful for floating point or approximations)
auto nearest = grid.findNearest(40); // Returns (2, 3) as 42 is closest to 40
```

### 5. Conversion & Interop

```cpp
ctool::array::Array2D<int> arr(2, 2);

// Convert back to standard vector for compatibility
std::vector<std::vector<int>> vec = arr.toVector();

// Get raw pointer for C-style API interaction (BLAS, LAPACK, etc.)
int* raw_ptr = arr.data();
```

## Performance Notes

- **Access**: `at(row, col)` is $O(1)$ with bounds checking. `operator()(row, col)` is $O(1)$ without checks.
- **Memory**: Uses a flat `std::vector<T>` for contiguous storage. This ensures high cache-locality and performance.
- **Conversion**: `toVector()` returns a copy. For direct row access, prefer `sliceRow(row)`.

## Error Handling

- **`std::out_of_range`**: Thrown by `at()`, `subArray()`, `sliceRow()`, etc., if indices exceed dimensions.
- **`std::invalid_argument`**: Thrown during construction or `pushRow()` if dimensions are inconsistent.
- **`std::runtime_error`**: Thrown by `findValue()` or `max()`/`min()` if the array is empty or value is missing.
