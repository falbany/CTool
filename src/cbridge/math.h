/**
 * @file math.h
 * @author Florent ALBANY - FAL
 * @brief Numerical utilities for statistical analysis and linear modeling in C.
 * @version 1.0
 * @date 2026-03-20
 */

#ifndef CBRIDGE_MATH_H
#define CBRIDGE_MATH_H

#include <stddef.h>
#include <stdbool.h>
#include "../internal/libctool.h"

/**
 * @struct cbridge_reg_result
 * @brief Result structure for linear regression calculations.
 */
typedef struct {
    double slope;        ///< The slope of the line (m).
    double intercept;    ///< The Y-intercept of the line (b).
    double r_squared;    ///< Coefficient of determination.
} cbridge_reg_result_t;

/**
 * @struct cbridge_sigma_bounds
 * @brief Result structure for sigma bounds calculations.
 */
typedef struct {
    double lower_bound;    ///< The lower bound (mean - sigma * std_dev).
    double upper_bound;    ///< The upper bound (mean + sigma * std_dev).
} cbridge_sigma_bounds_t;

/**
 * @struct cbridge_math_namespace
 * @brief Function pointer table acting as the "cbridge::math" namespace.
 */
struct cbridge_math_namespace {
    /**
     * @brief Performs a linear least-squares fit on a dataset.
     * @param x Array of independent values.
     * @param y Array of dependent values.
     * @param size Number of elements in arrays.
     * @return cbridge_reg_result_t containing the calculated model.
     */
    cbridge_reg_result_t (*linearFit)(const double* x, const double* y, size_t size);

    /**
     * @brief Performs linear least-squares regression on logarithmically transformed data.
     * @note Fits the model y = A * e^(B * x) by solving ln(y) = B * x + ln(A).
     *       Returns empty result if any y <= 0.
     * @param x Array of independent values.
     * @param y Array of dependent values (must be strictly positive).
     * @param size Number of elements in arrays.
     * @return cbridge_reg_result_t where:
     *         - slope corresponds to B (exponent coefficient).
     *         - intercept corresponds to ln(A).
     *         - r_squared is the goodness of fit for the log-transformed data.
     */
    cbridge_reg_result_t (*logLinearFit)(const double* x, const double* y, size_t size);

    /**
     * @brief Calculates residuals (errors) for a linear model.
     * @param x Array of independent values.
     * @param y Array of actual dependent values.
     * @param size Number of elements in arrays.
     * @param slope Slope of the linear model (m).
     * @param intercept Y-intercept of the linear model (c).
     * @param residuals Output array for residuals: y_actual - y_predicted.
     *                  Must be pre-allocated with 'size' elements.
     */
    void (*residuals)(const double* x, const double* y, size_t size, double slope, double intercept, double* out_residuals);

    /**
     * @brief Calculates the minimum value in a dataset.
     */
    double (*min)(const double* data, size_t size);

    /**
     * @brief Calculates the maximum value in a dataset.
     */
    double (*max)(const double* data, size_t size);

    /**
     * @brief Calculates the arithmetic mean (average) of a dataset.
     */
    double (*mean)(const double* data, size_t size);

    /**
     * @brief Calculates the variance of a dataset.
     */
    double (*variance)(const double* data, size_t size);

    /**
     * @brief Calculates the standard deviation of a dataset.
     */
    double (*standardDeviation)(const double* data, size_t size);

    /**
     * @brief Checks if two values are approximately equal within a tolerance.
     */
    bool (*isNear)(double a, double b, double epsilon);

    /**
     * @brief Clamps a value within a specified range.
     */
    double (*clamp)(double val, double min, double max);

    /**
     * @brief Calculates the lower and upper bounds of a dataset based on a sigma multiplier.
     * @param data Array of numerical values.
     * @param size Number of elements in the array.
     * @param sigma_multiplier The multiplier for the standard deviation (e.g., 2.0 for 95%).
     * @return cbridge_sigma_bounds_t containing the lower and upper bounds.
     */
    cbridge_sigma_bounds_t (*calculateSigmaBounds)(const double* data, size_t size, double sigma_multiplier);
};

LIBCTOOL_API cbridge_sigma_bounds_t calculateSigmaLowerBound(const double* data, size_t size, double sigma_multiplier);
LIBCTOOL_API cbridge_sigma_bounds_t calculateSigmaUpperBound(const double* data, size_t size, double sigma_multiplier);

LIBCTOOL_API extern const struct cbridge_math_namespace cbridge_math;

#endif    // CBRIDGE_MATH_H