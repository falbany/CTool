/** 
 * @file ct_time.hpp
 * @author Florent ALBANY (f.albany@serma.com)
 * @brief Time utilities.
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#ifndef CT_TIME_HPP
    #define CT_TIME_HPP

    #include <string>
    #include <chrono>

namespace ct {
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
         * @return std::string Formatted timestamp as "YYYY-MM-DD HH:MM:SS".
         */
        std::string getCurrentTimestamp();

        /**
         * @brief Halts the execution of the current thread for a specified number of milliseconds.
         * @param milliseconds Duration to sleep in milliseconds.
         */
        void sleepMs(long milliseconds);

        /**
         * @brief Halts the execution of the current thread for a specified number of seconds.
         * @param seconds Duration to sleep in seconds.
         */
        void sleepS(long seconds);

        /**
         * @brief Retrieves the uptime of the application (or steady clock) in milliseconds.
         * * Useful for lightweight execution timing or benchmarking standard operations.
         * @return long long Elapsed milliseconds since an arbitrary epoch (steady clock).
         */
        long long getUptimeMs();

        /**
         * @brief Retrieves the uptime of the application in nanoseconds.
         * @return long long Elapsed nanoseconds since an arbitrary epoch (steady clock).
         */
        long long getUptimeNs();

        /**
         * @brief A simple scoped timer that prints the elapsed time in the destructor.
         * * Usage: ct::time::Timer timer("Operation Name"); ... work ... 
         */
        class Timer {
          public:
            explicit Timer(const std::string& label = "Unknown");
            ~Timer();

            // Prevent copying
            Timer(const Timer&)            = delete;
            Timer& operator=(const Timer&) = delete;

          private:
            std::string                           label_;
            std::chrono::steady_clock::time_point start_;
        };
    }    // namespace time
}    // namespace ct
#endif