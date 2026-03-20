#include "cb_math.h"
#include <math.h>

static cb_reg_result_t impl_linear_fit(const double* x, const double* y, size_t n) {
    cb_reg_result_t res = {0.0, 0.0, 0.0};
    if (n < 2) return res;

    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0;
    for (size_t i = 0; i < n; i++) {
        sumX += x[i];
        sumY += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
        sumY2 += y[i] * y[i];
    }

    double denominator = (n * sumX2 - sumX * sumX);
    if (fabs(denominator) < 1e-12) return res;

    res.slope     = (n * sumXY - sumX * sumY) / denominator;
    res.intercept = (sumY - res.slope * sumX) / n;

    // R-Squared calculation
    double numR = (n * sumXY - sumX * sumY);
    double denR = sqrt((n * sumX2 - sumX * sumX) * (n * sumY2 - sumY * sumY));
    if (fabs(denR) > 1e-12) {
        double r      = numR / denR;
        res.r_squared = r * r;
    }

    return res;
}

static double impl_mean(const double* data, size_t n) {
    if (n == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) sum += data[i];
    return sum / n;
}

static double impl_standard_deviation(const double* data, size_t n) {
    if (n < 2) return 0.0;
    double avg         = impl_mean(data, n);
    double sum_sq_diff = 0.0;
    for (size_t i = 0; i < n; i++) {
        double diff = data[i] - avg;
        sum_sq_diff += diff * diff;
    }
    return sqrt(sum_sq_diff / (n - 1));
}

static bool impl_is_near(double a, double b, double epsilon) { return fabs(a - b) <= epsilon; }

static double impl_clamp(double val, double min, double max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

const struct cb_math_namespace cb_math = {.linear_fit         = impl_linear_fit,
                                          .mean               = impl_mean,
                                          .standard_deviation = impl_standard_deviation,
                                          .is_near            = impl_is_near,
                                          .clamp              = impl_clamp};