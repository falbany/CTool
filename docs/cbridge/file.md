# cbridge::file Module Documentation

The `cbridge::file` module provides high-level filesystem utilities for pure C. It integrates with `cbridge::str` and `cbridge::vector` to provide a modern experience for file handling.

## API Reference

| Function                        | Description                                       |
| :------------------------------ | :------------------------------------------------ |
| `exists(path)`                  | Returns `true` if the file exists.                |
| `get_size(path)`                | Returns the file size in bytes.                   |
| `read_all(path)`                | Reads a whole file and returns a `string_t*`.    |
| `read_lines(path, startLine, endLine)` | Reads a range of lines (1-indexed, inclusive). Pass `0` for either parameter to default to line `1`. |
| `read_last_lines(path, count)`  | Reads the last `count` lines from a file.         |
| `get_working_directory()`       | Returns the current app path as a `string_t*`.   |
| `get_parameter(path, key, sep)` | Parses a config file for a specific key (ignores comments). |
| `get_files(dir, pref, suff)`    | Returns a `vector_t*` of filenames.              |
| `remove(path)`                  | Deletes a file from the disk.                     |
| `copy(src, dest)`               | Copies a file from `src` to `dest`, overwriting `dest` if it exists. |
| `move(src, dest)`               | Moves or renames a file from `src` to `dest`.    |
| `is_directory(path)`            | Returns `true` if the path exists and is a directory. |
| `create_directory(path)`        | Creates a single directory at the given path.    |
| `write_all(path, content)`      | Writes a string to a file, overwriting.          |
| `append_all(path, content)`     | Appends a string to a file, creating if needed.  |
| `get_extension(path)`           | Extracts the file extension (e.g., `.csv`).      |
| `get_filename(path)`            | Extracts the filename (basename) from a path.    |
| `get_directory(path)`           | Extracts the parent directory from a path.       |
| `get_basename(path)`            | Extracts the filename without its last extension.|
| `create_directories(path)`      | Recursively creates intermediate directories.    |
| `remove_directory(path)`        | Removes a single empty directory.                |

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

### Writing to a File
```c
// Write content to a file (creates or overwrites)
if (cbridge_file.write_all("output.txt", "Hello, World!")) {
    printf("File written successfully\n");
}

// NULL content is treated as empty string
cbridge_file.write_all("empty.txt", NULL);

// Handle NULL path
if (!cbridge_file.write_all(NULL, "content")) {
    printf("Error: NULL path returned false\n");
}
```

### Appending to a File
```c
// Append content to a file (creates if doesn't exist)
cbridge_file.append_all("log.txt", "New log entry\n");

// Append to existing file preserves previous content
cbridge_file.append_all("data.txt", "\nAdditional data\n");

// NULL path returns false
if (!cbridge_file.append_all(NULL, "content")) {
    printf("Error: NULL path returned false\n");
}
```

### Getting File Extension
```c
// Extract extension from a simple filename
string_t* ext = cbridge_file.get_extension("file.txt");
printf("Extension: %s\n", cbridge_string.c_str(ext));  // Output: .txt
cbridge_string.free(ext);

// Extract from path with directories
ext = cbridge_file.get_extension("/path/to/data.csv");
printf("Extension: %s\n", cbridge_string.c_str(ext));  // Output: .csv
cbridge_string.free(ext);

// No extension returns empty string
ext = cbridge_file.get_extension("Makefile");
if (cbridge_string.empty(ext)) {
    printf("No extension found\n");
}
cbridge_string.free(ext);

// Multiple dots - returns last extension
ext = cbridge_file.get_extension("archive.tar.gz");
printf("Extension: %s\n", cbridge_string.c_str(ext));  // Output: .gz
cbridge_string.free(ext);

// Hidden files (dotfile) have no extension
ext = cbridge_file.get_extension(".gitignore");
if (cbridge_string.empty(ext)) {
    printf("No extension for hidden file\n");
}
cbridge_string.free(ext);

// Windows paths work correctly
ext = cbridge_file.get_extension("C:\\Users\\file.log");
printf("Extension: %s\n", cbridge_string.c_str(ext));  // Output: .log
cbridge_string.free(ext);

// NULL path returns empty string
ext = cbridge_file.get_extension(NULL);
EXPECT_TRUE(cbridge_string.empty(ext));
cbridge_string.free(ext);
```

### Deleting a File
```c
// Delete a file from disk
if (cbridge_file.remove("temp_output.txt")) {
    printf("File deleted successfully\n");
}

// Returns false if file does not exist
if (!cbridge_file.remove("nonexistent.txt")) {
    printf("Failed to delete file (may not exist)\n");
}

// NULL path returns false
if (!cbridge_file.remove(NULL)) {
    printf("Error: NULL path returned false\n");
}
```

### Copying a File
```c
// Copy a file from source to destination
if (cbridge_file.copy("source.txt", "backup.txt")) {
    printf("File copied successfully\n");
}

// Overwrites destination if it already exists
cbridge_file.copy("new_data.csv", "data.csv");

// Returns false if source does not exist
if (!cbridge_file.copy("missing.txt", "dest.txt")) {
    printf("Failed to copy file\n");
}

// NULL source or destination returns false
cbridge_file.copy(NULL, "dest.txt");   // Returns false
cbridge_file.copy("source.txt", NULL); // Returns false
```

