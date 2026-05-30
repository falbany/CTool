# cbridge::file Module Documentation

The `cbridge::file` module provides high-level filesystem utilities for pure C. It integrates with `cbridge::str` and `cbridge::vector` to provide a modern experience for file handling.

## API Reference

| Function                        | Description                                       |
| :------------------------------ | :------------------------------------------------ |
| `exists(path)`                  | Returns `true` if the file exists.                |
| `get_size(path)`                | Returns the file size in bytes.                   |
| `read_all(path)`                | Reads a whole file and returns a `string_t*`.    |
| `get_working_directory()`       | Returns the current app path as a `string_t*`.   |
| `get_parameter(path, key, sep)` | Parses a config file for a specific key (ignores comments). |
| `get_files(dir, pref, suff)`    | Returns a `vector_t*` of filenames.              |
| `remove(path)`                  | Deletes a file from the disk.                     |

## Usage Examples

### Reading a Configuration Parameter
```c
// Supports comments starting with //, #, *, ;, or !.
string_t* ip = cbridge_file.get_parameter("config.txt", "INSTR_IP", "=");
if (!cbridge_string.empty(ip)) {
    printf("Connecting to: %s\n", cbridge_string.c_str(ip));
}
cbridge_string.free(ip);
```

### Listing Log Files
```c
// List all .csv files starting with "Lot42"
vector_t* logs = cbridge_file.get_files("./data", "Lot42", ".csv");

for (size_t i = 0; i < cbridge_vector.size(logs); i++) {
    string_t* filename = (string_t*)cbridge_vector.at(logs, i);
    printf("Found log: %s\n", cbridge_string.c_str(filename));
    
    // Cleanup items inside loop if applicable
    cbridge_string.free(filename);
}
cbridge_vector.free(logs);
```
