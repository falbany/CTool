# CBridge Math Module

This module provides a comprehensive suite of statistical utilities and linear modeling functions in pure C. It uses a "namespace" pattern to group functions under the global `cbridge_math` object.

## Data Structures

### `cbridge_reg_result_t`

```c
typedef struct {
    double slope;        ///< The slope of the line (m).
    double intercept;    ///< The Y-intercept of the line (b).
    double r_squared;    ///< Coefficient of determination.
} cbridge_reg_result_t;
```

### `cbridge_sigma_bounds_t`

```c
typedef struct {
    double lower_bound;    ///< The lower bound (mean - sigma * std_dev).
    double upper_bound;    ///< The upper bound (mean + sigma * std_dev).
} cbridge_sigma_bounds_t;
```

## API Reference

### Basic Statistics

| Function                   | Description                                                                         | Signature                                                                                                   |
| :------------------------- | :---------------------------------------------------------------------------------- | :---------------------------------------------------------------------------------------------------------- |
| `min`                      | Minimum value.                                                                      | `double (*min)(const double* data, size_t size)`                                                            |
| `max`                      | Maximum value.                                                                      | `double (*max)(const double* data, size_t size)`                                                            |
| `mean`                     | Arithmetic mean (average).                                                          | `double (*mean)(const double* data, size_t size)`                                                           |
| `variance`                 | Population variance ($\sigma^2$).                                                   | `double (*variance)(const double* data, size_t size)`                                                       |
| `standardDeviation`        | Population standard deviation ($\sigma$).                                           | `double (*standardDeviation)(const double* data, size_t size)`                                              |
| `isNear`                   | Checks if two values are approximately equal within a tolerance.                    | `bool (*isNear)(double a, double b, double epsilon)`                                                        |
| `clamp`                    | Clamps a value within a specified range.                                            | `double (*clamp)(double val, double min, double max)`                                                       |
| `calculateSigmaBounds`     | Calculates lower/upper bounds based on a sigma multiplier (empirical rule).         | `cbridge_sigma_bounds_t (*calculateSigmaBounds)(const double* data, size_t size, double sigma_multiplier)`  |
| `calculateSigmaLowerBound` | Alias: calls `calculateSigmaBounds` and returns the same result (for API symmetry). | `cbridge_sigma_bounds_t calculateSigmaLowerBound(const double* data, size_t size, double sigma_multiplier)` |
| `calculateSigmaUpperBound` | Alias: calls `calculateSigmaBounds` and returns the same result (for API symmetry). | `cbridge_sigma_bounds_t calculateSigmaUpperBound(const double* data, size_t size, double sigma_multiplier)` |

### Modeling

| Function       | Description                                                     | Signature                                                                                                                 |
| :------------- | :-------------------------------------------------------------- | :------------------------------------------------------------------------------------------------------------------------ |
| `linearFit`    | Linear least-squares regression ($y = mx + c$).                 | `cbridge_reg_result_t (*linearFit)(const double* x, const double* y, size_t size)`                                        |
| `logLinearFit` | Exponential regression ($y = A e^{Bx}$) via log transformation. | `cbridge_reg_result_t (*logLinearFit)(const double* x, const double* y, size_t size)`                                     |
| `residuals`    | Error analysis for a given linear model.                        | `void (*residuals)(const double* x, const double* y, size_t size, double slope, double intercept, double* out_residuals)` |

---

## Usage Examples

### 1. Basic Statistics

```c
#include "CBridge.h"
#include <stdio.h>

int main() {
    double data[] = {2.0, 8.0, 3.0, 9.0, 1.0};
    size_t n = 5;

    double minimum = cbridge_math.min(data, n);     // 1.0
    double maximum = cbridge_math.max(data, n);     // 9.0
    double average = cbridge_math.mean(data, n);    // 4.6
    double variance = cbridge_math.variance(data, n);   // 8.96 (Population)
    double stddev = cbridge_math.standardDeviation(data, n); // ~2.99

    printf("Min: %.1f, Max: %.1f\n", minimum, maximum);
    printf("Mean: %.1f, StdDev: %.2f\n", average, stddev);
    printf("Variance: %.2f\n", variance);

    return 0;
}
```

