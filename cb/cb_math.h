/**
 * @file cb_math.h
 * @author Florent ALBANY - FAL
 * @brief Numerical utilities for statistical analysis and linear modeling in C.
 * @version 1.0
 * @date 2026-03-20
 */

#ifndef CB_MATH_H
#define CB_MATH_H

#include <stddef.h>
#include <stdbool.h>
#include "../libctool.h"

/**
 * @struct cb_reg_result
 * @brief Result structure for linear regression calculations.
 */
typedef struct {
    double slope;        ///< The slope of the line (m).
    double intercept;    ///< The Y-intercept of the line (b).
    double r_squared;    ///< Coefficient of determination.
} cb_reg_result_t;

/**
 * @struct cb_math_namespace
 * @brief Function pointer table acting as the "cb::math" namespace.
 */
struct cb_math_namespace {
    /**
     * @brief Performs a linear least-squares fit on a dataset.
     * @param x Array of independent values.
     * @param y Array of dependent values.
     * @param size Number of elements in arrays.
     * @return cb_reg_result_t containing the calculated model.
     */
    cb_reg_result_t (*linear_fit)(const double* x, const double* y, size_t size);

    /**
     * @brief Calculates the arithmetic mean (average) of a dataset.
     */
    double (*mean)(const double* data, size_t size);

    /**
     * @brief Calculates the standard deviation of a dataset.
     */
    double (*standard_deviation)(const double* data, size_t size);

    /**
     * @brief Checks if two values are approximately equal within a tolerance.
     */
    bool (*is_near)(double a, double b, double epsilon);

    /**
     * @brief Clamps a value within a specified range.
     */
    double (*clamp)(double val, double min, double max);
};

LIBCTOOL_API extern const struct cb_math_namespace cb_math;

#endif    // CB_MATH_H