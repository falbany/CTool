# ⚙️ Direct Source Usage Guide

This guide details how to integrate CTool and CBridge directly into your project by including their source files, rather than linking against pre-built shared or static libraries. This approach is suitable for projects that prefer to build all dependencies from source or for easier debugging within the consuming application.

## Integrating CBridge (Pure C)

The CBridge modules are designed for direct inclusion in C projects. Each module typically consists of a `.h` header file and a `.c` source file.

### 1. Include Headers

Directly include the unified CBridge header in your C source files:

```c
#include "path/to/CTool/src/CBridge.h"

// Or, for specific modules:
#include "path/to/CTool/src/cbridge/string.h"
#include "path/to/CTool/src/cbridge/file.h"
```

### 2. Add Source Files to Your Build System (CMake Example)

To compile CBridge modules as part of your project, add their `.c` files to your CMake target. Ensure that the include directories are correctly set up to find CBridge headers.

```cmake
# In your application's CMakeLists.txt

# Assuming CTool/CBridge are located in a subdirectory named 'CTool'
# Adjust paths based on your project structure

# Add CBridge source files
target_sources(
    YourApplication PRIVATE
    src/CTool/src/CBridge.c
    src/CTool/src/cbridge/string.c
    src/CTool/src/cbridge/vector.c
    src/CTool/src/cbridge/file.c
    # ... add other CBridge .c files as needed
)

# Add include directories for CBridge headers
target_include_directories(
    YourApplication PRIVATE
    src/CTool/src         # For CBridge.h
    src/CTool/src/cbridge # For individual module headers
    src/CTool/src/internal # For internal platform headers
)
```

### 3. Considerations for CBridge

-   **Memory Management:** Always remember to call the appropriate `.free()` functions for CBridge objects to prevent memory leaks, as it uses manual memory management.
-   **Namespace Pattern:** Access CBridge functionalities via the global `CBridge` struct or module-specific structs (e.g., `cbridge_string.method()`).

## Integrating CTool (C++11)

CTool modules are C++11 compliant and designed for direct inclusion in C++ projects. Each module typically uses a `.hpp` header file and may have a corresponding `.cpp` source file (or inline implementations/`.tpp` templates).

### 1. Include Headers

Include the unified CTool header or specific module headers:

```cpp
#include "path/to/CTool/src/CTool.hpp"

// Or, for specific modules:
#include "path/to/CTool/src/ctool/json.hpp"
#include "path/to/CTool/src/ctool/ini.hpp"
```

### 2. Add Source (and Template) Files to Your Build System (CMake Example)

Add CTool's `.cpp` and `.tpp` files to your CMake target. Ensure include paths are correct.

```cmake
# In your application's CMakeLists.txt

# Add CTool source files
target_sources(
    YourApplication PRIVATE
    src/CTool/src/CTool.cpp
    # src/CTool/src/ctool/my_module.cpp   # Add if your chosen module has a .cpp file
    # src/CTool/src/ctool/my_module.tpp   # Add if your chosen module has a .tpp file
    # ... add other CTool .cpp/.tpp files as needed
)

# Add include directories for CTool headers
target_include_directories(
    YourApplication PRIVATE
    src/CTool/src          # For CTool.hpp
    src/CTool/src/ctool    # For individual module headers and templates
    src/CTool/src/internal # For internal platform headers
)
```

### 3. Considerations for CTool

-   **RAII:** CTool modules extensively use RAII (Resource Acquisition Is Initialization) via C++ classes, simplifying memory management compared to CBridge.
-   **Namespaces:** CTool components are typically encapsulated within the `ct` namespace.
-   **C++11 Standard:** Ensure your project is compiled with C++11 or a newer standard (`-std=c++11` or higher).

## General Considerations for Direct Source Inclusion

-   **Compiler Flags:** Ensure consistent compiler flags (e.g., optimization levels, warning settings) between your project and CTool/CBridge sources to avoid unexpected behavior.
-   **Platform Macros:** The `src/internal/platform.h` header defines various macros (e.g., `_WIN32`, `DLLEXPORT`) for cross-platform compatibility. These will automatically apply when including the sources.
-   **Dependencies:** If CTool/CBridge have external dependencies, you will need to manage those in your project's build system as well.
