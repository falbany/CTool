/**
 * @file unit.hpp
 * @author Florent ALBANY - FAL
 * @brief Utilities for engineering notation and unit conversion.
 */

#pragma once

#ifndef CT_UNIT_HPP
    #define CT_UNIT_HPP

    #include <string>
    #include "../internal/libctool.h"

namespace ctool {
    namespace unit {
        // ------------------------------------------------------------------------
        // TEMPERATURE CONSTANTS
        // ------------------------------------------------------------------------

        /** @brief Absolute zero in Celsius, used for Celsius to Kelvin conversion. */
        constexpr double kAbsoluteZeroCelsius = 273.15;

        // ------------------------------------------------------------------------
        // API
        // ------------------------------------------------------------------------

        /**
         * @brief Constant for Celsius to Kelvin conversion offset.
         */
        constexpr double kKelvinOffset = kAbsoluteZeroCelsius;

        /**
         * @brief Formats a number into engineering notation (e.g., 0.0012 -> "1.20 m").
         * @param value The numeric value to format.
         * @param precision Number of decimal places.
         * @param unit Optional unit suffix (e.g., "A", "V", "Hz").
         * @return std::string Formatted string.
         */
        LIBCTOOL_API std::string format(double value, int precision = 2, const std::string& unit = "");

        /**
         * @brief Parses a string with an engineering prefix back into a double.
         * @param input String like "1.5n" or "2.2k".
         * @return double The numeric value (e.g., 1.5e-9).
         */
        LIBCTOOL_API double parse(const std::string& input);

        /**
         * @brief Converts Celsius to Kelvin.
         * @param celsiusValue The temperature in Celsius.
         */
        LIBCTOOL_API inline double cToK(double celsiusValue) { return celsiusValue + kAbsoluteZeroCelsius; }

        /**
         * @brief Converts Kelvin to Celsius.
         * @param kelvinValue The temperature in Kelvin.
         */
        LIBCTOOL_API inline double kToC(double kelvinValue) { return kelvinValue - kAbsoluteZeroCelsius; }

    }    // namespace unit
}    // namespace ctool

#endif    // CT_UNIT_HPP