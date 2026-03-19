#include "cb_vector.h"
#include <stdlib.h>
#include <string.h>

// ============================================================================
// Internal Helpers
// ============================================================================
static void impl_grow(cb_vector_t* self, size_t min_cap) {
    if (min_cap <= self->capacity) return;
    
    size_t new_cap = self->capacity == 0 ? 4 : self->capacity * 2;
    if (new_cap < min_cap) new_cap = min_cap;
    
    void** new_items = (void**)realloc(self->items, new_cap * sizeof(void*));
    if (new_items) {
        self->items = new_items;
        self->capacity = new_cap;
    }
}

// ============================================================================
// Lifecycle
// ============================================================================
static cb_vector_t* impl_create() {
    cb_vector_t* self = (cb_vector_t*)malloc(sizeof(cb_vector_t));
    if (self) {
        self->items = NULL;
        self->length = 0;
        self->capacity = 0;
    }
    return self;
}

static void impl_free(cb_vector_t* self) {
    if (self) {
        if (self->items) free(self->items);
        free(self);
    }
}

// ============================================================================
// Capacity & Access
// ============================================================================
static size_t impl_size(const cb_vector_t* self) {
    return self ? self->length : 0;
}

static bool impl_empty(const cb_vector_t* self) {
    return !self || self->length == 0;
}

static void impl_reserve(cb_vector_t* self, size_t new_cap) {
    if (self && new_cap > self->capacity) {
        void** new_items = (void**)realloc(self->items, new_cap * sizeof(void*));
        if (new_items) {
            self->items = new_items;
            self->capacity = new_cap;
        }
    }
}

static void* impl_at(const cb_vector_t* self, size_t index) {
    if (!self || index >= self->length) return NULL;
    return self->items[index];
}

static void* impl_front(const cb_vector_t* self) {
    return impl_at(self, 0);
}

static void* impl_back(const cb_vector_t* self) {
    if (!self || self->length == 0) return NULL;
    return self->items[self->length - 1];
}

// ============================================================================
// Modifiers
// ============================================================================
static void impl_push_back(cb_vector_t* self, void* item) {
    if (!self) return;
    if (self->length == self->capacity) {
        impl_grow(self, self->length + 1);
    }
    self->items[self->length++] = item;
}

static void impl_pop_back(cb_vector_t* self) {
    if (self && self->length > 0) {
        self->length--;
    }
}

static void impl_insert(cb_vector_t* self, size_t index, void* item) {
    if (!self) return;
    if (index > self->length) index = self->length;

    if (self->length == self->capacity) {
        impl_grow(self, self->length + 1);
    }

    // Shift elements to the right to make room
    if (index < self->length) {
        memmove(&self->items[index + 1], &self->items[index], 
                (self->length - index) * sizeof(void*));
    }
    
    self->items[index] = item;
    self->length++;
}

static void impl_erase(cb_vector_t* self, size_t index) {
    if (!self || index >= self->length) return;

    // Shift elements to the left to fill the gap
    if (index < self->length - 1) {
        memmove(&self->items[index], &self->items[index + 1], 
                (self->length - index - 1) * sizeof(void*));
    }
    self->length--;
}

static void impl_clear(cb_vector_t* self) {
    if (self) self->length = 0;
}

// ============================================================================
// Namespace Definition
// ============================================================================
const struct cb_vector_namespace cb_vector = {
    .create = impl_create,
    .free = impl_free,
    .size = impl_size,
    .empty = impl_empty,
    .reserve = impl_reserve,
    .at = impl_at,
    .front = impl_front,
    .back = impl_back,
    .push_back = impl_push_back,
    .pop_back = impl_pop_back,
    .insert = impl_insert,
    .erase = impl_erase,
    .clear = impl_clear
};