### Checking for a Directory
```c
// Check if a path is a directory
if (cbridge_file.is_directory("./data")) {
    printf("Path is a directory\n");
} else {
    printf("Path is not a directory or does not exist\n");
}

// Returns false for files
if (!cbridge_file.is_directory("config.txt")) {
    printf("config.txt is not a directory\n");
}

// Returns false for non-existent paths
if (!cbridge_file.is_directory("/nonexistent/path")) {
    printf("Path does not exist\n");
}

// NULL path returns false
if (!cbridge_file.is_directory(NULL)) {
    printf("Error: NULL path returned false\n");
}
```

### Getting the Filename from a Path
```c
// Extract filename from a Unix path
string_t* name = cbridge_file.get_filename("/path/to/file.txt");
printf("Filename: %s\n", cbridge_string.c_str(name));  // Output: file.txt
cbridge_string.free(name);

// Works with Windows paths
name = cbridge_file.get_filename("C:\\Users\\Documents\\report.csv");
printf("Filename: %s\n", cbridge_string.c_str(name));  // Output: report.csv
cbridge_string.free(name);

// Simple filename (no path) returns as-is
name = cbridge_file.get_filename("Makefile");
printf("Filename: %s\n", cbridge_string.c_str(name));  // Output: Makefile
cbridge_string.free(name);

// NULL or empty path returns empty string
name = cbridge_file.get_filename(NULL);
if (cbridge_string.empty(name)) {
    printf("Empty result for NULL path\n");
}
cbridge_string.free(name);
```

### Creating a Directory
```c
// Create a new directory
if (cbridge_file.create_directory("./output")) {
    printf("Directory created successfully\n");
}

// Returns false if directory already exists
if (!cbridge_file.create_directory("./output")) {
    printf("Directory already exists or creation failed\n");
}

// Returns false if path is a file
if (!cbridge_file.create_directory("config.txt")) {
    printf("Cannot create directory: path is a file\n");
}

// Does NOT create intermediate directories
if (!cbridge_file.create_directory("/nonexistent/parent/child")) {
    printf("Cannot create: parent directory does not exist\n");
}

// NULL path returns false
if (!cbridge_file.create_directory(NULL)) {
    printf("Error: NULL path returned false\n");
}
```

### Moving a File
```c
// Move a file from source to destination
if (cbridge_file.move("source.txt", "destination.txt")) {
    printf("File moved successfully\n");
}
// source.txt no longer exists; destination.txt contains the content

// Overwrites destination if it already exists
cbridge_file.move("new_data.csv", "data.csv");

// Returns false if source does not exist
if (!cbridge_file.move("missing.txt", "dest.txt")) {
    printf("Failed to move file: source not found\n");
}

// NULL source or destination returns false
cbridge_file.move(NULL, "dest.txt");   // Returns false
cbridge_file.move("source.txt", NULL); // Returns false
```

### Reading the Last N Lines
```c
// Read the last 5 lines from a large log file
string_t* tail = cbridge_file.read_last_lines("logfile.txt", 5);
if (!cbridge_string.empty(tail)) {
    printf("Last 5 lines:\n%s\n", cbridge_string.c_str(tail));
}
cbridge_string.free(tail);

// If file has fewer lines than count, returns all lines
tail = cbridge_file.read_last_lines("small.txt", 100);
printf("All lines:\n%s\n", cbridge_string.c_str(tail));
cbridge_string.free(tail);

// Edge case handling:
// - count = 0 returns empty string
// - NULL path returns empty string
// - File not found returns empty string
// - Trailing \n / \r\n are stripped from each line before joining
// - Lines are joined with '\n' in the output

tail = cbridge_file.read_last_lines(NULL, 5);
if (cbridge_string.empty(tail)) {
    printf("Empty result for NULL path\n");
}
cbridge_string.free(tail);

tail = cbridge_file.read_last_lines("logfile.txt", 0);
if (cbridge_string.empty(tail)) {
    printf("Empty result for count = 0\n");
}
cbridge_string.free(tail);
```

### Extracting Directory and Basename
```c
// Get parent directory
string_t* dir = cbridge_file.get_directory("/path/to/file.txt");
printf("Dir: %s\n", cbridge_string.c_str(dir));  // Output: /path/to
cbridge_string.free(dir);

// Get filename without last extension
string_t* base = cbridge_file.get_basename("/path/to/archive.tar.gz");
printf("Basename: %s\n", cbridge_string.c_str(base));  // Output: archive.tar
cbridge_string.free(base);

// Edge cases:
// - NULL or empty path returns empty string
// - Bare filename returns empty for directory, and the filename for basename
// - Hidden files (leading dot) have no extension, basename returns full filename
```

### Creating and Removing Directories Recursively
```c
// Create intermediate directories (idempotent)
if (cbridge_file.create_directories("/tmp/my/app/logs")) {
    printf("Directories created or already exist\n");
}

// Create_directories returns false if any component exists as a file

// Remove an empty directory (non-recursive)
if (cbridge_file.remove_directory("/tmp/my/app/logs")) {
    printf("Directory removed\n");
} else {
    printf("Failed to remove directory (may be non-empty)\n");
}

// Note: remove_directory does not recurse. Callers must empty the directory first.
```
