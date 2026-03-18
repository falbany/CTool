# CTool

**Project:** SERMA MTP Team / Librairies Logicielle / C-CPP  
**Author:** Florent ALBANY (FAL)  
**License:** Copyright (c) 2026  
**C++ Standard:** C++11 and above

## Overview

**CTool** is a robust, modernized C++11 utility library designed to facilitate hardware testing, cross-platform application development, and system automation. It encapsulates a comprehensive set of core utilities handling time management, string manipulation, file system operations, and configurable logging.

This library aims to reduce boilerplate code in C/C++ projects by providing safe, platform-independent wrappers for common tasks such as timestamp generation, string parsing, and configuration file handling.

## Features

-   **Cross-Platform Support:** Fully compatible with Windows (`_WIN32`) and Linux/Unix environments.
-   **Modern C++:** Built entirely on C++11 standards, utilizing `std::chrono`, `std::thread`, `std::vector`, and smart string handling.
-   **Logging System:** A multi-level logging system (Quiet, Error, Warning, Info, Debug) with configurable verbosity and output streams (`stdout`/`stderr`).
-   **Time Utilities:** Accurate timestamps, sleep functions, and uptime monitoring.
-   **String Manipulation:** Advanced string parsing, field updating, trimming, and safe conversion.
-   **File Operations:** Safe file reading, size detection, directory traversal, and configuration parameter extraction.

## Installation

CTool is designed as a header-only library with a single implementation file.

1.  Ensure you have a C++11 compliant compiler (e.g., GCC 4.8+, Clang, MSVC 2015+).
2.  Include the header and object file in your build system:

### Makefile Example
```makefile
CXXFLAGS += -std=c++11
CTOOL_SOURCES = CTool.cpp
CTOOL_HEADERS = CTool.hpp

all:
	g++ $(CXXFLAGS) -I. $(CTOOL_SOURCES) main.cpp -o my_app
```

## Usage Examples

### 1. Logging
The logging system respects the global verbosity level.

```cpp
#include "CTool.hpp"

using namespace ct;

int main() {
    // Log an error message
    log::toCerr(log::Level::Error, "Critical system failure.");
    
    // Log an info message (visible if currentLevel <= Debug)
    log::toCout(log::Level::Info, "Application started.");
    
    return 0;
}
```

### 2. Time Management
```cpp
#include "CTool.hpp"
#include <iostream>

// Get current formatted timestamp
std::cout << "Current Time: " << time::getCurrentTimestamp() << std::endl;

// Sleep for 500 milliseconds
time::sleepMs(500);

// Get application uptime in milliseconds
long long uptime = time::getUptimeMs();
```

### 3. String Utilities
```cpp
#include "CTool.hpp"

using namespace ct::str;

int main() {
    // Update a field in a delimited string (e.g., "SN001_LOT42_T" -> "SN001_LOT42_T25.00")
    std::string config = "SN001_LOT42";
    config = updateField(config, "T", "25.00");

    // Split a string
    std::string data = "a,b,c,d";
    std::vector<std::string> tokens = split(data, ',');

    // Trim whitespace
    std::string dirty = "  Hello World  ";
    std::string clean = trim(dirty);

    return 0;
}
```

### 4. File Operations
```cpp
#include "CTool.hpp"

using namespace ct::file;

int main() {
    std::string configPath = "config.txt";

    if (exists(configPath)) {
        // Read the entire file content
        std::string content = readAll(configPath);
        
        // Extract a specific parameter (e.g., "key=value" format)
        std::string value = getParameter(configPath, "timeout", "=");
        
        // List files in a directory with specific extensions
        auto txtFiles = getFiles("./data", "", ".txt");
    }

    return 0;
}
```

## API Reference

### Namespace `ct::log`
| Function                                           | Description                                         |
| :------------------------------------------------- | :-------------------------------------------------- |
| `message(Level, std::string, std::ostream, Level)` | Logs a message to a specific stream based on level. |
| `toCout(Level, std::string, Level)`                | Convenience wrapper for logging to `stdout`.        |
| `toCerr(Level, std::string, Level)`                | Convenience wrapper for logging to `stderr`.        |

**Log Levels:**
-   `Quiet` (0)
-   `Error` (1)
-   `Warning` (2)
-   `Info` (3)
-   `Debug` (4)

### Namespace `ct::time`
| Function                | Description                              |
| :---------------------- | :--------------------------------------- |
| `getCurrentTimestamp()` | Returns "YYYY-MM-DD HH:MM:SS".           |
| `sleepMs(long)`         | Sleeps for milliseconds.                 |
| `sleepS(long)`          | Sleeps for seconds.                      |
| `getUptimeMs()`         | Returns uptime since steady clock start. |

### Namespace `ct::str`
| Function                            | Description                                     |
| :---------------------------------- | :---------------------------------------------- |
| `assign(std::string&, const char*)` | Safe assignment from C-string.                  |
| `updateField(...)`                  | Updates a specific field in a delimited string. |
| `findAndReplace(...)`               | Replaces all occurrences of a substring.        |
| `catchInRange(...)`                 | Extracts text between two delimiters.           |
| `split(...)`                        | Splits string by delimiter.                     |
| `trim(...)`                         | Removes leading/trailing whitespace.            |
| `toString(...)`                     | Converts any type to string.                    |

### Namespace `ct::file`
| Function                | Description                                                  |
| :---------------------- | :----------------------------------------------------------- |
| `exists(...)`           | Checks file accessibility.                                   |
| `getSize(...)`          | Returns file size in bytes.                                  |
| `readAll(...)`          | Reads entire file content (normalizes line endings).         |
| `getWorkingDirectory()` | Returns current working directory.                           |
| `getParameter(...)`     | Parses `key=value` pairs from config files.                  |
| `getFiles(...)`         | Lists files in a directory (supports prefix/suffix filters). |

## Dependencies
-   C++11 Standard Library (`<iostream>`, `<string>`, `<vector>`, `<sstream>`, `<fstream>`, `<chrono>`, `<thread>`, `<algorithm>`, `<cctype>`)
-   Platform-specific headers:
    -   **Windows:** `<windows.h>`, `<direct.h>`
    -   **Linux/Unix:** `<unistd.h>`, `<dirent.h>`, `<sys/types.h>`

## License
Copyright (c) 2026 Florent ALBANY / SERMA MTP Team.

## Contact
-   **Author:** Florent ALBANY (f.albany@serma.com)
-   **Repository:** GitLab (SERMA MTP Team / Librairies Logicielle / C-CPP / CTool)