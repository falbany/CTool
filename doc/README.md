# 📚 Documentation Hub

This folder contains the complete technical documentation for the CTool and CBridge modules. Use the tables below to navigate the API references and usage guides.

## 🟢 CBridge (Pure C Modules)
*High-portability modules designed for C environments.*

| File                                  | Description                                                  | Documentation                    |
| :------------------------------------ | :----------------------------------------------------------- | :------------------------------- |
| [`cbridge/string.h`](../cbridge/string.h)       | Dynamic string object with C++-like functionality.           | [**string.md**](cbridge/string.md)       |
| [`cbridge/vector.h`](../cbridge/vector.h) | Generic dynamic array for storing any data type via `void*`. | [**vector.md**](cbridge/vector.md) |
| [`cbridge/file.h`](../cbridge/file.h)     | Filesystem utilities, path handling, and config parsing.     | [**file.md**](cbridge/file.md)     |
| [`cbridge/math.h`](../cbridge/math.h)     | high-performance numerical utilities.                        | [**math.md**](cbridge/math.md)     |
| [`cbridge/net.h`](../cbridge/net.h)       | C basic TCP and UDP networking utilities.                    | [**net.md**](cbridge/net.md)       |



---

## 🔵 CTool (C++11 Modules)
*Modern utility classes leveraging the C++ Standard Library.*

| File                                            | Description                                         | Documentation                          |
| :---------------------------------------------- | :-------------------------------------------------- | :------------------------------------- |
| [`ctool/json.hpp`](../ctool/json.hpp)           | Basic, JSON strings manipulation and extraction.    | [**json.md**](ctool/json.md)           |
| [`ctool/ini.hpp`](../ctool/ini.hpp)             | Manage standard INI configuration files             | [**ini.md**](ctool/ini.md)             |
| [`ctool/net.hpp`](../ctool/net.hpp)             | object-oriented interface for TCP networking.       | [**net.md**](ctool/net.md)             |
| [`ctool/math.hpp`](../ctool/math.hpp)           | Common mathematical and statistical utilities.      | [**math.md**](ctool/math.md)           |
| [`ctool/array2d.hpp`](../ctool/array2d.hpp)     | Dynamic 2D Array container                          | [**array2d.md**](ctool/array2d.md)     |
| [`ctool/array3d.hpp`](../ctool/array3d.hpp)     | Dynamic 3D Array container                          | [**array3d.md**](ctool/array3d.md)     |
| [`ctool/dataframe.hpp`](../ctool/dataframe.hpp) | A Python Pandas like DataFrame.                     | [**dataframe.md**](ctool/dataframe.md) |
| [`ctool/num.hpp`](../ctool/num.hpp)             | NumPy-like Array Operations                         | [**num.md**](ctool/num.md)             |
| [`ctool/csv.hpp`](../ctool/csv.hpp)             | Generic CSV parser and data generator.              | *Coming Soon*                          |
| [`ctool/string.hpp`](../ctool/string.hpp)             | Advanced string manipulation.                       | *Coming Soon*                          |
| [`ctool/file.hpp`](../ctool/file.hpp)           | C++ wrappers for file I/O and directory operations. | [**ct_file.md**](ctool/ct_file.md)     |
| [`ctool/log.hpp`](../ctool/log.hpp)             | Multi-level, thread-safe logging system.            | *Coming Soon*                          |
| [`ctool/time.hpp`](../ctool/time.hpp)           | High-precision timestamps and sleep utilities.      | *Coming Soon*                          |
| [`ctool/sys.hpp`](../ctool/sys.hpp)             | Cross-platform system and environment tools.        | *Coming Soon*                          |
| [`ctool/unit.hpp`](../ctool/unit.hpp)           | Flexible unit conversion and handling.              | [**unit.md**](ctool/unit.md)           |
| [`ctool/core.hpp`](../ctool/core.hpp)           | Framework core definitions and templates.           | *Coming Soon*                          |

---

## Usage Principles

1.  **Memory Management:** CBridge modules (`cbridge_`) require manual memory management via their respective `.free()` functions. CTool modules (`ctool_`) utilize standard RAII.
2.  **Namespace Pattern:** CBridge uses a global struct (e.g., `cbridge_string.method()`) to simulate namespaces in C.
3.  **Cross-Platform:** All modules are tested for compatibility between MSVC (Windows) and GCC/Clang (Linux).

**Maintained by:** Florent ALBANY  
**Last Updated:** 19/03/2026
