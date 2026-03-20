/**
 * @file ct_sys.hpp
 * @author Florent ALBANY - FAL
 * @brief System-level utilities for OS interaction and process management.
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#ifndef CT_SYS_HPP
    #define CT_SYS_HPP

    #include <string>
    #include <vector>

namespace ct {

    /**
     * @namespace ct::sys
     * @brief Utilities for interacting with the Operating System and environment.
     */
    namespace sys {

        /**
         * @brief Executes a system command and captures its standard output (stdout).
         * @param command The shell command to execute.
         * @return std::string The resulting output from the command.
         */
        std::string executeCommand(const std::string& command);

        /**
         * @brief Retrieves the value of an environment variable.
         * @param varName The name of the environment variable (e.g., "PATH").
         * @return std::string The value of the variable, or an empty string if not found.
         */
        std::string getEnv(const std::string& varName);

        /**
         * @brief Creates a new directory on the disk.
         * @param path The path of the directory to create.
         * @return true If the directory was created successfully or already exists.
         * @return false If creation failed.
         */
        bool makeDirectory(const std::string& path);

        /**
         * @brief Retrieves the network hostname of the local machine.
         * @return std::string The machine hostname.
         */
        std::string getHostname();

        /**
         * @brief Retrieves the Process ID (PID) of the current application.
         * @return int The current process identifier.
         */
        int getProcessId();

        /**
         * @brief Checks if the current system architecture is Little Endian.
         * @return true if Little Endian, false if Big Endian.
         */
        bool isLittleEndian();

    }    // namespace sys
}    // namespace ct

#endif    // CT_SYS_HPP