### 2. Sigma Bounds (Empirical Rule)

```c
#include "CBridge.h"
#include <stdio.h>

int main() {
    double data[] = {10.0, 20.0, 30.0};
    size_t n = 3;

    // 1-sigma bounds (~68% of data within this range for a normal distribution)
    cbridge_sigma_bounds_t bounds = cbridge_math.calculateSigmaBounds(data, n, 1.0);
    printf("1-Sigma — Lower: %.4f, Upper: %.4f\n", bounds.lower_bound, bounds.upper_bound);

    // 2-sigma bounds (~95%)
    bounds = cbridge_math.calculateSigmaBounds(data, n, 2.0);
    printf("2-Sigma — Lower: %.4f, Upper: %.4f\n", bounds.lower_bound, bounds.upper_bound);

    // Using alias functions
    cbridge_sigma_bounds_t lb = calculateSigmaLowerBound(data, n, 2.0);
    cbridge_sigma_bounds_t ub = calculateSigmaUpperBound(data, n, 2.0);
    printf("Alias — Lower: %.4f, Upper: %.4f\n", lb.lower_bound, ub.upper_bound);

    return 0;
}
```

### 3. Linear Regression

```c
#include "CBridge.h"
#include <stdio.h>

int main() {
    double x[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double y[] = {2.1, 4.0, 5.9, 8.1, 9.9};
    size_t n = 5;

    cbridge_reg_result_t result = cbridge_math.linearFit(x, y);

    printf("Slope: %.4f\n", result.slope);
    printf("Intercept: %.4f\n", result.intercept);
    printf("R-Squared: %.4f\n", result.r_squared);

    return 0;
}
```

### 4. Residuals Analysis

```c
#include "CBridge.h"
#include <stdio.h>

int main() {
    double x[] = {1.0, 2.0, 3.0};
    double y[] = {2.1, 3.9, 6.2};
    size_t n = 3;
    double residuals[3];

    cbridge_math.residuals(x, y, n, 2.0, 0.0, residuals);

    for (size_t i = 0; i < n; i++) {
        printf("Residual[%zu] = %.6f\n", i, residuals[i]);
    }

    return 0;
}
```

---

## Performance & Memory Notes

| Function                        | Complexity | Memory Overhead | Notes                                               |
| :------------------------------ | :--------- | :-------------- | :-------------------------------------------------- |
| `min`, `max`                    | $O(N)$     | $O(1)$          | Single pass.                                        |
| `mean`                          | $O(N)$     | $O(1)$          | Single pass.                                        |
| `variance`, `standardDeviation` | $O(N)$     | $O(1)$          | Two passes (mean + deviation).                      |
| `clamp`                         | $O(1)$     | $O(1)$          | Direct comparison.                                  |
| `linearFit`                     | $O(N)$     | $O(1)$          | Single pass accumulation.                           |
| `logLinearFit`                  | $O(N)$     | $O(1)$          | Single pass with `log()` per element.               |
| `residuals`                     | $O(N)$     | $O(1)$          | Single pass, output buffer pre-allocated by caller. |
| `calculateSigmaBounds`          | $O(N)$     | $O(1)$          | Delegates to `mean` and `standardDeviation`.        |

## Error Handling & Edge Cases

- **Empty Arrays / NULL**: Functions returning scalar values (`mean`, `min`, `max`, `variance`) return `0.0`. Struct results are zeroed.
- **Single Element**: `variance`, `standardDeviation`, and `calculateSigmaBounds` return `0.0` / zeroed struct for arrays with fewer than 2 elements.
- **Non-positive y**: `logLinearFit` returns a zeroed result if any `y[i] <= 0`.
- **Perfect collinearity**: `linearFit` and `logLinearFit` return a zeroed result if the denominator in slope calculation is near zero.
- **NULL Pointers**: Modeling functions guard against NULL input pointers and return zeroed result structs.
