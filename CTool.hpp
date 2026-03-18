/**
 * @file CTool.hpp
 * @author Florent ALBANY (f.albany@serma.com)
 * @brief Core utility functions for Time, String, File, and Logging operations.
 * * This header provides a modernized, C++11-compliant collection of utilities 
 * to facilitate hardware testing, string manipulation, file parsing, and 
 * cross-platform execution flows.
 * * @version 1.0
 * @date 2026-03-18
 * @copyright Copyright (c) 2026
 */

#pragma once

#ifndef CTOOL_UTILITIES_HPP
#define CTOOL_UTILITIES_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <thread>

/**
 * @namespace ct
 * @brief The core namespace encapsulating all CTool utilities.
 */
namespace ct {

    // ------------------------------------------------------------------------
    // CORE UTILITIES
    // ------------------------------------------------------------------------
    
    /**
     * @brief Converts a C-style array to an `std::vector`.
     * * This function safely copies elements from a raw C-style array into a 
     * managed C++ standard vector.
     * * @tparam T The data type of the array elements.
     * @param arr Pointer to the first element of the C-style array.
     * @param size The total number of elements in the array.
     * @return std::vector<T> A newly constructed vector containing the copied elements.
     * @throws std::invalid_argument If the input pointer `arr` is `nullptr`.
     */
    template <typename T>
    std::vector<T> vectorize(T* arr, size_t size) {
        if (!arr) {
            throw std::invalid_argument("Input array pointer cannot be null.");
        }
        return std::vector<T>(arr, arr + size);
    }

    // ------------------------------------------------------------------------
    // LOGGING API
    // ------------------------------------------------------------------------
    
    /**
     * @namespace ct::log
     * @brief Logging utilities for console output with varying verbosity levels.
     */
    namespace log {
        
        /**
         * @enum Level
         * @brief Defines the verbosity levels for the logging system.
         */
        enum class Level {
            Quiet   = 0,    ///< No output generated.
            Error   = 1,    ///< Only critical errors are logged.
            Warning = 2,    ///< Errors and warnings are logged.
            Info    = 3,    ///< Standard informational messages, warnings, and errors.
            Debug   = 4     ///< Maximum verbosity, used for deep troubleshooting.
        };

        /**
         * @brief Logs a formatted message to a specific output stream.
         * * @param msgLevel The severity level of the current message.
         * @param msg The string message to log.
         * @param stream The output stream destination (e.g., `std::cout`, `std::cerr`). Defaults to `std::cout`.
         * @param currentLevel The global/application verbosity threshold. Defaults to `Level::Debug`.
         */
        void message(Level msgLevel, const std::string& msg, std::ostream& stream = std::cout, Level currentLevel = Level::Debug);

        /**
         * @brief Convenience function to log a message directly to standard output (`std::cout`).
         * * @param msgLevel The severity level of the current message.
         * @param msg The string message to log.
         * @param currentLevel The global/application verbosity threshold. Defaults to `Level::Warning`.
         */
        void toCout(Level msgLevel, const std::string& msg, Level currentLevel = Level::Warning);

        /**
         * @brief Convenience function to log a message directly to standard error (`std::cerr`).
         * * @param msgLevel The severity level of the current message.
         * @param msg The string message to log.
         * @param currentLevel The global/application verbosity threshold. Defaults to `Level::Warning`.
         */
        void toCerr(Level msgLevel, const std::string& msg, Level currentLevel = Level::Warning);
    }

    // ------------------------------------------------------------------------
    // TIME API
    // ------------------------------------------------------------------------
    
    /**
     * @namespace ct::time
     * @brief Platform-independent utilities for timestamps, execution delays, and profiling.
     */
    namespace time {
        
        /**
         * @brief Generates a human-readable timestamp of the current system time.
         * * @return std::string Formatted timestamp as "YYYY-MM-DD HH:MM:SS".
         */
        std::string getCurrentTimestamp();
        
        /**
         * @brief Halts the execution of the current thread for a specified number of milliseconds.
         * * @param milliseconds Duration to sleep in milliseconds.
         */
        void sleepMs(long milliseconds);

        /**
         * @brief Halts the execution of the current thread for a specified number of seconds.
         * * @param seconds Duration to sleep in seconds.
         */
        void sleepS(long seconds);
        
        /**
         * @brief Retrieves the uptime of the application (or steady clock) in milliseconds.
         * * Useful for lightweight execution timing or benchmarking standard operations.
         * * @return long long Elapsed milliseconds since an arbitrary epoch (steady clock).
         */
        long long getUptimeMs(); 
    }

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
         * * @param dest The destination `std::string` object.
         * @param src The source C-style string. If `nullptr`, `dest` is cleared.
         */
        void assign(std::string& dest, const char* src);

