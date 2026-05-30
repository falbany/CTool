# ctool::file Module Documentation

The `ctool::file` module provides high-level filesystem utilities, configuration parsing, and directory traversal for C++11. It integrates with `ctool::str` for robust string handling.

## API Reference

| Function | Description |
| :--- | :--- |
| `exists(filename)` | Returns `true` if the file exists and is accessible. |
| `getSize(filename)` | Returns the file size in bytes, or -1 if the file does not exist. |
| `readAll(filename)` | Reads the entire file content into a `std::string` (normalizes `\r`). |
| `readLines(filename)` | Reads a file line by line into a `std::vector<std::string>`. |
| `writeAll(filename, content)` | Writes a string to a file, overwriting existing content. |
| `appendLine(filename, line)` | Appends a single line (with `\n`) to a file. |
| `getParameter(filename, key, separator)` | Parses a config file for a specific key (ignores comments). |
| `getWorkingDirectory()` | Returns the absolute path of the current working directory. |
| `getExtension(path)` | Extracts the file extension (including the dot). |
| `getFiles(directory, prefix, suffix)` | Lists filenames in a directory matching specific criteria. |
| `remove(filename)` | Deletes a file from the disk. |

## Usage Examples

### Basic File I/O
```cpp
#include "ctool/file.hpp"
#include <iostream>

// Write content to a file
ctool::file::writeAll("test.txt", "Hello CTool!\nLine 2");

// Read it back
std::string content = ctool::file::readAll("test.txt");
std::cout << "Content:\n" << content << std::endl;

// Read line by line
std::vector<std::string> lines = ctool::file::readLines("test.txt");
for (const auto& line : lines) {
    std::cout << "Line: " << line << std::endl;
}
```

### Configuration Parsing
Supports comments starting with `//`, `#`, `*`, `;`, or `!`.

```cpp
// config.ini
// # System Settings
// IP = 192.168.1.100
// PORT : 8080 ! Default port

std::string ip = ctool::file::getParameter("config.ini", "IP");
std::string port = ctool::file::getParameter("config.ini", "PORT", ":");
```

### Directory Traversal
```cpp
// List all .log files starting with "error" in current directory
std::vector<std::string> logs = ctool::file::getFiles(".", "error", ".log");

for (const auto& log : logs) {
    std::cout << "Found log file: " << log << " (Size: " << ctool::file::getSize(log) << " bytes)" << std::endl;
}
```

### Path Utilities
```cpp
std::string ext = ctool::file::getExtension("data.archive.tar.gz"); // Returns ".gz"
std::string cwd = ctool::file::getWorkingDirectory();
```
