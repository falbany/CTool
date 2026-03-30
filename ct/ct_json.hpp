/**
 * @file ct_json.hpp
 * @author Florent ALBANY (FAL)
 * @brief Lightweight JSON serializer and parser for C++11.
 * @version 1.1
 * @date 2026-03-20
 * @note This is a lightweight implementation. For complex schemas, 
 * consider a full-featured library like nlohmann/json.
 */

#ifndef CT_JSON_HPP
#define CT_JSON_HPP

#include <string>
#include <map>
#include <vector>
#include "../libctool.h"

namespace ct {
    /**
     * @namespace ct::json
     * @brief Utilities for JSON data handling and stringification.
     */
    namespace json {

        /**
         * @brief Converts a flat map of strings into a JSON object string.
         * @param kv_pairs Map of Key-Value pairs.
         * @return std::string Formatted JSON: {"key": "value"}.
         */
        LIBCTOOL_API std::string stringify(const std::map<std::string, std::string>& kv_pairs);

        /**
         * @brief Serializes a list of strings into a JSON array.
         * @param list Vector of strings.
         * @return std::string Formatted JSON array: ["item1", "item2"].
         */
        LIBCTOOL_API std::string stringifyArray(const std::vector<std::string>& list);

        /**
         * @brief Extracts a string value associated with a key in a JSON string.
         * @details Handles basic whitespace and escaped quotes.
         * @param json The source JSON string.
         * @param key The key to look for.
         * @return std::string The value found, or empty string if not found.
         */
        LIBCTOOL_API std::string getValue(const std::string& json, const std::string& key);

        /**
         * @brief Validates if a string has basic JSON syntax (starts/ends with braces).
         * @param json String to check.
         * @return true if it appears to be valid JSON.
         */
        LIBCTOOL_API bool isValid(const std::string& json);

        /**
         * @brief Minifies a JSON string by removing unnecessary whitespace.
         * @param json The source JSON.
         * @return std::string Minified version.
         */
        LIBCTOOL_API std::string minify(const std::string& json);

    }    // namespace json
}    // namespace ct

#endif    // CT_JSON_HPP