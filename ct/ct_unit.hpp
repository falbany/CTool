/**
 * @file ct_unit.hpp
 * @author Florent ALBANY (f.albany@serma.com)
 * @brief Utilities for engineering notation and unit conversion.
 */

#pragma once

#ifndef CT_UNIT_HPP
    #define CT_UNIT_HPP

    #include <string>

namespace ct {
    namespace unit {

        /**
         * @brief Formats a number into engineering notation (e.g., 0.0012 -> "1.20 m").
         * @param value The numeric value to format.
         * @param precision Number of decimal places.
         * @param unit Optional unit suffix (e.g., "A", "V", "Hz").
         * @return std::string Formatted string.
         */
        std::string format(double value, int precision = 2, const std::string& unit = "");

        /**
         * @brief Parses a string with an engineering prefix back into a double.
         * @param input String like "1.5n" or "2.2k".
         * @return double The numeric value (e.g., 1.5e-9).
         */
        double parse(const std::string& input);

        /**
         * @brief Converts Celsius to Kelvin.
         */
        inline double cToK(double c) { return c + 273.15; }

        /**
         * @brief Converts Kelvin to Celsius.
         */
        inline double kToC(double k) { return k - 273.15; }

    }    // namespace unit
}    // namespace ct

#endif    // CT_UNIT_HPP