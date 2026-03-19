/**
 * @file cb_file.h
 * @author Florent ALBANY (f.albany@serma.com)
 * @brief Utilities for filesystem operations and configuration parsing in pure C.
 * @version 1.0
 * @date 2026-03-19
 */

#ifndef CB_FILE_H
#define CB_FILE_H

#include "cb_str.h"
#include "cb_vector.h"
#include <stdbool.h>

/**
 * @struct cb_file_namespace
 * @brief Function pointer table acting as the "cb::file" namespace.
 */
struct cb_file_namespace {
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
     * @return A new cb_string_t* containing file data, or NULL on failure.
     */
    cb_string_t* (*read_all)(const char* path);

    /**
     * @brief Retrieves the current working directory.
     * @return A new cb_string_t* containing the path.
     */
    cb_string_t* (*get_working_directory)();

    /**
     * @brief Extracts a value from a configuration file (key=value format).
     * @param path Path to the config file.
     * @param key The key to search for.
     * @param separator The separator character (e.g., "=").
     * @return A new cb_string_t* containing the value, or empty string if not found.
     */
    cb_string_t* (*get_parameter)(const char* path, const char* key, const char* separator);

    /**
     * @brief Lists files in a directory matching specific criteria.
     * @param directory The target directory.
     * @param prefix Optional prefix filter (use NULL for none).
     * @param suffix Optional suffix filter (e.g., ".csv", NULL for none).
     * @return A cb_vector_t* containing cb_string_t* objects for each filename.
     */
    cb_vector_t* (*get_files)(const char* directory, const char* prefix, const char* suffix);
};

extern const struct cb_file_namespace cb_file;

#endif    // CB_FILE_H