doc/ct_math.md
# CTool Math Module

This module provides statistical utilities for numerical analysis, linear modeling, and data processing.

## Functions

| Function            | Description                               | Parameters          | Returns            |
| :------------------ | :---------------------------------------- | :------------------ | :----------------- |
| `linearFit`         | Performs linear least-squares regression. | `x`, `y` vectors    | `RegResult` struct |
| `mean`              | Calculates the arithmetic mean.           | `data` vector       | `double`           |
| `standardDeviation` | Calculates the standard deviation.        | `data` vector       | `double`           |
| `isNear`            | Checks approximate equality of floats.    | `a`, `b`, `epsilon` | `bool`             |
| `clamp`             | Clamps a value within a range.            | `val`, `min`, `max` | `T` (Template)     |
| **`median`**        | **Calculates the median of a dataset.**   | **`data` vector**   | **`T` (Template)** |

## Usage Examples

### Median Calculation

The `median` function is a template supporting various numeric types (`int`, `double`, `float`, etc.). It requires a copy of the data to sort.

```cpp
#include "ct/ct_math.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<double> data_double = {3.0, 1.0, 4.0, 1.0, 5.0};
    double medD = ct::math::median(data_double); // Returns 3.0

    std::vector<int> data_int = {10, 20, 30};
    int medI = ct::math::median(data_int);       // Returns 20

    std::vector<int> data_even = {1, 2, 3, 4};
    // Returns 2 (truncated from 2.5) or 3 depending on cast logic in implementation
    int medEven = ct::math::median(data_even);   

    return 0;
}
```

## Memory & Performance Notes
- The `median` function takes a `const std::vector<T>&`. It creates an internal copy to perform sorting, so O(N) memory overhead is added per call.
- Sorting complexity is O(N log N).
```

### 5. Update Root `README.md`

Add the new function to the "Function Reference" table in the root documentation.

```markdown
README.md
# CTool / CBridge

... existing content ...

## Core C++ Module (`ct_`)
| Module | Description                             | Header        | Functions                                                             |
| :----- | :-------------------------------------- | :------------ | :-------------------------------------------------------------------- |
| `math` | Statistical and mathematical utilities. | `ct_math.hpp` | `linearFit`, `mean`, `standardDeviation`, `median`, `isNear`, `clamp` |
| ...    | ...                                     | ...           | ...                                                                   |
```

### 6. Update `CHANGELOG.md`

```markdown
CHANGELOG.md
... existing content ...

## [Unreleased]
### Added
- `ct::math::median`: Template function to calculate the median of `std::vector<int>` or `std::vector<double>`.

### Updated
- Documentation: Updated `doc/ct_math.md` and `README.md` with the new `median` function.