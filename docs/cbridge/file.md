# cbridge::file Module Documentation

The `cbridge::file` module provides high-level filesystem utilities for pure C. It integrates with `cbridge::str` and `cbridge::vector` to provide a modern experience for file handling.

## API Reference

| Function                        | Description                                       |
| :------------------------------ | :------------------------------------------------ |
| `exists(path)`                  | Returns `true` if the file exists.                |
| `get_size(path)`                | Returns the file size in bytes.                   |
| `read_all(path)`                | Reads a whole file and returns a `string_t*`.    |
| `read_lines(path, startLine, endLine)` | Reads a range of lines (1-indexed, inclusive). Pass `0` for either parameter to default to line `1`. |
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

### Reading a Range of Lines
```c
// Read lines 10 to 20 (1-indexed, inclusive) from a large log file
string_t* lines = cbridge_file.read_lines("logfile.txt", 10, 20);
if (!cbridge_string.empty(lines)) {
    printf("Lines 10-20:\n%s\n", cbridge_string.c_str(lines));
}
cbridge_string.free(lines);

// Edge case handling:
// - If endLine > total lines, reads until EOF
// - If startLine > endLine (after normalization), returns empty string
// - If startLine is 0, it is internally treated as 1
// - If endLine is 0, it is internally treated as 1
// - Negative values are not allowed (size_t is unsigned)
```
