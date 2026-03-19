#include "cb_str.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================================================
// Internal Helper
// ============================================================================
static void impl_append_len(cb_string_t* self, const char* str, size_t len) {
    if (!self || !str || len == 0) return;
    size_t required_capacity = self->length + len;
    if (required_capacity > self->capacity) {
        size_t new_cap = self->capacity * 2;
        if (new_cap < required_capacity) new_cap = required_capacity;
        char* new_data = (char*)realloc(self->data, new_cap + 1);
        if (new_data) {
            self->data = new_data;
            self->capacity = new_cap;
        } else return;
    }
    memcpy(self->data + self->length, str, len);
    self->length += len;
    self->data[self->length] = '\0';
}

// ============================================================================
// Lifecycle & State
// ============================================================================
static cb_string_t* impl_create(const char* init_str) {
    cb_string_t* self = (cb_string_t*)malloc(sizeof(cb_string_t));
    if (!self) return NULL;
    self->length = init_str ? strlen(init_str) : 0;
    self->capacity = self->length > 0 ? self->length * 2 : 16;
    self->data = (char*)malloc(self->capacity + 1);
    if (self->data) {
        if (init_str) strcpy(self->data, init_str);
        else self->data[0] = '\0';
    }
    return self;
}

static void impl_free(cb_string_t* self) {
    if (self) {
        if (self->data) free(self->data);
        free(self);
    }
}

static void impl_reserve(cb_string_t* self, size_t new_cap) {
    if (!self || new_cap <= self->capacity) return;
    char* new_data = (char*)realloc(self->data, new_cap + 1);
    if (new_data) {
        self->data = new_data;
        self->capacity = new_cap;
    }
}

static void impl_clear(cb_string_t* self) {
    if (self && self->data) {
        self->length = 0;
        self->data[0] = '\0';
    }
}

static bool impl_empty(const cb_string_t* self) {
    return !self || self->length == 0;
}

static size_t impl_length(const cb_string_t* self) {
    return self ? self->length : 0;
}

static const char* impl_c_str(const cb_string_t* self) {
    return (self && self->data) ? self->data : "";
}

// ============================================================================
// Modifiers
// ============================================================================
static void impl_append(cb_string_t* self, const char* suffix) {
    if (suffix) impl_append_len(self, suffix, strlen(suffix));
}

static void impl_insert(cb_string_t* self, size_t pos, const char* str) {
    if (!self || !str) return;
    if (pos > self->length) pos = self->length; // Clamp to end
    
    size_t str_len = strlen(str);
    if (str_len == 0) return;

    // Ensure capacity
    if (self->length + str_len > self->capacity) {
        impl_reserve(self, self->length + str_len + (self->capacity / 2)); 
    }

    // Shift existing data to the right
    memmove(self->data + pos + str_len, self->data + pos, self->length - pos + 1);
    // Copy new string into the gap
    memcpy(self->data + pos, str, str_len);
    self->length += str_len;
}

static void impl_erase(cb_string_t* self, size_t pos, size_t len) {
    if (!self || pos >= self->length || len == 0) return;
    
    // Clamp length to avoid out-of-bounds
    if (pos + len > self->length) len = self->length - pos;

    // Shift data left
    memmove(self->data + pos, self->data + pos + len, self->length - pos - len + 1);
    self->length -= len;
}

static void impl_trim(cb_string_t* self) {
    if (!self || self->length == 0) return;

    size_t start = 0;
    while (start < self->length && isspace((unsigned char)self->data[start])) start++;

    if (start == self->length) {
        impl_clear(self);
        return;
    }

    size_t end = self->length - 1;
    while (end > start && isspace((unsigned char)self->data[end])) end--;

    size_t new_len = end - start + 1;
    if (start > 0) {
        memmove(self->data, self->data + start, new_len);
    }
    self->data[new_len] = '\0';
    self->length = new_len;
}

static void impl_replace_all(cb_string_t* self, const char* search_str, const char* replace_str) {
    if (!self || !self->data || !search_str || !replace_str) return;
    size_t search_len = strlen(search_str);
    if (search_len == 0) return;

    cb_string_t* temp = impl_create("");
    const char* current = self->data;
    char* match;

    while ((match = strstr(current, search_str)) != NULL) {
        impl_append_len(temp, current, match - current);
        impl_append(temp, replace_str);
        current = match + search_len;
    }
    impl_append(temp, current);

    free(self->data);
    self->data = temp->data;
    self->length = temp->length;
    self->capacity = temp->capacity;
    free(temp); 
}

// ============================================================================
// Operations
// ============================================================================
static int impl_compare(const cb_string_t* self, const char* other) {
    if (!self && !other) return 0;
    if (!self) return -1;
    if (!other) return 1;
    return strcmp(self->data, other);
}

static bool impl_starts_with(const cb_string_t* self, const char* prefix) {
    if (!self || !prefix) return false;
    size_t prefix_len = strlen(prefix);
    if (prefix_len > self->length) return false;
    return strncmp(self->data, prefix, prefix_len) == 0;
}

static bool impl_ends_with(const cb_string_t* self, const char* suffix) {
    if (!self || !suffix) return false;
    size_t suffix_len = strlen(suffix);
    if (suffix_len > self->length) return false;
    return strcmp(self->data + (self->length - suffix_len), suffix) == 0;
}

static size_t impl_find(const cb_string_t* self, const char* target, size_t start_pos) {
    if (!self || !target || start_pos >= self->length) return CB_STR_NPOS;
    const char* match = strstr(self->data + start_pos, target);
    if (match) {
        return (size_t)(match - self->data);
    }
    return CB_STR_NPOS;
}

static cb_string_t* impl_substr(const cb_string_t* self, size_t pos, size_t len) {
    if (!self || pos >= self->length) return impl_create("");
    if (pos + len > self->length) len = self->length - pos;
    
    cb_string_t* result = impl_create("");
    impl_append_len(result, self->data + pos, len);
    return result;
}

static cb_string_t* impl_catch_in_range(const cb_string_t* self, const char* start_str, const char* end_str) {
    if (!self || !self->data || !start_str || !end_str) return impl_create("");
    const char* start_pos = strstr(self->data, start_str);
    if (!start_pos) return impl_create("");
    start_pos += strlen(start_str);
    const char* end_pos = strstr(start_pos, end_str);
    if (!end_pos) return impl_create("");
    
    cb_string_t* result = impl_create("");
    impl_append_len(result, start_pos, end_pos - start_pos);
    return result;
}

// ============================================================================
// Global Namespace Instance Definition
// ============================================================================
const struct cb_str_namespace cb_str = {
    .create = impl_create,
    .free = impl_free,
    .reserve = impl_reserve,
    .clear = impl_clear,
    .empty = impl_empty,
    .length = impl_length,
    .c_str = impl_c_str,
    .append = impl_append,
    .insert = impl_insert,
    .erase = impl_erase,
    .trim = impl_trim,
    .replace_all = impl_replace_all,
    .compare = impl_compare,
    .starts_with = impl_starts_with,
    .ends_with = impl_ends_with,
    .find = impl_find,
    .substr = impl_substr,
    .catch_in_range = impl_catch_in_range
};