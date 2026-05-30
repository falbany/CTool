# /file Module Documentation

The `/file` module provides high-level filesystem utilities. It integrates seamlessly with `/str` and `/vector` to provide a modern, C++-like experience for file handling in C.

## API Reference

| Function                        | Description                                       |
| :------------------------------ | :------------------------------------------------ |
| `exists(path)`                  | Returns `true` if the file exists.                |
| `get_size(path)`                | Returns the file size in bytes.                   |
| `read_all(path)`                | Reads a whole file and returns a `/string_t*`.  |
| `get_working_directory()`       | Returns the current app path as a `/string_t*`. |
| `get_parameter(path, key, sep)` | Parses a config file for a specific key.          |
| `get_files(dir, pref, suff)`    | Returns a `/vector_t*` of filenames.            |

## Usage Examples

### Reading a Configuration Parameter
```c
/string_t* ip = /file.get_parameter("config.txt", "INSTR_IP", "=");
if (!/str.empty(ip)) {
    printf("Connecting to: %s\n", /str.c_str(ip));
}
/str.free(ip);
```

### Listing Log Files
```c
// List all .csv files starting with "Lot42"
/vector_t* logs = /file.get_files("./data", "Lot42", ".csv");

for (size_t i = 0; i < /vector.size(logs); i++) {
    /string_t* filename = (/string_t*)/vector.at(logs, i);
    printf("Found log: %s\n", /str.c_str(filename));
    
    // Cleanup items inside loop
    /str.free(filename);
}
/vector.free(logs);
```
