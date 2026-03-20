# CTool & CBridge Utility Library

**Project:** SERMA MTP Team / Librairies Logicielle / C-CPP  
**Author:** Florent ALBANY (FAL)  
**License:** Copyright (c) 2026  
**C++ Standard:** C++11 and above | **C Standard:** C99/C11

## Overview

**CTool** is a robust utility framework designed to streamline general software development in C and C++. It provides safe, platform-independent wrappers for common programming challenges, allowing developers to focus on logic rather than boilerplate code.

The project is structured into two distinct layers:
* **CBridge (`/cb`)**: A pure C implementation using an object-oriented "Namespace" pattern.
* **CTool (`/ct`)**: A modern C++11 library utilizing the Standard Template Library (STL) and modern RAII principles.

## Features

- **Cross-Platform:** Native support for Windows (`_WIN32`) and Linux/Unix environments.
- **String Manipulation:** Advanced parsing, trimming, splitting, and recursive find-and-replace.
- **Filesystem Management:** Safe I/O, directory traversal, and configuration (`key=value`) parsing.
- **Data Containers:** Generic dynamic vectors and string objects for C.
- **System Utilities:** High-precision timing, thread-safe logging, and unit conversion.

## Directory Structure

```text
.
├── cb/                # CBridge: Pure C Modules (Namespace Pattern)
├── ct/                # CTool: C++11 Utility Modules
├── doc/               # Dedicated Markdown Documentation
├── examples/          # Non-compiled usage examples
├── CBridge.h / .c     # Unified C entry point
├── CTool.hpp / .cpp   # Unified C++ entry point
└── README.md          # Main Project Hub
````

## Quick Start

### C++ Example (CTool)

```cpp
#include "CTool.hpp"

using namespace ct;
std::string clean = str::trim("  General Purpose Code  ");
if (file::exists("settings.conf")) {
    auto logFiles = file::getFiles("./logs", "data_", ".log");
}
```

### C Example (CBridge)

```c
#include "CBridge.h"

cb_string_t* s = cb_str.create("Generic_String");
cb_str.append(s, "_Suffix");
printf("%s\n", cb_str.c_str(s));
cb_str.free(s);
```

## Documentation

For a full API reference and detailed module guides, visit the **[Documentation Hub](.
/doc/README.md)**.

## Contact

  - **Author:** Florent ALBANY (f.albany@serma.com)
  - **Repository:** GitLab (SERMA MTP Team / CTool)
