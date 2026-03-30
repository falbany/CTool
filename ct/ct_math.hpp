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
         * @brief Calculates the arithmetic mean (average) of a dataset.
         */
        LIBCTOOL_API double mean(const std::vector<double>& data);

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

    }    // namespace math
}    // namespace ct

    #include "ct_math.tpp"

#endif    // CT_MATH_HPP