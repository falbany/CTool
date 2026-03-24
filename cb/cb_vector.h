/**
 * @file cb_vector.h
 * @author Florent ALBANY - FAL
 * @brief Pure C imitation of C++ std::vector for generic pointer collections.
 * @version 1.0
 * @date 2026-03-19
 */

#ifndef CB_VECTOR_H
#define CB_VECTOR_H

#include <stddef.h>
#include <stdbool.h>
#include "../libctool.h"

/**
 * @struct cb_vector_t
 * @brief Dynamic array structure mimicking std::vector.
 * Stores pointers (void*) to allow for generic data collections.
 */
typedef struct {
    void** items;       ///< Array of pointers to the stored elements.
    size_t length;      ///< Number of elements currently in the vector.
    size_t capacity;    ///< Total allocated storage space.
} cb_vector_t;

/**
 * @struct cb_vector_namespace
 * @brief Function pointer table acting as the "cb::vector" namespace.
 */
struct cb_vector_namespace {
    // --- LIFECYCLE ---

    /**
     * @brief Initializes a new vector.
     * @return Pointer to a new cb_vector_t. Must be destroyed with free().
     */
    cb_vector_t* (*create)();

    /**
     * @brief Frees the vector container.
     * @note This does NOT free the individual items if they were dynamically allocated.
     * User must iterate and free items manually or use a cleanup loop.
     */
    void (*free)(cb_vector_t* self);

    // --- CAPACITY ---

    /** @brief Returns number of elements. */
    size_t (*size)(const cb_vector_t* self);

    /** @brief Returns true if size is 0. */
    bool (*empty)(const cb_vector_t* self);

    /** @brief Pre-allocates memory to prevent multiple reallocations. */
    void (*reserve)(cb_vector_t* self, size_t new_cap);

    // --- ELEMENT ACCESS ---

    /**
     * @brief Access element at index.
     * @return The pointer stored at index, or NULL if out of bounds.
     */
    void* (*at)(const cb_vector_t* self, size_t index);

    /** @brief Returns the first element. */
    void* (*front)(const cb_vector_t* self);

    /** @brief Returns the last element. */
    void* (*back)(const cb_vector_t* self);

    // --- MODIFIERS ---

    /**
     * @brief Adds an item to the end of the vector.
     * @param item Pointer to the data to store.
     */
    void (*push_back)(cb_vector_t* self, void* item);

    /** @brief Removes the last element. */
    void (*pop_back)(cb_vector_t* self);

    /**
     * @brief Inserts an item at a specific index.
     */
    void (*insert)(cb_vector_t* self, size_t index, void* item);

    /**
     * @brief Removes an item at a specific index and shifts remaining items.
     */
    void (*erase)(cb_vector_t* self, size_t index);

    /** @brief Clears all pointers from the vector (sets length to 0). */
    void (*clear)(cb_vector_t* self);
};

/**
 * @brief Global access to vector functionalities.
 * Example: cb_vector.push_back(my_vec, my_data);
 */
LIBCTOOL_API extern const struct cb_vector_namespace cb_vector;

#endif    // CB_VECTOR_H