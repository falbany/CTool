/**
 * @file vector.h
 * @author Florent ALBANY - FAL
 * @brief Pure C imitation of C++ std::vector for generic pointer collections.
 * @version 1.0
 * @date 2026-03-19
 */

#ifndef CBRIDGE_VECTOR_H
#define CBRIDGE_VECTOR_H

#include <stddef.h>
#include <stdbool.h>
#include "../internal/libctool.h"

/**
 * @struct cbridge_vector_t
 * @brief Dynamic array structure mimicking std::vector.
 * Stores pointers (void*) to allow for generic data collections.
 */
typedef struct {
    void** items;       ///< Array of pointers to the stored elements.
    size_t length;      ///< Number of elements currently in the vector.
    size_t capacity;    ///< Total allocated storage space.
} cbridge_vector_t;

/**
 * @struct cbridge_vector_namespace
 * @brief Function pointer table acting as the "cbridge::vector" namespace.
 */
struct cbridge_vector_namespace {
    // --- LIFECYCLE ---

    /**
     * @brief Initializes a new vector.
     * @return Pointer to a new cbridge_vector_t. Must be destroyed with free().
     */
    cbridge_vector_t* (*create)();

    /**
     * @brief Frees the vector container.
     * @note This does NOT free the individual items if they were dynamically allocated.
     * User must iterate and free items manually or use a cleanup loop.
     */
    void (*free)(cbridge_vector_t* self);

    // --- CAPACITY ---

    /** @brief Returns number of elements. */
    size_t (*size)(const cbridge_vector_t* self);

    /** @brief Returns true if size is 0. */
    bool (*empty)(const cbridge_vector_t* self);

    /** @brief Pre-allocates memory to prevent multiple reallocations. */
    void (*reserve)(cbridge_vector_t* self, size_t new_cap);

    // --- ELEMENT ACCESS ---

    /**
     * @brief Access element at index.
     * @return The pointer stored at index, or NULL if out of bounds.
     */
    void* (*at)(const cbridge_vector_t* self, size_t index);

    /** @brief Returns the first element. */
    void* (*front)(const cbridge_vector_t* self);

    /** @brief Returns the last element. */
    void* (*back)(const cbridge_vector_t* self);

    // --- MODIFIERS ---

    /**
     * @brief Adds an item to the end of the vector.
     * @param item Pointer to the data to store.
     */
    void (*push_back)(cbridge_vector_t* self, void* item);

    /** @brief Removes the last element. */
    void (*pop_back)(cbridge_vector_t* self);

    /**
     * @brief Inserts an item at a specific index.
     */
    void (*insert)(cbridge_vector_t* self, size_t index, void* item);

    /**
     * @brief Removes an item at a specific index and shifts remaining items.
     */
    void (*erase)(cbridge_vector_t* self, size_t index);

    /** @brief Clears all pointers from the vector (sets length to 0). */
    void (*clear)(cbridge_vector_t* self);
};

/**
 * @brief Global access to vector functionalities.
 * Example: cbridge_vector.push_back(my_vec, my_data);
 */
LIBCTOOL_API extern const struct cbridge_vector_namespace cbridge_vector;

#endif    // CBRIDGE_VECTOR_H