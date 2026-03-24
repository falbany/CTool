/** 
 * @file ct_str.hpp
 * @author Florent ALBANY - FAL
 * @brief String utilities for string manipulation.
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#ifndef CT_STR_HPP
    #define CT_STR_HPP

    #include <string>
    #include <vector>
    #include "../libctool.h"

namespace ct {
    // ------------------------------------------------------------------------
    // STRING API
    // ------------------------------------------------------------------------

    /**
     * @namespace ct::str
     * @brief Utilities for safe string manipulation, splitting, and type conversion.
     */
    namespace str {

        /**
         * @brief Safely assigns a C-style char array to a standard string.
         * @param dest The destination `std::string` object.
         * @param src The source C-style string. If `nullptr`, `dest` is cleared.
         */
        LIBCTOOL_API void assign(std::string& dest, const char* src);

        /**
         * @brief Updates or appends a prefixed field within a delimited string.
         * @param currentStr The base string (e.g., "SN001_LOT42").
         * @param prefix The identifier prefix to look for (e.g., "T").
         * @param newValue The new value to set (e.g., "25.00").
         * @param delimiter The character splitting the fields. Defaults to '_'.
         * @return std::string The updated string (e.g., "SN001_LOT42_T25.00").
         */
        LIBCTOOL_API std::string updateField(const std::string& currentStr, const std::string& prefix, const std::string& newValue, char delimiter = '_');

        /**
         * @brief Finds and replaces all occurrences of a substring within a string.
         * @param sIn The original string.
         * @param searchStr The substring to look for.
         * @param replaceStr The substring to replace `searchStr` with.
         * @return std::string The modified string.
         */
        LIBCTOOL_API std::string findAndReplace(const std::string& sIn, const std::string& searchStr, const std::string& replaceStr);

        /**
         * @brief Recursively finds and replaces all occurrences of a substring.
         * @warning Do not use if `replaceStr` contains `searchStr`, as it will cause an infinite loop.
         * @param sIn The original string.
         * @param searchStr The substring to look for.
         * @param replaceStr The substring to replace `searchStr` with.
         * @return std::string The modified string.
         */
        LIBCTOOL_API std::string findAndReplaceRecursive(const std::string& sIn, const std::string& searchStr, const std::string& replaceStr);

        /**
         * @brief Extracts a substring trapped between two delimiter strings.
         * @param sIn The original string to parse.
         * @param searchStr1 The starting delimiter.
         * @param searchStr2 The ending delimiter.
         * @return std::string The extracted internal string, or an empty string if bounds are not found.
         */
        LIBCTOOL_API std::string catchInRange(const std::string& sIn, const std::string& searchStr1, const std::string& searchStr2);

        /**
         * @brief Splits a string into a vector of substrings based on a delimiter character.
         * @param str The string to split.
         * @param delimiter The character used as the separation boundary.
         * @return std::vector<std::string> A list of extracted tokens. Empty tokens are discarded.
         */
        LIBCTOOL_API std::vector<std::string> split(const std::string& str, char delimiter);

        /**
         * @brief Removes leading and trailing whitespace characters from a string.
         * @param str The string to trim.
         * @return std::string The trimmed string.
         */
        LIBCTOOL_API std::string trim(const std::string& str);

        /**
         * @brief Converts any fundamental data type to its string representation.
         * @tparam T The type of the value to convert.
         * @param value The value to be converted.
         * @return std::string The string representation of `value`.
         */
        template <typename T>
        std::string toString(T value);
    }    // namespace str
}    // namespace ct
    #include "ct_str.tpp"
#endif