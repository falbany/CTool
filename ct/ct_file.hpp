/** 
 * @file ct_file.hpp
 * @author Florent ALBANY (f.albany@serma.com)
 * @brief Utilities for file system operations, configuration parsing, and directory traversal.
 * @version 1.1
 * @date 2026-03-18
 */

#pragma once

#ifndef CT_FILE_HPP
    #define CT_FILE_HPP

    #include <string>
    #include <vector>

namespace ct {
    namespace file {

        // --- READ OPERATIONS ---

        /**
         * @brief Checks if a file exists and is accessible.
         * @param filename Path to the file.
         * @return true if accessible, false otherwise.
         */
        bool exists(const std::string& filename);

        /**
         * @brief Retrieves the size of a file in bytes.
         * @param filename Path to the file.
         * @return long long Size in bytes, or -1 if the file does not exist.
         */
        long long getSize(const std::string& filename);

        /**
         * @brief Reads the entire contents of a file into a string.
         * @param filename Path to the file.
         * @return std::string The file content.
         */
        std::string readAll(const std::string& filename);

        /**
         * @brief Reads a file line by line into a vector.
         * @param filename Path to the file.
         * @return std::vector<std::string> A list of lines.
         */
        std::vector<std::string> readLines(const std::string& filename);


        // --- WRITE OPERATIONS ---

        /**
         * @brief Writes a string to a file, overwriting existing content.
         * @param filename Path to the file.
         * @param content The string to write.
         * @return true if successful.
         */
        bool writeAll(const std::string& filename, const std::string& content);

        /**
         * @brief Appends a single line to the end of a file.
         * @param filename Path to the file.
         * @param line The string to append.
         * @return true if successful.
         */
        bool appendLine(const std::string& filename, const std::string& line);


        // --- CONFIGURATION & PARSING ---

        /**
         * @brief Extracts a configuration parameter while ignoring common comment markers.
         * * Supports stripping comments starting with //, #, *, ;, or !.
         * @param filename Path to the configuration file.
         * @param key The parameter name.
         * @param separator The delimiter between key and value (default: "=").
         * @return std::string The extracted value, trimmed of whitespace.
         */
        std::string getParameter(const std::string& filename, const std::string& key, const std::string& separator = "=");


        // --- PATH & DIRECTORY ---

        /**
         * @brief Retrieves the current working directory.
         * @return std::string Absolute path of the working directory.
         */
        std::string getWorkingDirectory();

        /**
         * @brief Extracts the file extension (e.g., ".csv") from a path.
         * @param path The full path or filename.
         * @return std::string The extension including the dot.
         */
        std::string getExtension(const std::string& path);

        /**
         * @brief Lists filenames in a directory matching specific criteria.
         * @param directory The target path.
         * @param prefix Optional filename start string.
         * @param suffix Optional filename end string (e.g., extension).
         * @return std::vector<std::string> List of matching filenames.
         */
        std::vector<std::string> getFiles(const std::string& directory, const std::string& prefix = "", const std::string& suffix = "");

        /**
         * @brief Deletes a file from the disk.
         * @param filename Path to the file.
         * @return true if successfully deleted.
         */
        bool remove(const std::string& filename);

    }    // namespace file
}    // namespace ct
#endif