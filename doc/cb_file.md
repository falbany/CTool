# cb_file Module Documentation

The `cb_file` module provides high-level filesystem utilities. It integrates seamlessly with `cb_str` and `cb_vector` to provide a modern, C++-like experience for file handling in C.

## API Reference

| Function                        | Description                                       |
| :------------------------------ | :------------------------------------------------ |
| `exists(path)`                  | Returns `true` if the file exists.                |
| `get_size(path)`                | Returns the file size in bytes.                   |
| `read_all(path)`                | Reads a whole file and returns a `cb_string_t*`.  |
| `get_working_directory()`       | Returns the current app path as a `cb_string_t*`. |
| `get_parameter(path, key, sep)` | Parses a config file for a specific key.          |
| `get_files(dir, pref, suff)`    | Returns a `cb_vector_t*` of filenames.            |

## Usage Examples

### Reading a Configuration Parameter
```c
cb_string_t* ip = cb_file.get_parameter("config.txt", "INSTR_IP", "=");
if (!cb_str.empty(ip)) {
    printf("Connecting to: %s\n", cb_str.c_str(ip));
}
cb_str.free(ip);
```

### Listing Log Files
```c
// List all .csv files starting with "Lot42"
cb_vector_t* logs = cb_file.get_files("./data", "Lot42", ".csv");

for (size_t i = 0; i < cb_vector.size(logs); i++) {
    cb_string_t* filename = (cb_string_t*)cb_vector.at(logs, i);
    printf("Found log: %s\n", cb_str.c_str(filename));
    
    // Cleanup items inside loop
    cb_str.free(filename);
}
cb_vector.free(logs);
```
