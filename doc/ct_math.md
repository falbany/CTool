doc/ct_math.md
# CTool Math Module

This module provides a comprehensive suite of statistical utilities, data preprocessing tools, and mathematical operations for numerical analysis and linear modeling.

## Function Reference

### Basic Statistics (Non-Template)
These functions operate on `std::vector<double>` and return `double` results.

| Function            | Description                               | Signature                                                   |
| :------------------ | :---------------------------------------- | :---------------------------------------------------------- |
| `mean`              | Arithmetic mean (average).                | `double mean(const std::vector<double>& data)`              |
| `min`               | Minimum value.                            | `double min(const std::vector<double>& data)`               |
| `max`               | Maximum value.                            | `double max(const std::vector<double>& data)`               |
| `variance`          | Population variance ($\sigma^2$).         | `double variance(const std::vector<double>& data)`          |
| `standardDeviation` | Population standard deviation ($\sigma$). | `double standardDeviation(const std::vector<double>& data)` |
| `isNear`            | Checks floating-point equality.           | `bool isNear(double a, double b, double epsilon)`           |

### Advanced Statistics & Preprocessing (Template)
Generic functions supporting `int`, `double`, `float`, etc.

| Function      | Description                           | Signature                                           |
| :------------ | :------------------------------------ | :-------------------------------------------------- |
| `sum`         | Sum of all elements.                  | `template <T> T sum(const std::vector<T>& data)`    |
| `median`      | Median value (middle element).        | `template <T> T median(const std::vector<T>& data)` |
| `percentile`  | $k$-th percentile with interpolation. | `template <T> double percentile(..., double k)`     |
| `minMaxScale` | Normalize data to [0, 1].             | `template <T> std::vector<double> minMaxScale(...)` |
| `clamp`       | Constrain value to range [min, max].  | `template <T> T clamp(T val, T min, T max)`         |

### Modeling
| Function        | Description                                     | Signature                                                                                                                   |
| :-------------- | :---------------------------------------------- | :-------------------------------------------------------------------------------------------------------------------------- |
| `linearFit`     | Linear least-squares regression ($y = mx + c$). | `RegResult linearFit(const std::vector<double>& x, const std::vector<double>& y)`                                           |
| `logLinearFit`  | Exponential regression ($y = A e^{Bx}$).        | `RegResult logLinearFit(const std::vector<double>& x, const std::vector<double>& y)`                                        |
| `polynomialFit` | Polynomial regression (Degree 1–4).             | `std::vector<double> polynomialFit(const std::vector<double>& x, const std::vector<double>& y, int degree)`                 |
| `residuals`     | Error analysis for a given linear model.        | `std::vector<double> residuals(const std::vector<double>& x, const std::vector<double>& y, double slope, double intercept)` |

---

## Usage Examples

### 1. Basic Statistics (Double Vector)
```cpp
#include "ct/ct_math.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<double> data = {2.0, 8.0, 3.0, 9.0, 1.0};

    // Extremes
    double minimum = ct::math::min(data);  // 1.0
    double maximum = ct::math::max(data);  // 9.0

    // Central Tendency
    double average = ct::math::mean(data); // 4.6

    // Dispersion
    double variance = ct::math::variance(data);      // 8.96 (Population)
    double stddev = ct::math::standardDeviation(data); // 2.99

    std::cout << "Min: " << minimum << ", Max: " << maximum << "\n";
    std::cout << "Mean: " << average << ", StdDev: " << stddev << "\n";
    std::cout << "Variance: " << variance << "\n";

    return 0;
}
```

### 2. Template Functions (Generic Types)
These functions work with `int`, `double`, `float`, etc.

```cpp
#include "ct/ct_math.hpp"
#include <vector>
#include <iostream>

int main() {
    // --- Sum ---
    std::vector<int> int_data = {1, 2, 3, 4, 5};
    int sum_int = ct::math::sum(int_data); // 15

    std::vector<double> double_data = {1.5, 2.5, 3.0};
    double sum_double = ct::math::sum(double_data); // 7.0

    // --- Median (Handles both odd and even lengths) ---
    std::vector<int> odd_median = {10, 20, 30};          // Returns 20
    int medOdd = ct::math::median(odd_median);

    std::vector<int> even_median = {1, 2, 3, 4};         // Returns 2 (truncated from 2.5)
    int medEven = ct::math::median(even_median);

    // --- Percentile (Linear Interpolation) ---
    std::vector<int> scores = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    double p50 = ct::math::percentile(scores, 50.0); // 55.0
    double p90 = ct::math::percentile(scores, 90.0); // 91.0

    std::cout << "Sum (Int): " << sum_int << ", Sum (Double): " << sum_double << "\n";
    std::cout << "Median (Odd): " << medOdd << ", Median (Even): " << medEven << "\n";
    std::cout << "Percentile 50%: " << p50 << ", 90%: " << p90 << "\n";

    return 0;
}
```

