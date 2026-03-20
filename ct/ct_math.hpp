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
        RegResult linearFit(const std::vector<double>& x, const std::vector<double>& y);

        /**
     * @brief Calculates the arithmetic mean (average) of a dataset.
     */
        double mean(const std::vector<double>& data);

        /**
     * @brief Calculates the standard deviation of a dataset.
     */
        double standardDeviation(const std::vector<double>& data);

        /**
     * @brief Checks if two floating-point values are approximately equal within a tolerance.
     * @param a First value.
     * @param b Second value.
     * @param epsilon The maximum allowable difference.
     * @return true if the values are near each other.
     */
        bool isNear(double a, double b, double epsilon = 1e-9);

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

    }    // namespace math
}    // namespace ct

    #include "ct_math.tpp"

#endif    // CT_MATH_HPP