/**
 * @file ct_math.hpp
 * @author Florent ALBANY - FAL
 * @brief Numerical utilities for statistical analysis and linear modeling.
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#ifndef CT_MATH_HPP
    #define CT_MATH_HPP

    #include <vector>
    #include <cmath>
    #include "../libctool.h"

namespace ct {
    // ------------------------------------------------------------------------
    // MATH API
    // ------------------------------------------------------------------------

    /**
     * @namespace ct::math
     * @brief Math Tool - Calculation and statistics.
     */
    namespace math {

        /**
         * @brief Result structure for linear regression calculations.
         */
        struct RegResult {
            double slope;        ///< The slope of the line (e.g., 1/Ron).
            double intercept;    ///< The Y-intercept of the line.
            double rSquared;     ///< Coefficient of determination (goodness of fit).
        };

        /**
         * @brief Performs a linear least-squares fit on a dataset.
         * @param x Vector of independent values (e.g., Drain-Source Voltage).
         * @param y Vector of dependent values (e.g., Drain-Source Current).
         * @return RegResult containing the calculated model.
         */
        LIBCTOOL_API RegResult linearFit(const std::vector<double>& x, const std::vector<double>& y);

        
        /**
         * @brief Performs linear least-squares regression on logarithmically transformed data.
         * @note Fits the model $y = A \cdot e^{B \cdot x}$ by solving $\ln(y) = B \cdot x + \ln(A)$.
         *       Throws an error if any $y \le 0$.
         * @param x Vector of independent values.
         * @param y Vector of dependent values (must be strictly positive).
         * @return RegResult where:
         *         - slope corresponds to B (exponent coefficient).
         *         - intercept corresponds to $\ln(A)$.
         *         - rSquared is the goodness of fit for the log-transformed data.
         */
        LIBCTOOL_API RegResult logLinearFit(const std::vector<double>& x, const std::vector<double>& y);

        /**
         * @brief Calculates residuals (errors) for a linear model.
         * @param x Vector of independent values.
         * @param y Vector of actual dependent values.
         * @param slope Slope of the linear model ($m$).
         * @param intercept Y-intercept of the linear model ($c$).
         * @return std::vector<double> Vector of residuals: $y_{actual} - y_{predicted}$.
         */
        LIBCTOOL_API std::vector<double> residuals(const std::vector<double>& x, const std::vector<double>& y, double slope, double intercept);

        /**
         * @brief Fits a polynomial of degree $n$ to the data using the Normal Equation.
         * @note Supports degrees 1 to 4. Higher degrees may cause numerical instability 
         *       without iterative solvers or external libraries.
         *       Solves $(X^T X) \beta = X^T y$ using Gaussian elimination.
         * @param x Vector of independent values.
         * @param y Vector of dependent values.
         * @param degree Degree of the polynomial ($1 \le degree \le 4$).
         * @return std::vector<double> Coefficients $[a_0, a_1, ..., a_{degree}]$ 
         *         where $y = a_0 + a_1 x + \dots + a_{degree} x^{degree}$.
         * @return Returns empty vector if input sizes mismatch or degree is invalid.
         * @throw std::invalid_argument if degree is outside [1, 4].
         */
        LIBCTOOL_API std::vector<double> polynomialFit(const std::vector<double>& x, const std::vector<double>& y, int degree);

        /**
         * @brief Calculates the minimum value in a dataset.
         * @note Returns a double. If the input is integral, it is cast to double.
         *       Returns 0.0 if the vector is empty.
         * @param data Vector of numeric values.
         * @return double The minimum value.
         */
        LIBCTOOL_API double min(const std::vector<double>& data);

        /**
         * @brief Calculates the maximum value in a dataset.
         * @note Returns a double. If the input is integral, it is cast to double.
         *       Returns 0.0 if the vector is empty.
         * @param data Vector of numeric values.
         * @return double The maximum value.
         */
        LIBCTOOL_API double max(const std::vector<double>& data);

        /**
         * @brief Calculates the arithmetic mean (average) of a dataset.
         */
        LIBCTOOL_API double mean(const std::vector<double>& data);

        /**
         * @brief Calculates the variance of a dataset (population variance).
         * @note Variance is the square of the standard deviation.
         *       Returns 0.0 if the vector has fewer than 2 elements.
         * @param data Vector of numeric values.
         * @return double The variance.
         */
        LIBCTOOL_API double variance(const std::vector<double>& data);

        /**
         * @brief Calculates the standard deviation of a dataset.
         */
        LIBCTOOL_API double standardDeviation(const std::vector<double>& data);

        /**
         * @brief Checks if two floating-point values are approximately equal within a tolerance.
         * @param a First value.
         * @param b Second value.
         * @param epsilon The maximum allowable difference.
         * @return true if the values are near each other.
         */
        LIBCTOOL_API bool isNear(double a, double b, double epsilon = 1e-9);

        /**
         * @brief Clamps a value within a specified range.
         * @tparam T Numeric type.
         * @param val Value to clamp.
         * @param min Minimum boundary.
         * @param max Maximum boundary.
         * @return T The clamped value.
         */
        template <typename T>
        T clamp(T val, T min, T max);

        /**
         * @brief Calculates the median value of a dataset.
         * @note The input vector is not modified during calculation (const reference).
         *       A copy is made internally to sort if necessary.
         * @tparam T Numeric type (e.g., double, int).
         * @param data Vector of numeric values.
         * @return T The median value. Returns 0 if the vector is empty.
         */
        template <typename T>
        T median(const std::vector<T>& data);

        /**
         * @brief Calculates the sum of numeric values in a dataset.
         * @note This function uses `double` for internal accumulation to prevent overflow 
         *       and loss of precision when summing integers or floating-point numbers.
         *       The return type `T` is cast from the accumulated double value.
         * @tparam T Numeric type (e.g., double, int, float).
         * @param data Vector of numeric values to sum.
         * @return T The sum of the elements. Returns 0 if the vector is empty.
         */
        template <typename T>
        T sum(const std::vector<T>& data);

        /**
         * @brief Calculates the $k$-th percentile of a dataset.
         * @note Uses linear interpolation between values if the percentile falls between indices.
         * @tparam T Numeric type.
         * @param data Vector of numeric values.
         * @param k The percentile to calculate (0.0 to 100.0).
         * @return double The value at the specified percentile.
         * @return 0.0 if the vector is empty.
         * @throw std::invalid_argument if $k$ is outside [0, 100].
         */
        template <typename T>
        double percentile(const std::vector<T>& data, double k);

        /**
         * @brief Normalizes a dataset to a range [0, 1] using Min-Max scaling.
         * @note Formula: $x_{scaled} = \frac{x - min}{max - min}$.
         *       If $max == min$, all values are set to 0.5 (or 0.0 if all are same).
         * @tparam T Numeric type.
         * @param data Vector of numeric values.
         * @return std::vector<double> The normalized values.
         */
        template <typename T>
        std::vector<double> minMaxScale(const std::vector<T>& data);

    }    // namespace math
}    // namespace ct

    #include "ct_math.tpp"

#endif    // CT_MATH_HPP