        /**
         * @brief Updates or appends a prefixed field within a delimited string.
         * * @param currentStr The base string (e.g., "SN001_LOT42").
         * @param prefix The identifier prefix to look for (e.g., "T").
         * @param newValue The new value to set (e.g., "25.00").
         * @param delimiter The character splitting the fields. Defaults to '_'.
         * @return std::string The updated string (e.g., "SN001_LOT42_T25.00").
         */
        std::string updateField(const std::string& currentStr, const std::string& prefix, const std::string& newValue, char delimiter = '_');
        
        /**
         * @brief Finds and replaces all occurrences of a substring within a string.
         * * @param sIn The original string.
         * @param searchStr The substring to look for.
         * @param replaceStr The substring to replace `searchStr` with.
         * @return std::string The modified string.
         */
        std::string findAndReplace(const std::string& sIn, const std::string& searchStr, const std::string& replaceStr);

        /**
         * @brief Recursively finds and replaces all occurrences of a substring.
         * * @warning Do not use if `replaceStr` contains `searchStr`, as it will cause an infinite loop.
         * * @param sIn The original string.
         * @param searchStr The substring to look for.
         * @param replaceStr The substring to replace `searchStr` with.
         * @return std::string The modified string.
         */
        std::string findAndReplaceRecursive(const std::string& sIn, const std::string& searchStr, const std::string& replaceStr);

        /**
         * @brief Extracts a substring trapped between two delimiter strings.
         * * @param sIn The original string to parse.
         * @param searchStr1 The starting delimiter.
         * @param searchStr2 The ending delimiter.
         * @return std::string The extracted internal string, or an empty string if bounds are not found.
         */
        std::string catchInRange(const std::string& sIn, const std::string& searchStr1, const std::string& searchStr2);
        
        /**
         * @brief Splits a string into a vector of substrings based on a delimiter character.
         * * @param str The string to split.
         * @param delimiter The character used as the separation boundary.
         * @return std::vector<std::string> A list of extracted tokens. Empty tokens are discarded.
         */
        std::vector<std::string> split(const std::string& str, char delimiter);

        /**
         * @brief Removes leading and trailing whitespace characters from a string.
         * * @param str The string to trim.
         * @return std::string The trimmed string.
         */
        std::string trim(const std::string& str);

        /**
         * @brief Converts any fundamental data type to its string representation.
         * * @tparam T The type of the value to convert.
         * @param value The value to be converted.
         * @return std::string The string representation of `value`.
         */
        template <typename T>
        std::string toString(T value) {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
    }

    // ------------------------------------------------------------------------
    // FILE API
    // ------------------------------------------------------------------------
    
    /**
     * @namespace ct::file
     * @brief File system operations for reading configurations and navigating directories.
     */
    namespace file {
        
        /**
         * @brief Checks if a file exists and is accessible on the disk.
         * * @param filename The absolute or relative path to the file.
         * @return true If the file exists and can be opened.
         * @return false Otherwise.
         */
        bool exists(const std::string& filename);

        /**
         * @brief Retrieves the size of a file in bytes.
         * * @param filename The absolute or relative path to the file.
         * @return long long The size in bytes, or 0 if the file cannot be read.
         */
        long long getSize(const std::string& filename);

        /**
         * @brief Reads the entire contents of a file into a single string.
         * * Normalizes line endings by replacing carriage returns (`\r`) with newlines (`\n`).
         * * @param filename The path to the target file.
         * @return std::string The full file content. Returns an empty string on failure.
         */
        std::string readAll(const std::string& filename);

        /**
         * @brief Retrieves the current working directory of the application environment.
         * * @return std::string The absolute path of the working directory.
         */
        std::string getWorkingDirectory();

        /**
         * @brief Extracts a specific configuration parameter from a file.
         * * Strips standard comment tokens (`//`, `#`, etc.) and searches for a `key=value` format.
         * * @param filename Path to the configuration file.
         * @param key The parameter key to search for.
         * @param separator The character/string separating the key from the value (usually "=").
         * @return std::string The extracted value.
         */
        std::string getParameter(const std::string& filename, const std::string& key, const std::string& separator);
        
        /**
         * @brief Retrieves a list of filenames from a directory matching optional prefix/suffix criteria.
         * * @param directory The target directory path.
         * @param prefix (Optional) String that the filename must start with.
         * @param suffix (Optional) String that the filename must end with (e.g., ".txt").
         * @return std::vector<std::string> List of filenames matching the criteria.
         */
        std::vector<std::string> getFiles(const std::string& directory, const std::string& prefix = "", const std::string& suffix = "");
    }

} // namespace ct

#endif /* CTOOL_UTILITIES_HPP */