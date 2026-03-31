# ct_num: NumPy-like Array Operations

## 📖 Overview
The `ct_num` module provides a high-performance, contiguous memory 2D array container designed to mimic the syntax and behavior of Python's **NumPy** library. Unlike the legacy `ct_array2d` (which uses a "vector of vectors"), `NumArray` stores data in a single contiguous block, offering superior cache locality and performance for numerical computations.

## ✨ Key Features
- **Contiguous Memory Layout:** Data is stored in a single `std::vector<T>`, ensuring cache-friendly access patterns similar to NumPy's `ndarray`.
- **Element-wise Operators:** Supports natural C++ syntax for arithmetic (`+`, `-`, `*`, `/`) with both arrays and scalars.
- **Universal Functions (Ufuncs):** Math functions (`sqrt`, `sin`, `abs`) operate element-wise on the entire array.
- **Reduction Operations:** Aggregation functions like `sum`, `mean`, `min`, `max`.
- **Type Safety:** Fully templated (`<typename T>`) to support `double`, `int`, `float`, etc., with compile-time type checking.

## 🚀 Quick Start

### 1. Inclusion
```cpp
#include "ct/ct_num.hpp"
```

### 2. Construction
```cpp
// Create arrays using the full namespace path
ct::num::NumArray<double> empty;

ct::num::NumArray<int> zeros(3, 4); 

ct::num::NumArray<double> matrix = {{1.0, 2.0, 3.0}, 
                                   {4.0, 5.0, 6.0}};

// Check shape and size
size_t rows = matrix.rows();      // 2
size_t cols = matrix.cols();      // 3
size_t total = matrix.size();     // 6
auto shape = matrix.shape();      // {2, 3}
```

### 3. Element Access
```cpp
// Checked access (throws if out of bounds)
matrix.at(0, 1) = 100.0;

// Unchecked access (faster, no bounds check)
matrix(1, 2) = 200.0;

// Raw pointer access (for interoperability with C APIs)
double* data_ptr = matrix.data(); 
```

### 4. Arithmetic (Element-wise)
```cpp
ct::num::NumArray<double> a = {{1.0, 2.0}, {3.0, 4.0}};
ct::num::NumArray<double> b = {{5.0, 6.0}, {7.0, 8.0}};

// Array + Array (Shapes must match)
ct::num::NumArray<double> sum_ab = a + b; 
// Result: {{6.0, 8.0}, {10.0, 12.0}}

// Array + Scalar (Broadcasting)
ct::num::NumArray<double> add_5 = a + 5.0; 
// Result: {{6.0, 7.0}, {8.0, 9.0}}

// Scalar + Array (Commutative)
ct::num::NumArray<double> add_5_rev = 5.0 + a;

// Multiplication and Division
ct::num::NumArray<double> scaled = a * 2.0;
ct::num::NumArray<double> divided = b / 2.0;
```

### 5. Universal Functions (Ufuncs)
These functions return a **new** array with the transformed values.
```cpp
ct::num::NumArray<double> data = {{1.0, 4.0}, {9.0, 16.0}};

// Square Root
ct::num::NumArray<double> roots = ct::num::sqrt(data); 
// Result: {{1.0, 2.0}, {3.0, 4.0}}

// Sine (radians)
ct::num::NumArray<double> sine_vals = ct::num::sin(data);

// Absolute Value
ct::num::NumArray<int> negatives = {{-1, -2}, {3, -4}};
ct::num::NumArray<int> pos = ct::num::abs(negatives);
// Result: {{1, 2}, {3, 4}}
```

### 6. Reduction Operations
```cpp
ct::num::NumArray<double> m = {{1.0, 2.0}, {3.0, 4.0}};

double s = ct::num::sum(m);      // 10.0
double mean_val = ct::num::mean(m); // 2.5
double max_val = ct::num::max(m);   // 4.0
double min_val = ct::num::min(m);   // 1.0
```

### 7. Utility
```cpp
// Check if empty
if (m.empty()) {
    // Handle empty array
}

// Clear the array (resets to 0x0)
m.clear(); 

// Output to stream
std::cout << m << std::endl;
// Output:
// NumArray(
//   [1, 2]
//   [3, 4]
// )
```

## 📊 Comparison: `NumArray` vs `Array2D`

| Feature            | `ct::array::Array2D`               | `ct::num::NumArray`                    |
| :----------------- | :--------------------------------- | :------------------------------------- |
| **Memory Layout**  | Vector of Vectors (Non-contiguous) | **Contiguous 1D Block**                |
| **Performance**    | Slower (Cache misses)              | **High (Cache friendly)**              |
| **Operations**     | Scalar only (`scale()`, `add()`)   | **Element-wise (`+`, `*`, etc.)**      |
| **Math Functions** | Manual loops                       | **Built-in Ufuncs (`sqrt`, `sin`)**    |
| **Use Case**       | General grid storage, simple logic | **Scientific computing, Data Science** |

> **Note:** If you are doing heavy numerical calculations, use `ct::num::NumArray`. If you need a simple grid where rows might have different lengths (jagged arrays), use `ct::array::Array2D`.

## ⚠️ Limitations & Notes
1.  **No Views:** Slicing or sub-array operations currently create **copies** of the data (unlike NumPy's zero-copy views).
2.  **Shape Broadcasting:** Currently supports scalar broadcasting only. Array-to-array operations require matching dimensions.
3.  **Matrix Multiplication:** Standard matrix multiplication (`@` operator) is not implemented yet; use element-wise `*` for now.

## 📝 License
Part of the CTool project.

## 📚 References
- [Python NumPy Documentation](https://numpy.org/doc/stable/)
- [C++ Standard Template Library](https://en.cppreference.com/w/cpp/container/vector)
