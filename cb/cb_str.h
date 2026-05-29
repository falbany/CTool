/**
 * @file cb_str.h
 * @author Florent ALBANY - FAL
 * @brief Pure C imitation of C++ std::string and namespaces.
 * @version 3.0
 * @date 2026-03-19
 */

#ifndef CB_STR_H
#define CB_STR_H

#include <stddef.h>
#include <stdbool.h>
#include "../libctool.h"

/**
 * @brief Returned by search functions to indicate no matches were found.
 * Mimics std::string::npos.
 */
#define CB_STR_NPOS ((size_t)-1)

/**
 * @struct cb_string_t
 * @brief The data structure representing a dynamic string.
 * This mimics the internal state of a C++ std::string.
 */
typedef struct {
    char*  data;        ///< Pointer to the null-terminated char array.
    size_t length;      ///< Current number of characters (excluding null terminator).
    size_t capacity;    ///< Currently allocated memory size.
} cb_string_t;

/**
 * @struct cb_str_parts_t
 * @brief Result of splitting a string. Contains an array of cb_string_t pointers
 *        and the total number of parts.
 * @note  Caller owns all memory. Use cb_str_parts_free() to deallocate.
 */
typedef struct {
    cb_string_t** parts;    ///< Array of dynamically allocated cb_string_t objects.
    size_t        count;    ///< Number of elements in the parts array.
} cb_str_parts_t;

/**
 * @brief Frees a cb_str_parts_t result: deallocates every part and the parts array itself.
 * @param parts Pointer to the split result (will be zeroed out after the call).
 * @note  Safe to call with a NULL pointer or a result where parts == NULL.
 */
LIBCTOOL_API void cb_str_parts_free(cb_str_parts_t* parts);

/**
 * @struct cb_str_namespace
 * @brief A struct of function pointers acting as the "cb::str" namespace.
 * This groups all string manipulation functions under a single API object.
 */
struct cb_str_namespace {
    // --- LIFECYCLE MANAGEMENT ---

    /**
     * @brief Creates a dynamically resizing string object.
     * @param init_str Initial C-string (can be NULL).
     * @return Pointer to a newly allocated cb_string_t.
     */
    cb_string_t* (*create)(const char* init_str);

    /**
     * @brief Safely frees the string and its internal buffer.
     * @param self The string object to destroy.
     */
    void (*free)(cb_string_t* self);

    // --- CAPACITY & STATE ---

    /**
     * @brief Explicitly increases the internal capacity of the string.
     * @param self The string object.
     * @param new_cap The new capacity in bytes.
     */
    void (*reserve)(cb_string_t* self, size_t new_cap);

    /**
     * @brief Empties the string (length becomes 0) without freeing the allocated capacity.
     */
    void (*clear)(cb_string_t* self);

    /**
     * @brief Checks if the string is empty.
     * @return true if length is 0, false otherwise.
     */
    bool (*empty)(const cb_string_t* self);

    /**
     * @brief Gets the current length of the string.
     */
    size_t (*length)(const cb_string_t* self);

    /**
     * @brief Returns the raw, null-terminated C-string.
     */
    const char* (*c_str)(const cb_string_t* self);

    // --- MODIFIERS ---

    /**
     * @brief Appends a standard C-string to the dynamic string.
     */
    void (*append)(cb_string_t* self, const char* suffix);

    /**
     * @brief Inserts a C-string at a specific position.
     * @param self The string object.
     * @param pos The index at which to insert.
     * @param str The string to insert.
     */
    void (*insert)(cb_string_t* self, size_t pos, const char* str);

    /**
     * @brief Removes a portion of the string.
     * @param self The string object.
     * @param pos The starting index of the removal.
     * @param len The number of characters to remove.
     */
    void (*erase)(cb_string_t* self, size_t pos, size_t len);

    /**
     * @brief Removes leading and trailing whitespace characters.
     */
    void (*trim)(cb_string_t* self);

    /**
     * @brief Replaces all occurrences of a search string with a replace string.
     */
    void (*replace_all)(cb_string_t* self, const char* search_str, const char* replace_str);

    // --- OPERATIONS ---

    /**
     * @brief Compares the string with a standard C-string.
     * @return 0 if equal, <0 if self is less, >0 if self is greater.
     */
    int (*compare)(const cb_string_t* self, const char* other);

    /**
     * @brief Checks if the string starts with a specific prefix.
     */
    bool (*starts_with)(const cb_string_t* self, const char* prefix);

    /**
     * @brief Checks if the string ends with a specific suffix.
     */
    bool (*ends_with)(const cb_string_t* self, const char* suffix);

    /**
     * @brief Finds the first occurrence of a substring.
     * @param self The string object.
     * @param target The string to search for.
     * @param start_pos The index to start searching from (default 0).
     * @return The index of the first match, or CB_STR_NPOS if not found.
     */
    size_t (*find)(const cb_string_t* self, const char* target, size_t start_pos);

    /**
     * @brief Returns a newly allocated substring.
     * @param self The original string object.
     * @param pos The starting index.
     * @param len The length of the substring to extract.
     * @return A new cb_string_t containing the extracted text.
     */
    cb_string_t* (*substr)(const cb_string_t* self, size_t pos, size_t len);

    /**
     * @brief Splits the string by a delimiter, returning an array of newly allocated cb_string_t objects.
     * @param self The string object.
     * @param delimiter The delimiter string (can be NULL/empty to return the whole string as a single part).
     * @return A cb_str_parts_t with the parts array and count.
     * @note  Caller must call cb_str_parts_free() on the result when done.
     */
    cb_str_parts_t (*split)(const cb_string_t* self, const char* delimiter);

    /**
     * @brief Extracts a substring trapped between two delimiter strings.
     * @return A new cb_string_t containing the caught text.
     */
    cb_string_t* (*catch_in_range)(const cb_string_t* self, const char* start_str, const char* end_str);
};

/**
 * @brief The global namespace instance. Use this to access the string functions.
 * ### Example Usage:
 * @code
 * cb_string_t* scpi_cmd = cb_str.create("MEAS:VOLT:DC?");
 * if (cb_str.starts_with(scpi_cmd, "MEAS")) {
 * cb_str.append(scpi_cmd, " 10, 0.01");
 * }
 * cb_str.free(scpi_cmd);
 * @endcode
 */
LIBCTOOL_API extern const struct cb_str_namespace cb_str;

#endif    // CB_STR_H