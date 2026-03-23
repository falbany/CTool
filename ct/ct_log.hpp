/** 
 * @file ct_log.hpp
 * @author Florent ALBANY - FAL
 * @brief Log utilities.
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#ifndef CT_LOG_HPP
    #define CT_LOG_HPP

    #include <iostream>
    #include <string>

namespace ct {
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
         * @param msgLevel The severity level of the current message.
         * @param msg The string message to log.
         * @param stream The output stream destination (e.g., `std::cout`, `std::cerr`). Defaults to `std::cout`.
         * @param currentLevel The global/application verbosity threshold. Defaults to `Level::Debug`.
         */
        void message(Level msgLevel, const std::string& msg, std::ostream& stream = std::cout, Level currentLevel = Level::Debug);

        /**
         * @brief Convenience function to log a message directly to standard output (`std::cout`).
         * @param msgLevel The severity level of the current message.
         * @param msg The string message to log.
         * @param currentLevel The global/application verbosity threshold. Defaults to `Level::Warning`.
         */
        void toCout(Level msgLevel, const std::string& msg, Level currentLevel = Level::Warning);

        /**
         * @brief Convenience function to log a message directly to standard error (`std::cerr`).
         * @param msgLevel The severity level of the current message.
         * @param msg The string message to log.
         * @param currentLevel The global/application verbosity threshold. Defaults to `Level::Warning`.
         */
        void toCerr(Level msgLevel, const std::string& msg, Level currentLevel = Level::Warning);

    }    // namespace log
}    // namespace ct
#endif