/**
 * @file str.h
 * @author Florent ALBANY - FAL
 * @brief Pure C imitation of C++ std::string and namespaces.
 * @version 3.0
 * @date 2026-03-19
 */

#ifndef CBRIDGE_STR_H
#define CBRIDGE_STR_H

#include <stddef.h>
#include <stdbool.h>
#include "../internal/libctool.h"

/**
 * @brief Returned by search functions to indicate no matches were found.
 * Mimics std::string::npos.
 */
#define CBRIDGE_STR_NPOS ((size_t)-1)

/**
 * @struct string_t
 * @brief The data structure representing a dynamic string.
 * This mimics the internal state of a C++ std::string.
 */
typedef struct {
    char*  data;        ///< Pointer to the null-terminated char array.
    size_t length;      ///< Current number of characters (excluding null terminator).
    size_t capacity;    ///< Currently allocated memory size.
} string_t;

/**
 * @struct string_parts_t
 * @brief Result of splitting a string. Contains an array of string_t pointers
 *        and the total number of parts.
 * @note  Caller owns all memory. Use cbridge_str_parts_free() to deallocate.
 */
typedef struct {
    string_t** parts;    ///< Array of dynamically allocated string_t objects.
    size_t     count;    ///< Number of elements in the parts array.
} string_parts_t;

/**
 * @brief Frees a string_parts_t result: deallocates every part and the parts array itself.
 * @param parts Pointer to the split result (will be zeroed out after the call).
 * @note  Safe to call with a NULL pointer or a result where parts == NULL.
 */
LIBCTOOL_API void cbridge_str_parts_free(string_parts_t* parts);

/**
 * @struct cbridge_string_namespace
 * @brief A struct of function pointers acting as the "cbridge::str" namespace.
 * This groups all string manipulation functions under a single API object.
 */
struct cbridge_string_namespace {
    // --- LIFECYCLE MANAGEMENT ---

    /**
     * @brief Creates a dynamically resizing string object.
     * @param initString Initial C-string (can be NULL).
     * @return Pointer to a newly allocated string_t.
     */
    string_t* (*create)(const char* initString);

    /**
     * @brief Safely frees the string and its internal buffer.
     * @param self The string object to destroy.
     */
    void (*free)(string_t* self);

    // --- CAPACITY & STATE ---

    /**
     * @brief Explicitly increases the internal capacity of the string.
     * @param self The string object.
     * @param newCapacity The new capacity in bytes.
     */
    void (*reserve)(string_t* self, size_t newCapacity);

    /**
     * @brief Empties the string (length becomes 0) without freeing the allocated capacity.
     */
    void (*clear)(string_t* self);

    /**
     * @brief Checks if the string is empty.
     * @return true if length is 0, false otherwise.
     */
    bool (*empty)(const string_t* self);

    /**
     * @brief Gets the current length of the string.
     */
    size_t (*length)(const string_t* self);

    /**
     * @brief Returns the raw, null-terminated C-string.
     */
    const char* (*c_str)(const string_t* self);

    // --- MODIFIERS ---

    /**
     * @brief Appends a standard C-string to the dynamic string.
     */
    void (*append)(string_t* self, const char* suffix);

    /**
     * @brief Inserts a C-string at a specific position.
     * @param self The string object.
     * @param pos The index at which to insert.
     * @param str The string to insert.
     */
    void (*insert)(string_t* self, size_t pos, const char* str);

    /**
     * @brief Removes a portion of the string.
     * @param self The string object.
     * @param pos The starting index of the removal.
     * @param len The number of characters to remove.
     */
    void (*erase)(string_t* self, size_t pos, size_t len);

    /**
     * @brief Removes leading and trailing whitespace characters.
     */
    void (*trim)(string_t* self);

    /**
     * @brief Replaces all occurrences of a search string with a replace string.
     */
    void (*replace_all)(string_t* self, const char* searchString, const char* replaceString);

    // --- OPERATIONS ---

    /**
     * @brief Compares the string with a standard C-string.
     * @return 0 if equal, <0 if self is less, >0 if self is greater.
     */
    int (*compare)(const string_t* self, const char* other);

    /**
     * @brief Checks if the string starts with a specific prefix.
     */
    bool (*starts_with)(const string_t* self, const char* prefix);

    /**
     * @brief Checks if the string ends with a specific suffix.
     */
    bool (*ends_with)(const string_t* self, const char* suffix);

    /**
     * @brief Finds the first occurrence of a substring.
     * @param self The string object.
     * @param target The string to search for.
     * @param start_pos The index to start searching from (default 0).
     * @return The index of the first match, or CBRIDGE_STR_NPOS if not found.
     */
    size_t (*find)(const string_t* self, const char* target, size_t start_pos);

    /**
     * @brief Returns a newly allocated substring.
     * @param self The original string object.
     * @param pos The starting index.
     * @param len The length of the substring to extract.
     * @return A new string_t containing the extracted text.
     */
    string_t* (*substr)(const string_t* self, size_t pos, size_t len);

    /**
     * @brief Splits the string by a delimiter, returning an array of newly allocated string_t objects.
     * @param self The string object.
     * @param delimiter The delimiter string (can be NULL/empty to return the whole string as a single part).
     * @return A string_parts_t with the parts array and count.
     * @note  Caller must call cbridge_str_parts_free() on the result when done.
     */
    string_parts_t (*split)(const string_t* self, const char* delimiter);

    /**
     * @brief Extracts a substring trapped between two delimiter strings.
     * @return A new string_t containing the caught text.
     */
    string_t* (*catch_in_range)(const string_t* self, const char* startString, const char* endString);
};

/**
 * @brief The global namespace instance. Use this to access the string functions.
 * ### Example Usage:
 * @code
 * string_t* scpi_cmd = cbridge_string.create("MEAS:VOLT:DC?");
 * if (cbridge_string.starts_with(scpi_cmd, "MEAS")) {
 * cbridge_string.append(scpi_cmd, " 10, 0.01");
 * }
 * cbridge_string.free(scpi_cmd);
 * @endcode
 */
LIBCTOOL_API extern const struct cbridge_string_namespace cbridge_string;

#endif    // CBRIDGE_STR_H