# 📚 Documentation Hub

This folder contains the complete technical documentation for the CTool and CBridge modules. Use the tables below to navigate the API references and usage guides.

## 🟢 CBridge (Pure C Modules)
*High-portability modules designed for C environments.*

| File                                  | Description                                                  | Documentation                    |
| :------------------------------------ | :----------------------------------------------------------- | :------------------------------- |
| [`src/cbridge/string.h`](../src/cbridge/string.h)       | Dynamic string object with C++-like functionality.           | [**string.md**](src/cbridge/string.md)       |
| [`src/cbridge/vector.h`](../src/cbridge/vector.h) | Generic dynamic array for storing any data type via `void*`. | [**vector.md**](src/cbridge/vector.md) |
| [`src/cbridge/file.h`](../src/cbridge/file.h)     | Filesystem utilities, path handling, and config parsing.     | [**file.md**](src/cbridge/file.md)     |
| [`src/cbridge/math.h`](../src/cbridge/math.h)     | high-performance numerical utilities.                        | [**math.md**](src/cbridge/math.md)     |
| [`src/cbridge/net.h`](../src/cbridge/net.h)       | C basic TCP and UDP networking utilities.                    | [**net.md**](src/cbridge/net.md)       |



---

## 🔵 CTool (C++11 Modules)
*Modern utility classes leveraging the C++ Standard Library.*

| File                                            | Description                                         | Documentation                          |
| :---------------------------------------------- | :-------------------------------------------------- | :------------------------------------- |
| [`src/ctool/json.hpp`](../src/ctool/json.hpp)           | Basic, JSON strings manipulation and extraction.    | [**json.md**](src/ctool/json.md)           |
| [`src/ctool/ini.hpp`](../src/ctool/ini.hpp)             | Manage standard INI configuration files             | [**ini.md**](src/ctool/ini.md)             |
| [`src/ctool/net.hpp`](../src/ctool/net.hpp)             | object-oriented interface for TCP networking.       | [**net.md**](src/ctool/net.md)             |
| [`src/ctool/math.hpp`](../src/ctool/math.hpp)           | Common mathematical and statistical utilities.      | [**math.md**](src/ctool/math.md)           |
| [`src/ctool/array2d.hpp`](../src/ctool/array2d.hpp)     | Dynamic 2D Array container                          | [**array2d.md**](src/ctool/array2d.md)     |
| [`src/ctool/array3d.hpp`](../src/ctool/array3d.hpp)     | Dynamic 3D Array container                          | [**array3d.md**](src/ctool/array3d.md)     |
| [`src/ctool/dataframe.hpp`](../src/ctool/dataframe.hpp) | A Python Pandas like DataFrame.                     | [**dataframe.md**](src/ctool/dataframe.md) |
| [`src/ctool/num.hpp`](../src/ctool/num.hpp)             | NumPy-like Array Operations                         | [**num.md**](src/ctool/num.md)             |
| [`src/ctool/csv.hpp`](../src/ctool/csv.hpp)             | Generic CSV parser and data generator.              | *Coming Soon*                          |
| [`src/ctool/string.hpp`](../src/ctool/string.hpp)             | Advanced string manipulation.                       | *Coming Soon*                          |
| [`src/ctool/file.hpp`](../src/ctool/file.hpp)           | C++ wrappers for file I/O and directory operations. | [**ct_file.md**](src/ctool/ct_file.md)     |
| [`src/ctool/log.hpp`](../src/ctool/log.hpp)             | Multi-level, thread-safe logging system.            | *Coming Soon*                          |
| [`src/ctool/time.hpp`](../src/ctool/time.hpp)           | High-precision timestamps and sleep utilities.      | *Coming Soon*                          |
| [`src/ctool/sys.hpp`](../src/ctool/sys.hpp)             | Cross-platform system and environment tools.        | *Coming Soon*                          |
| [`src/ctool/unit.hpp`](../src/ctool/unit.hpp)           | Flexible unit conversion and handling.              | [**unit.md**](src/ctool/unit.md)           |
| [`src/ctool/core.hpp`](../src/ctool/core.hpp)           | Framework core definitions and templates.           | *Coming Soon*                          |

---

## Usage Principles

1.  **Memory Management:** CBridge modules (`cbridge_`) require manual memory management via their respective `.free()` functions. CTool modules (`ctool_`) utilize standard RAII.
2.  **Namespace Pattern:** CBridge uses a global struct (e.g., `cbridge_string.method()`) to simulate namespaces in C.
3.  **Cross-Platform:** All modules are tested for compatibility between MSVC (Windows) and GCC/Clang (Linux).

**Maintained by:** Florent ALBANY  
**Last Updated:** 19/03/2026
