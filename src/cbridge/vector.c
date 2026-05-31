#include "vector.h"
#include <stdlib.h>
#include <string.h>

// ============================================================================
// Internal Helpers
// ============================================================================
static void impl_grow(vector_t* self, size_t min_cap) {
    if (min_cap <= self->capacity) return;

    size_t newCapacity = self->capacity == 0 ? 4 : self->capacity * 2;
    if (newCapacity < min_cap) newCapacity = min_cap;

    void** new_items = (void**)realloc(self->items, newCapacity * sizeof(void*));
    if (new_items) {
        self->items    = new_items;
        self->capacity = newCapacity;
    }
}

// ============================================================================
// Lifecycle
// ============================================================================
static vector_t* impl_create() {
    vector_t* self = (vector_t*)malloc(sizeof(vector_t));
    if (self) {
        self->items    = NULL;
        self->length   = 0;
        self->capacity = 0;
    }
    return self;
}

static void impl_free(vector_t* self) {
    if (self) {
        if (self->items) free(self->items);
        free(self);
    }
}

// ============================================================================
// Capacity & Access
// ============================================================================
static size_t impl_size(const vector_t* self) { return self ? self->length : 0; }

static bool impl_empty(const vector_t* self) { return !self || self->length == 0; }

static void impl_reserve(vector_t* self, size_t newCapacity) {
    if (self && newCapacity > self->capacity) {
        void** new_items = (void**)realloc(self->items, newCapacity * sizeof(void*));
        if (new_items) {
            self->items    = new_items;
            self->capacity = newCapacity;
        }
    }
}

static void* impl_at(const vector_t* self, size_t index) {
    if (!self || index >= self->length) return NULL;
    return self->items[index];
}

static void* impl_front(const vector_t* self) { return impl_at(self, 0); }

static void* impl_back(const vector_t* self) {
    if (!self || self->length == 0) return NULL;
    return self->items[self->length - 1];
}

// ============================================================================
// Modifiers
// ============================================================================
static void impl_push_back(vector_t* self, void* item) {
    if (!self) return;
    if (self->length == self->capacity) {
        impl_grow(self, self->length + 1);
    }
    self->items[self->length++] = item;
}

static void impl_pop_back(vector_t* self) {
    if (self && self->length > 0) {
        self->length--;
    }
}

static void impl_insert(vector_t* self, size_t index, void* item) {
    if (!self) return;
    if (index > self->length) index = self->length;

    if (self->length == self->capacity) {
        impl_grow(self, self->length + 1);
    }

    // Shift elements to the right to make room
    if (index < self->length) {
        memmove(&self->items[index + 1], &self->items[index], (self->length - index) * sizeof(void*));
    }

    self->items[index] = item;
    self->length++;
}

static void impl_erase(vector_t* self, size_t index) {
    if (!self || index >= self->length) return;

    // Shift elements to the left to fill the gap
    if (index < self->length - 1) {
        memmove(&self->items[index], &self->items[index + 1], (self->length - index - 1) * sizeof(void*));
    }
    self->length--;
}

static void impl_clear(vector_t* self) {
    if (self) self->length = 0;
}

// ============================================================================
// Namespace Definition
// ============================================================================
const struct cbridge_vector_namespace cbridge_vector = {.create    = impl_create,
                                                        .free      = impl_free,
                                                        .size      = impl_size,
                                                        .empty     = impl_empty,
                                                        .reserve   = impl_reserve,
                                                        .at        = impl_at,
                                                        .front     = impl_front,
                                                        .back      = impl_back,
                                                        .push_back = impl_push_back,
                                                        .pop_back  = impl_pop_back,
                                                        .insert    = impl_insert,
                                                        .erase     = impl_erase,
                                                        .clear     = impl_clear};