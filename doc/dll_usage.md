# 📦 DLL Usage Guide

This document outlines the process for building and utilizing CTool and CBridge as Dynamic Link Libraries (DLLs) on Windows, and shared libraries on other platforms.

## Understanding Dynamic Linking

Dynamic linking allows a single copy of a library to be shared among multiple applications, reducing memory footprint and simplifying updates.

-   **DLL (Dynamic Link Library):** The term used on Windows for shared libraries.
-   **Shared Library (.so, .dylib):** The equivalent term used on Linux (`.so`) and macOS (`.dylib`).

## Exporting Symbols (Library Development)

For CTool and CBridge modules to be accessible from other applications, their symbols (functions, variables, classes) must be explicitly exported when building the library. This project uses a set of macros to handle platform-specific export/import declarations.

The core macro is `DLLEXPORT`, which resolves to `__declspec(dllexport)` on Windows when building the library, and no special declaration on other platforms.

### Example: Defining an Exported Function

```c
// In a header file (e.g., src/cbridge/my_module.h)

#pragma once

#include "src/internal/platform.h" // For DLLEXPORT

#ifdef __cplusplus
extern "C" {
#endif

// Declare a CBridge function to be exported
DLLEXPORT int cbridge_my_module_add(int a, int b);

#ifdef __cplusplus
}
#endif
```

```cpp
// In a header file (e.g., src/ctool/my_class.hpp)

#pragma once

#include "src/internal/platform.h" // For DLLEXPORT

namespace ct {

// Declare a CTool class to be exported
class DLLEXPORT MyClass {
public:
    MyClass();
    int add(int a, int b);
};

} // namespace ct
```

## Importing Symbols (Application Development)

When consuming CTool or CBridge as a DLL/shared library in your own application, you need to declare that you are *importing* the symbols. The `DLLEXPORT` macro automatically handles this by resolving to `__declspec(dllimport)` on Windows when *consuming* the library, provided it's defined correctly during compilation.

You generally don't need to do anything special in your application code other than including the appropriate headers and linking against the import library (Windows) or the shared library (Linux/macOS). The build system (CMake) handles the correct `DLLEXPORT` definition internally based on whether the current compilation unit is part of the library itself or an external application.

### CMake Integration

When building an application that uses CTool/CBridge, ensure your `CMakeLists.txt` correctly links against the installed libraries.

```cmake
# In your application's CMakeLists.txt

# Find CTool/CBridge (assuming they are installed to a known location)
find_package(CTool REQUIRED)
find_package(CBridge REQUIRED)

# Link your application against the libraries
target_link_libraries(YourApplication PRIVATE CTool::CTool CBridge::CBridge)
```

CMake will automatically propagate the necessary compiler definitions (e.g., `CTOOL_EXPORTS` or `CBRIDGE_EXPORTS` internal definitions for library compilation) to ensure `DLLEXPORT` expands correctly for both building and consuming the libraries.

## Build Process

To build CTool/CBridge as a shared library:

1.  **Configure CMake:**
    ```bash
    cmake -B build -DBUILD_SHARED_LIBS=ON
    ```
    The `BUILD_SHARED_LIBS=ON` flag instructs CMake to build shared libraries instead of static ones.

2.  **Build the Project:**
    ```bash
    cmake --build build
    ```

After building, the DLLs (e.g., `.dll` on Windows, `.so` on Linux, `.dylib` on macOS) will be located in the `build/bin/` directory.

## Deployment Considerations

-   **Windows:** When deploying an application that uses CTool/CBridge DLLs, ensure the `.dll` files are placed in a location where the operating system can find them (e.g., same directory as the executable, system PATH, or `SxS` assemblies).
-   **Linux/macOS:** Shared libraries (`.so`, `.dylib`) are typically found via `LD_LIBRARY_PATH` (Linux) or `DYLD_LIBRARY_PATH` (macOS), or by installing them to standard system library paths.
