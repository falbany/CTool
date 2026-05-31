#include "math.h"
#include <math.h>

static cbridge_reg_result_t impl_linearFit(const double* x, const double* y, size_t n) {
    cbridge_reg_result_t res = {0.0, 0.0, 0.0};
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

static cbridge_reg_result_t impl_logLinearFit(const double* x, const double* y, size_t n) {
    cbridge_reg_result_t res = {0.0, 0.0, 0.0};
    if (n < 2) return res;

    // Check for non-positive y values
    for (size_t i = 0; i < n; i++) {
        if (y[i] <= 0.0) return res;
    }

    double sumX = 0, sumYlog = 0, sumXYlog = 0, sumX2 = 0, sumY2log = 0;
    for (size_t i = 0; i < n; i++) {
        double yLog = log(y[i]);
        sumX += x[i];
        sumYlog += yLog;
        sumXYlog += x[i] * yLog;
        sumX2 += x[i] * x[i];
        sumY2log += yLog * yLog;
    }

    double denominator = (n * sumX2 - sumX * sumX);
    if (fabs(denominator) < 1e-12) return res;

    res.slope     = (n * sumXYlog - sumX * sumYlog) / denominator;
    res.intercept = (sumYlog - res.slope * sumX) / n;

    // R-Squared calculation on log data
    double numR = (n * sumXYlog - sumX * sumYlog);
    double denR = sqrt((n * sumX2 - sumX * sumX) * (n * sumY2log - sumYlog * sumYlog));
    if (fabs(denR) > 1e-12) {
        double r      = numR / denR;
        res.r_squared = r * r;
    }

    return res;
}

static void impl_residuals(const double* x, const double* y, size_t n, double slope, double intercept, double* out) {
    if (!x || !y || !out) return;
    for (size_t i = 0; i < n; i++) {
        double predicted = (slope * x[i]) + intercept;
        out[i]           = y[i] - predicted;
    }
}

static double impl_min(const double* data, size_t n) {
    if (n == 0) return 0.0;
    double m = data[0];
    for (size_t i = 1; i < n; i++) {
        if (data[i] < m) m = data[i];
    }
    return m;
}

static double impl_max(const double* data, size_t n) {
    if (n == 0) return 0.0;
    double m = data[0];
    for (size_t i = 1; i < n; i++) {
        if (data[i] > m) m = data[i];
    }
    return m;
}

static double impl_mean(const double* data, size_t n) {
    if (n == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) sum += data[i];
    return sum / n;
}

static double impl_variance(const double* data, size_t n) {
    if (n < 2) return 0.0;
    double avg    = impl_mean(data, n);
    double sum_sq = 0.0;
    for (size_t i = 0; i < n; i++) {
        double diff = data[i] - avg;
        sum_sq += diff * diff;
    }
    return sum_sq / n;
}

static double impl_standardDeviation(const double* data, size_t n) {
    if (n < 2) return 0.0;
    return sqrt(impl_variance(data, n));
}

static bool impl_isNear(double a, double b, double epsilon) { return fabs(a - b) <= epsilon; }

static double impl_clamp(double val, double min, double max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

static cbridge_sigma_bounds_t impl_calculateSigmaBounds(const double* data, size_t n, double sigma) {
    cbridge_sigma_bounds_t res = {0.0, 0.0};
    if (!data || n < 2) return res;

    double avg     = cbridge_math.mean(data, n);
    double std_dev = cbridge_math.standardDeviation(data, n);

    res.lower_bound = avg - (sigma * std_dev);
    res.upper_bound = avg + (sigma * std_dev);

    return res;
}

const struct cbridge_math_namespace cbridge_math = {.linearFit            = impl_linearFit,
                                                    .logLinearFit         = impl_logLinearFit,
                                                    .residuals            = impl_residuals,
                                                    .min                  = impl_min,
                                                    .max                  = impl_max,
                                                    .mean                  = impl_mean,
                                                    .variance             = impl_variance,
                                                    .standardDeviation    = impl_standardDeviation,
                                                    .isNear               = impl_isNear,
                                                    .clamp                 = impl_clamp,
                                                    .calculateSigmaBounds = impl_calculateSigmaBounds};

LIBCTOOL_API cbridge_sigma_bounds_t calculateSigmaLowerBound(const double* data, size_t size, double sigma_multiplier) {
    return cbridge_math.calculateSigmaBounds(data, size, sigma_multiplier);
}

LIBCTOOL_API cbridge_sigma_bounds_t calculateSigmaUpperBound(const double* data, size_t size, double sigma_multiplier) {
    return cbridge_math.calculateSigmaBounds(data, size, sigma_multiplier);
}