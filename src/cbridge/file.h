/**
 * @file file.h
 * @author Florent ALBANY - FAL
 * @brief Utilities for filesystem operations and configuration parsing in pure C.
 * @version 1.0
 * @date 2026-03-19
 */

#ifndef CBRIDGE_FILE_H
#define CBRIDGE_FILE_H

#include "string.h"
#include "vector.h"
#include <stdbool.h>
#include "../internal/libctool.h"

/**
 * @struct cbridge_file_namespace
 * @brief Function pointer table acting as the "cbridge::file" namespace.
 */
struct cbridge_file_namespace {
    /**
     * @brief Checks if a file exists and is accessible.
     */
    bool (*exists)(const char* path);

    /**
     * @brief Returns the size of a file in bytes.
     */
    long long (*get_size)(const char* path);

    /**
     * @brief Reads the entire content of a file into a dynamic string.
     * @return A new string_t* containing file data, or NULL on failure.
     */
    string_t* (*read_all)(const char* path);

    /**
     * @brief Retrieves the current working directory.
     * @return A new string_t* containing the path.
     */
    string_t* (*get_working_directory)();

    /**
     * @brief Extracts a value from a configuration file (key=value format).
     * @param path Path to the config file.
     * @param key The key to search for.
     * @param separator The separator character (e.g., "=").
     * @return A new string_t* containing the value, or empty string if not found.
     */
    string_t* (*get_parameter)(const char* path, const char* key, const char* separator);

    /**
 * @brief Reads a range of lines from a file (1-indexed, inclusive).
 * @param path Path to the file.
 * @param startLine Starting line number (1-based). Pass 0 to default to line 1.
 * @param endLine Ending line number (1-based, inclusive). Pass 0 to default to line 1.
 * @return A new string_t* containing the requested lines joined by '\n',
 *         or an empty string_t* on error (null path, file not found, invalid range).
 * @note If endLine exceeds the total number of lines, reads until EOF.
 *       If startLine > endLine (after normalization), returns an empty string.
 *       A value of 0 for startLine or endLine is internally treated as 1.
 */
string_t* (*read_lines)(const char* path, size_t startLine, size_t endLine);

/**
     * @param directory The target directory.
     * @param prefix Optional prefix filter (use NULL for none).
     * @param suffix Optional suffix filter (e.g., ".csv", NULL for none).
     * @return A vector_t* containing string_t* objects for each filename.
     */
    vector_t* (*get_files)(const char* directory, const char* prefix, const char* suffix);
};

LIBCTOOL_API extern const struct cbridge_file_namespace cbridge_file;

#endif    // CBRIDGE_FILE_H