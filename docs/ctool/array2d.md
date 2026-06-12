# CTool Array2D Module

This module provides a type-safe, dynamic 2D array container (`ctool::Array2D<T>`). It offers bounds-checked access, dimension management, and basic mathematical operations, acting as a robust alternative to `std::vector<std::vector<T>>`.

## Class Reference: `ctool::array::Array2D<T>`

### Features
- **Safety**: Bounds-checked access via `at(row, col)`.
- **Flexibility**: Resizable dimensions, dynamic row addition.
- **Math**: In-place scaling and addition operations.
- **Performance**: High-level abstraction over `std::vector<std::vector<T>>`.

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
    std::cout << "arr2: " << arr2 << "\n";
    std::cout << "arr3: " << arr3 << "\n";
    std::cout << "arr4: " << arr4 << "\n";

    return 0;
}
```

### 2. Access and Modification

```cpp
ctool::array::Array2D<double> matrix(3, 3);

// Safe access (throws if out of bounds)
matrix.at(0, 0) = 1.0; 

// Fast unchecked access
matrix(1, 1) = 2.5;

// Resize
matrix.resize(4, 4, 0.0); // Expands to 4x4, filling new cells with 0.0

// Push a new row
matrix.pushRow(4, 0.0);
```

### 3. Math Operations

```cpp
ctool::array::Array2D<double> data(2, 2);
data.fill(1.0); // Fills with 1.0

// Scale by 2.0 -> all elements become 2.0
data.scale(2.0); 

// Add 5.0 -> all elements become 7.0
data.add(5.0); 

// Get min/max
double max_val = data.max(); // 7.0
double min_val = data.min(); // 7.0
```

### 4. Conversion

```cpp
ctool::array::Array2D<int> arr(2, 2);
// ... fill data ...

// Convert back to standard vector for compatibility
std::vector<std::vector<int>> vec = arr.toVector();
```

## Performance Notes

- **Access**: `at(row, col)` is $O(1)$ but includes bounds checking (slower). `operator()(row, col)` is $O(1)$ with no checks (faster).
- **Resize**: $O(N)$ where $N$ is the number of elements.
- **Memory**: Uses a flat `std::vector<T>` for contiguous storage. This ensures high cache-locality and performance, making it suitable for linear algebra and interfacing with C-style APIs via `data()`.
- **Note**: `toVector()` returns a copy. For read-only access to rows, prefer `sliceRow()`.

## Error Handling

- **`std::out_of_range`**: Thrown by `at()` if indices exceed dimensions.
- **`std::invalid_argument`**: Thrown during construction if input vectors are jagged (inconsistent row lengths).
- **`std::runtime_error`**: Thrown by `min()`/`max()` on empty arrays.

