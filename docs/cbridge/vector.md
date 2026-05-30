# /vector Module Documentation

The `/vector` module provides a generic dynamic array (vector) for C, mimicking `std::vector<void*>`. It is designed to store pointers to any data type, including custom structs and `/string_t` objects.

## API Reference

| Function                              | Description                                  |
| :------------------------------------ | :------------------------------------------- |
| `create()`                            | Allocates a new empty vector.                |
| `free(/vector_t*)`                  | Frees the vector container (not the items).  |
| `size(/vector_t*)`                  | Returns the number of elements.              |
| `empty(/vector_t*)`                 | Returns `true` if size is 0.                 |
| `reserve(/vector_t*, size_t)`       | Pre-allocates pointer slots.                 |
| `at(/vector_t*, size_t)`            | Returns the `void*` at the index.            |
| `front(/vector_t*)`                 | Returns the first element.                   |
| `back(/vector_t*)`                  | Returns the last element.                    |
| `push_back(/vector_t*, void*)`      | Adds a pointer to the end.                   |
| `pop_back(/vector_t*)`              | Removes the last pointer (does not free it). |
| `insert(/vector_t*, size_t, void*)` | Inserts a pointer at the index.              |
| `erase(/vector_t*, size_t)`         | Removes the pointer at the index.            |
| `clear(/vector_t*)`                 | Resets size to 0.                            |

## Usage Examples

### Storing Integers
```c
/vector_t* vec = /vector.create();

for (int i = 0; i < 10; i++) {
    int* val = malloc(sizeof(int));
    *val = i * 10;
    /vector.push_back(vec, val);
}

int* middle = (int*)/vector.at(vec, 5);
printf("Value at index 5: %d\n", *middle);

// Cleanup: must free individual pointers
for (size_t i = 0; i < /vector.size(vec); i++) {
    free(/vector.at(vec, i));
}
/vector.free(vec);
```

### Managing a Collection of Strings
This is a common pattern for log management or storing list of serial numbers.

```c
/vector_t* list = /vector.create();

/vector.push_back(list, /str.create("SN_001"));
/vector.push_back(list, /str.create("SN_002"));

// Add a prefix to all elements
for (size_t i = 0; i < /vector.size(list); i++) {
    /string_t* s = (/string_t*)/vector.at(list, i);
    /str.insert(s, 0, "PROD_");
}

// Memory Cleanup for Vector of /strings
for (size_t i = 0; i < /vector.size(list); i++) {
    /str.free((/string_t*)/vector.at(list, i));
}
/vector.free(list);
```

## Memory Ownership Rules

1.  **The Container**: `/vector.free()` only deallocates the internal array of pointers.
2.  **The Content**: If you used `malloc` or `/str.create()` to generate the pointers you pushed into the vector, **you** are responsible for iterating through the vector and freeing those items before destroying the vector itself.
3.  **Out-of-bounds**: `/vector.at()` returns `NULL` if the index is invalid. Always check for `NULL` in critical applications.