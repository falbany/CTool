# cb_math Module

The `cb_math` module provides high-performance numerical utilities in pure C. It is designed to mirror the `ct::math` C++ namespace, offering statistical analysis and linear modeling tools.

## API Reference

### Structures
- **`cb_reg_result_t`**: Stores `slope`, `intercept`, and `r_squared`.

### Functions
| Function                         | Description                               |
| :------------------------------- | :---------------------------------------- |
| `linear_fit(x, y, size)`         | Performs linear least-squares regression. |
| `mean(data, size)`               | Calculates the arithmetic average.        |
| `standard_deviation(data, size)` | Calculates the sample standard deviation. |
| `is_near(a, b, epsilon)`         | Safe floating-point comparison.           |
| `clamp(val, min, max)`           | Constrains a value within a range.        |

## Usage Example

### Linear Regression
```c
double volt[] = {0.0, 1.0, 2.0, 3.0, 4.0};
double curr[] = {0.0, 0.02, 0.041, 0.059, 0.082};

cb_reg_result_t res = cb_math.linear_fit(volt, curr, 5);

printf("Model: y = %.3fx + %.3f\n", res.slope, res.intercept);
printf("Precision (R2): %.4f\n", res.r_squared);
```

### Statistics
```c
double samples[] = {10.2, 10.5, 9.8, 10.1};
double avg = cb_math.mean(samples, 4);
double sigma = cb_math.standard_deviation(samples, 4);
```

---

## 🛠 Integration Note
To use this module through the unified CBridge interface, ensure your `CBridge.h` includes the reference:
```c
struct CBridge_Namespace {
    // ... other modules
    const struct cb_math_namespace* math;
};
```
