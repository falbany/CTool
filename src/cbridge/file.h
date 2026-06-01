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

    /**
     * @brief Writes the entire string content to a file, creating or overwriting it.
     * @param path Path to the target file.
     * @param content Null-terminated string to write. If NULL, treated as an empty string.
     * @return true on success, false on failure.
     * @note Returns false if path is NULL. Opens file in text mode ("w").
     */
    bool (*write_all)(const char* path, const char* content);

    /**
     * @brief Appends the entire string content to a file, creating it if it doesn't exist.
     * @param path Path to the target file.
     * @param content Null-terminated string to append. If NULL, treated as an empty string.
     * @return true on success, false on failure.
     * @note Returns false if path is NULL. Opens file in append mode ("a").
     */
    bool (*append_all)(const char* path, const char* content);

    /**
     * @brief Extracts the file extension (including the dot) from the last '.' in the filename.
     * @param path Path to the file.
     * @return A new string_t* containing the extension (e.g., ".csv"), or an empty string_t* if none found.
     * @note Returns an empty string_t* if path is NULL or no extension exists.
     *       Only considers the filename portion (after the last '/' or '\\').
     *       Callers are responsible for cbridge_string.free() on the returned pointer.
     */
    string_t* (*get_extension)(const char* path);

    /**
     * @brief Deletes a file from disk.
     * @param path Path to the file to remove.
     * @return true on success, false on failure.
     * @note Returns false if `path` is NULL or removal fails (permissions, not found).
     */
    bool (*remove)(const char* path);

    /**
     * @brief Copies a file from `src` to `dest`, overwriting `dest` if it exists.
     * @param src Source file path.
     * @param dest Destination file path.
     * @return true on success, false on failure.
     */
    bool (*copy)(const char* src, const char* dest);

    /**
     * @brief Returns whether a path exists and is a directory.
     * @param path Path to test.
     * @return true if the path exists and is a directory, false otherwise.
     */
    bool (*is_directory)(const char* path);

    /**
     * @brief Extracts the filename (basename) from a path.
     * @param path Path to extract the filename from.
     * @return A new string_t* containing the last path component (e.g., "file.txt" from "/path/to/file.txt"),
     *         or an empty string_t* if path is NULL or empty.
     * @note Handles both '/' and '\\' as path separators.
     *       The extension is NOT stripped; callers use get_extension() separately if needed.
     *       Callers are responsible for cbridge_string.free() on the returned pointer.
     */
    string_t* (*get_filename)(const char* path);

    /**
     * @brief Creates a single directory at the given path.
     * @param path Path to the directory to create.
     * @return true on success, false on failure.
     * @note Returns false if path is NULL, creation fails, or the path already exists as a file.
     *       Does NOT create intermediate (parent) directories.
     *       Uses _mkdir on Windows, mkdir on POSIX.
     */
    bool (*create_directory)(const char* path);

    /**
     * @brief Moves or renames a file from src to dest.
     * @param src Source file path.
     * @param dest Destination file path.
     * @return true on success, false on failure.
     * @note Overwrites dest if it already exists.
     *       Returns false if src does not exist or if either parameter is NULL.
     *       Prefers standard rename(); falls back to copy+remove on Windows if cross-device moves fail.
     */
    bool (*move)(const char* src, const char* dest);

    /**
     * @brief Reads the last count lines from a file.
     * @param path Path to the file.
     * @param count Number of lines to read from the end. Pass 0 to return an empty string.
     * @return A new string_t* containing the last count lines joined by '\n',
     *         or an empty string_t* on error (null path, file not found, count is 0).
     * @note If the file has fewer than count lines, returns all lines.
     *       Trailing '\n' / '\r\n' from each line are stripped before joining.
     *       Callers are responsible for cbridge_string.free() on the returned pointer.
     */
    string_t* (*read_last_lines)(const char* path, size_t count);
};

LIBCTOOL_API extern const struct cbridge_file_namespace cbridge_file;

#endif    // CBRIDGE_FILE_H