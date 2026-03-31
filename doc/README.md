# 📚 Documentation Hub

This folder contains the complete technical documentation for the CTool and CBridge modules. Use the tables below to navigate the API references and usage guides.

## 🟢 CBridge (Pure C Modules)
*High-portability modules designed for C environments.*

| File                                  | Description                                                  | Documentation                    |
| :------------------------------------ | :----------------------------------------------------------- | :------------------------------- |
| [`cb/cb_str.h`](../cb/cb_str.h)       | Dynamic string object with C++-like functionality.           | [**cb_str.md**](cb_str.md)       |
| [`cb/cb_vector.h`](../cb/cb_vector.h) | Generic dynamic array for storing any data type via `void*`. | [**cb_vector.md**](cb_vector.md) |
| [`cb/cb_file.h`](../cb/cb_file.h)     | Filesystem utilities, path handling, and config parsing.     | [**cb_file.md**](cb_file.md)     |
| [`cb/cb_math.h`](../cb/cb_math.h)     | high-performance numerical utilities.                        | [**cb_math.md**](cb_math.md)     |
| [`cb/cb_net.h`](../cb/cb_net.h)       | C basic TCP and UDP networking utilities.                    | [**cb_net.md**](cb_net.md)       |



---

## 🔵 CTool (C++11 Modules)
*Modern utility classes leveraging the C++ Standard Library.*

| File                                            | Description                                         | Documentation                          |
| :---------------------------------------------- | :-------------------------------------------------- | :------------------------------------- |
| [`ct/ct_json.hpp`](../ct/ct_json.hpp)           | Basic, JSON strings manipulation and extraction.    | [**ct_json.md**](ct_json.md)           |
| [`ct/ct_ini.hpp`](../ct/ct_ini.hpp)             | Manage standard INI configuration files             | [**ct_ini.md**](ct_ini.md)             |
| [`ct/ct_net.hpp`](../ct/ct_net.hpp)             | object-oriented interface for TCP networking.       | [**ct_net.md**](ct_net.md)             |
| [`ct/ct_math.hpp`](../ct/ct_math.hpp)           | Common mathematical and statistical utilities.      | [**ct_math.md**](ct_math.md)           |
| [`ct/ct_array2d.hpp`](../ct/ct_array2d.hpp)     | Dynamic 2D Array container                          | [**ct_array2d.md**](ct_array2d.md)     |
| [`ct/ct_array3d.hpp`](../ct/ct_array3d.hpp)     | Dynamic 3D Array container                          | [**ct_array3d.md**](ct_array3d.md)     |
| [`ct/ct_dataframe.hpp`](../ct/ct_dataframe.hpp) | A Python Pandas like DataFrame.                     | [**ct_dataframe.md**](ct_dataframe.md) |
| [`ct/ct_csv.hpp`](../ct/ct_csv.hpp)             | Generic CSV parser and data generator.              | *Coming Soon*                          |
| [`ct/ct_str.hpp`](../ct/ct_str.hpp)             | Advanced string manipulation.                       | *Coming Soon*                          |
| [`ct/ct_file.hpp`](../ct/ct_file.hpp)           | C++ wrappers for file I/O and directory operations. | *Coming Soon*                          |
| [`ct/ct_log.hpp`](../ct/ct_log.hpp)             | Multi-level, thread-safe logging system.            | *Coming Soon*                          |
| [`ct/ct_time.hpp`](../ct/ct_time.hpp)           | High-precision timestamps and sleep utilities.      | *Coming Soon*                          |
| [`ct/ct_sys.hpp`](../ct/ct_sys.hpp)             | Cross-platform system and environment tools.        | *Coming Soon*                          |
| [`ct/ct_unit.hpp`](../ct/ct_unit.hpp)           | Flexible unit conversion and handling.              | *Coming Soon*                          |
| [`ct/ct_core.hpp`](../ct/ct_core.hpp)           | Framework core definitions and templates.           | *Coming Soon*                          |

---

## Usage Principles

1.  **Memory Management:** CBridge modules (`cb_`) require manual memory management via their respective `.free()` functions. CTool modules (`ct_`) utilize standard RAII.
2.  **Namespace Pattern:** CBridge uses a global struct (e.g., `cb_str.method()`) to simulate namespaces in C.
3.  **Cross-Platform:** All modules are tested for compatibility between MSVC (Windows) and GCC/Clang (Linux).

**Maintained by:** Florent ALBANY  
**Last Updated:** 19/03/2026
