# cb_str Module Documentation

The `cb_str` module provides a dynamic string implementation for pure C that mimics the behavior and ease of use of C++ `std::string`. It uses a "namespace" pattern to group functions under a global `cb_str` object.

## API Reference

| Function                                                 | Description                                         |
| :------------------------------------------------------- | :-------------------------------------------------- |
| `create(const char*)`                                    | Allocates a new dynamic string.                     |
| `free(cb_string_t*)`                                     | Deallocates the string and its buffer.              |
| `reserve(cb_string_t*, size_t)`                          | Pre-allocates memory to avoid future reallocations. |
| `clear(cb_string_t*)`                                    | Resets length to 0 without freeing memory.          |
| `empty(cb_string_t*)`                                    | Returns `true` if length is 0.                      |
| `length(cb_string_t*)`                                   | Returns the current number of characters.           |
| `c_str(cb_string_t*)`                                    | Returns a null-terminated `const char*`.            |
| `append(cb_string_t*, const char*)`                      | Concatenates a C-string to the end.                 |
| `insert(cb_string_t*, size_t, const char*)`              | Inserts a C-string at the specified index.          |
| `erase(cb_string_t*, size_t, size_t)`                    | Removes a range of characters.                      |
| `trim(cb_string_t*)`                                     | Removes leading and trailing whitespace.            |
| `replace_all(cb_string_t*, const char*, const char*)`    | Replaces all occurrences of a substring.            |
| `compare(cb_string_t*, const char*)`                     | Standard string comparison (0 if equal).            |
| `starts_with(cb_string_t*, const char*)`                 | Returns `true` if string starts with prefix.        |
| `ends_with(cb_string_t*, const char*)`                   | Returns `true` if string ends with suffix.          |
| `find(cb_string_t*, const char*, size_t)`                | Returns index of first match or `CB_STR_NPOS`.      |
| `substr(cb_string_t*, size_t, size_t)`                   | Returns a **new** string containing the range.      |
| `catch_in_range(cb_string_t*, const char*, const char*)` | Extracts text between two delimiters.               |

## Usage Examples

### Initialization and Basic Manipulation
```c
cb_string_t* s = cb_str.create("  SCPI:MEAS:VOLT ");
cb_str.trim(s);                    // "SCPI:MEAS:VOLT"
cb_str.append(s, ":DC?");          // "SCPI:MEAS:VOLT:DC?"
cb_str.insert(s, 5, "CH1:");       // "SCPI:CH1:MEAS:VOLT:DC?"

printf("Length: %zu\n", cb_str.length(s));
cb_str.free(s);
```

### Search and Extraction
```c
cb_string_t* raw = cb_str.create("DATA:1.234;STATUS:OK");
size_t pos = cb_str.find(raw, "STATUS", 0);

if (pos != CB_STR_NPOS) {
    cb_string_t* status = cb_str.substr(raw, pos + 7, 2);
    if (cb_str.compare(status, "OK") == 0) {
        // Logic for success
    }
    cb_str.free(status);
}

// Quick extraction between delimiters
cb_string_t* val = cb_str.catch_in_range(raw, "DATA:", ";");
printf("Value: %s\n", cb_str.c_str(val));

cb_str.free(val);
cb_str.free(raw);
```
