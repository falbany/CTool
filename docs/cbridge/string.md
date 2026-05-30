# cbridge_string Module Documentation

The `cbridge_string` module provides a dynamic string implementation for pure C that mimics the behavior and ease of use of C++ `std::string`. It uses a "namespace" pattern to group functions under a global `cbridge_string` object.

## API Reference

| Function                                                 | Description                                         |
| :------------------------------------------------------- | :-------------------------------------------------- |
| `create(const char*)`                                    | Allocates a new dynamic string.                     |
| `free(string_t*)`                                     | Deallocates the string and its buffer.              |
| `reserve(string_t*, size_t)`                          | Pre-allocates memory to avoid future reallocations. |
| `clear(string_t*)`                                    | Resets length to 0 without freeing memory.          |
| `empty(string_t*)`                                    | Returns `true` if length is 0.                      |
| `length(string_t*)`                                   | Returns the current number of characters.           |
| `c_str(string_t*)`                                    | Returns a null-terminated `const char*`.            |
| `append(string_t*, const char*)`                      | Concatenates a C-string to the end.                 |
| `insert(string_t*, size_t, const char*)`              | Inserts a C-string at the specified index.          |
| `erase(string_t*, size_t, size_t)`                    | Removes a range of characters.                      |
| `trim(string_t*)`                                     | Removes leading and trailing whitespace.            |
| `replace_all(string_t*, const char*, const char*)`    | Replaces all occurrences of a substring.            |
| `compare(string_t*, const char*)`                     | Standard string comparison (0 if equal).            |
| `starts_with(string_t*, const char*)`                 | Returns `true` if string starts with prefix.        |
| `ends_with(string_t*, const char*)`                   | Returns `true` if string ends with suffix.          |
| `find(string_t*, const char*, size_t)`                | Returns index of first match or `CB_STR_NPOS`.      |
| `substr(string_t*, size_t, size_t)`                   | Returns a **new** string containing the range.      |
| `catch_in_range(string_t*, const char*, const char*)` | Extracts text between two delimiters.               |
| `split(string_t*, const char*)`                       | Splits the string by a delimiter into an array.     |

### Standalone Utilities

| Function                              | Description                                          |
| :------------------------------------ | :--------------------------------------------------- |
| `cbridge_str_parts_free(string_parts_t*)`  | Frees all parts and the array from a split result.   |

## Usage Examples

### Initialization and Basic Manipulation
```c
string_t* s = cbridge_string.create("  SCPI:MEAS:VOLT ");
cbridge_string.trim(s);                    // "SCPI:MEAS:VOLT"
cbridge_string.append(s, ":DC?");          // "SCPI:MEAS:VOLT:DC?"
cbridge_string.insert(s, 5, "CH1:");       // "SCPI:CH1:MEAS:VOLT:DC?"

printf("Length: %zu\n", cbridge_string.length(s));
cbridge_string.free(s);
```

### Search and Extraction
```c
string_t* raw = cbridge_string.create("DATA:1.234;STATUS:OK");
size_t pos = /str.find(raw, "STATUS", 0);

if (pos != CB_STR_NPOS) {
    /string_t* status = /str.substr(raw, pos + 7, 2);
    if (/str.compare(status, "OK") == 0) {
        // Logic for success
    }
    /str.free(status);
}

// Quick extraction between delimiters
/string_t* val = /str.catch_in_range(raw, "DATA:", ";");
printf("Value: %s\n", /str.c_str(val));

/str.free(val);
/str.free(raw);
```

### Splitting Strings
```c
/string_t* csv = /str.create("apple,banana,cherry,fig");
/str_parts_t parts = /str.split(csv, ",");

for (size_t i = 0; i < parts.count; i++) {
    printf("Part %zu: %s\n", i, /str.c_str(parts.parts[i]));
}

// Clean up the entire split result in one call
/str_parts_free(&parts);
/str.free(csv);
```

### Splitting — Edge Cases
```c
// Trailing / consecutive delimiters produce empty parts
/string_t* s1 = /str.create("a,,b,");
/str_parts_t p1 = /str.split(s1, ",");
// p1.count == 4  →  "a"  ""  "b"  ""

/str_parts_free(&p1);
/str.free(s1);

// NULL or empty delimiter returns the whole string as a single part
/string_t* s2 = /str.create("hello");
/str_parts_t p2 = /str.split(s2, "");
// p2.count == 1  →  "hello"

/str_parts_free(&p2);
/str.free(s2);
```