### 3. Data Preprocessing: Min-Max Scaling
Normalizes data to the [0, 1] range. Useful for machine learning inputs.

```cpp
#include "ct/ct_math.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<int> raw_data = {20, 25, 30, 35, 40};

    // Returns a new vector of doubles scaled to [0, 1]
    std::vector<double> scaled = ct::math::minMaxScale(raw_data);

    std::cout << "Scaled Data: ";
    for (double val : scaled) {
        std::cout << val << " "; // Output: 0.0 0.25 0.5 0.75 1.0
    }
    std::cout << "\n";

    return 0;
}
```

### 4. Linear Regression
Fits a line to x/y data points.

```cpp
#include "ct/ct_math.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<double> x = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> y = {2.1, 4.0, 5.9, 8.1, 9.9};

    ct::math::RegResult result = ct::math::linearFit(x, y);

    std::cout << "Slope: " << result.slope << "\n";
    std::cout << "Intercept: " << result.intercept << "\n";
    std::cout << "R-Squared: " << result.rSquared << "\n";

    return 0;
}
```

## Advanced Modeling Functions

### Exponential Regression (Log-Linear)
Fits $y = A e^{Bx}$ by performing linear regression on $\ln(y)$.

```cpp
#include "ct/ct_math.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<double> x = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> y = {2.7, 7.4, 20.1, 54.6, 148.4}; // approx exp(1.1x)

    ct::math::RegResult res = ct::math::logLinearFit(x, y);

    // Model: y = exp(intercept) * exp(slope * x)
    double A = std::exp(res.intercept);
    double B = res.slope;

    std::cout << "Model: y = " << A << " * exp(" << B << " * x)\n";
    std::cout << "R-Squared: " << res.rSquared << "\n";

    return 0;
}
```

### Residual Analysis
Calculates the difference between actual and predicted values for a given linear model.

```cpp
#include "ct/ct_math.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<double> x = {1.0, 2.0, 3.0};
    std::vector<double> y = {2.0, 4.0, 6.5}; // Slight noise
    double m = 2.0;
    double c = 0.0;

    std::vector<double> resid = ct::math::residuals(x, y, m, c);

    std::cout << "Residuals: ";
    for (double r : resid) std::cout << r << " "; // 0.0, 0.0, 0.5
    std::cout << "\n";

    return 0;
}
```

### Polynomial Fitting
Fits a polynomial $y = a_0 + a_1 x + \dots + a_n x^n$. Supports degrees 1 to 4.

```cpp
#include "ct/ct_math.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<double> x = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> y = {2.1, 4.9, 12.2, 20.1, 30.0}; // approx y = x^2 + 1

    // Fit quadratic (degree 2)
    std::vector<double> coeffs = ct::math::polynomialFit(x, y, 2);

    // Coefficients: [a0, a1, a2] -> y = a0 + a1*x + a2*x^2
    std::cout << "Coefficients: ";
    for (double c : coeffs) std::cout << c << " "; // approx 1.0 0.0 1.0
    std::cout << "\n";

    return 0;
}
```

---

## Performance & Memory Notes

| Function             | Complexity    | Memory Overhead | Notes                                        |
| :------------------- | :------------ | :-------------- | :------------------------------------------- |
| `min`, `max`         | $O(N)$        | $O(1)$          | Single pass.                                 |
| `mean`, `sum`        | $O(N)$        | $O(1)$          | Single pass.                                 |
| `variance`, `stdDev` | $O(N)$        | $O(1)$          | Two passes (mean + deviation).               |
| `clamp`              | $O(1)$        | $O(1)$          | Direct comparison.                           |
| `median`             | $O(N \log N)$ | $O(N)$          | Requires a copy for sorting.                 |
| `percentile`         | $O(N \log N)$ | $O(N)$          | Requires a copy for sorting + interpolation. |
| `minMaxScale`        | $O(N)$        | $O(N)$          | Returns a new vector.                        |
| `linearFit`          | $O(N)$        | $O(1)$          | Single pass accumulation.                    |

## Error Handling & Edge Cases

- **Empty Vectors**: Functions returning scalar values (`mean`, `sum`, `min`, `max`, `variance`) return `0.0` (or `T(0)` for templates).
- **Single Element**: `variance` and `standardDeviation` return `0.0` for vectors with fewer than 2 elements.
- **Invalid Percentile**: `percentile` throws `std::invalid_argument` if $k < 0.0$ or $k > 100.0$.
- **Division by Zero**: `minMaxScale` handles constant data (max == min) by returning a vector of `0.0`.
- **Integer Precision**: When calculating statistics for `int` vectors using template functions (like `median`), results are cast back to `T`. For accurate fractional results (e.g., median of `{1, 2}`), use `std::vector<double>` inputs.