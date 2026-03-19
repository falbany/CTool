# cb_vector Module Documentation

The `cb_vector` module provides a generic dynamic array (vector) for C, mimicking `std::vector<void*>`. It is designed to store pointers to any data type, including custom structs and `cb_string_t` objects.

## API Reference

| Function                              | Description                                  |
| :------------------------------------ | :------------------------------------------- |
| `create()`                            | Allocates a new empty vector.                |
| `free(cb_vector_t*)`                  | Frees the vector container (not the items).  |
| `size(cb_vector_t*)`                  | Returns the number of elements.              |
| `empty(cb_vector_t*)`                 | Returns `true` if size is 0.                 |
| `reserve(cb_vector_t*, size_t)`       | Pre-allocates pointer slots.                 |
| `at(cb_vector_t*, size_t)`            | Returns the `void*` at the index.            |
| `front(cb_vector_t*)`                 | Returns the first element.                   |
| `back(cb_vector_t*)`                  | Returns the last element.                    |
| `push_back(cb_vector_t*, void*)`      | Adds a pointer to the end.                   |
| `pop_back(cb_vector_t*)`              | Removes the last pointer (does not free it). |
| `insert(cb_vector_t*, size_t, void*)` | Inserts a pointer at the index.              |
| `erase(cb_vector_t*, size_t)`         | Removes the pointer at the index.            |
| `clear(cb_vector_t*)`                 | Resets size to 0.                            |

## Usage Examples

### Storing Integers
```c
cb_vector_t* vec = cb_vector.create();

for (int i = 0; i < 10; i++) {
    int* val = malloc(sizeof(int));
    *val = i * 10;
    cb_vector.push_back(vec, val);
}

int* middle = (int*)cb_vector.at(vec, 5);
printf("Value at index 5: %d\n", *middle);

// Cleanup: must free individual pointers
for (size_t i = 0; i < cb_vector.size(vec); i++) {
    free(cb_vector.at(vec, i));
}
cb_vector.free(vec);
```

### Managing a Collection of Strings
This is a common pattern for log management or storing list of serial numbers.

```c
cb_vector_t* list = cb_vector.create();

cb_vector.push_back(list, cb_str.create("SN_001"));
cb_vector.push_back(list, cb_str.create("SN_002"));

// Add a prefix to all elements
for (size_t i = 0; i < cb_vector.size(list); i++) {
    cb_string_t* s = (cb_string_t*)cb_vector.at(list, i);
    cb_str.insert(s, 0, "PROD_");
}

// Memory Cleanup for Vector of cb_strings
for (size_t i = 0; i < cb_vector.size(list); i++) {
    cb_str.free((cb_string_t*)cb_vector.at(list, i));
}
cb_vector.free(list);
```

## Memory Ownership Rules

1.  **The Container**: `cb_vector.free()` only deallocates the internal array of pointers.
2.  **The Content**: If you used `malloc` or `cb_str.create()` to generate the pointers you pushed into the vector, **you** are responsible for iterating through the vector and freeing those items before destroying the vector itself.
3.  **Out-of-bounds**: `cb_vector.at()` returns `NULL` if the index is invalid. Always check for `NULL` in critical applications.