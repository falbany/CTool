# 📚 CTool & CBridge Documentation Hub

This hub provides a central map for the **CTool** framework. The project is divided into **CBridge** (Object-oriented C for portability) and **CTool** (Modern C++ for high-level bench automation).

## 🛠 Project Modules

| File | Description | Documentation |
| :--- | :--- | :--- |
| **CBridge (Pure C)** | | |
| [`cb/cb_str.h`](https://www.google.com/search?q=../cb/cb_str.h) | Dynamic string management with C++-like API. | [**Usage Guide**](./cb_str.md) |
| [`cb/cb_vector.h`](https://www.google.com/search?q=../cb/cb_vector.h) | Generic `void*` dynamic array (std::vector mimic). | [**Usage Guide**](./cb_vector.md) |
| [`cb/cb_file.h`](https://www.google.com/search?q=../cb/cb_file.h) | Low-level file I/O and configuration parsing. | [**Usage Guide**](./cb_file.md) |
| **CTool (C++)** | | |
| [`ct/ct_str.hpp`](https://www.google.com/search?q=../ct/ct_str.hpp) | Advanced string formatting and manipulation. | *Pending* |
| [`ct/ct_file.hpp`](https://www.google.com/search?q=../ct/ct_file.hpp) | High-level filesystem and path utilities. | *Pending* |
| [`ct/ct_csv.hpp`](https://www.google.com/search?q=../ct/ct_csv.hpp) | CSV data parsing and generation for test results. | *Pending* |
| [`ct/ct_log.hpp`](https://www.google.com/search?q=../ct/ct_log.hpp) | Thread-safe logging for automated test sequences. | *Pending* |
| [`ct/ct_math.hpp`](https://www.google.com/search?q=../ct/ct_math.hpp) | Statistical and mathematical tools for data analysis. | *Pending* |
| [`ct/ct_time.hpp`](https://www.google.com/search?q=../ct/ct_time.hpp) | High-precision timing and timestamping for bench measurements. | *Pending* |
| [`ct/ct_unit.hpp`](https://www.google.com/search?q=../ct/ct_unit.hpp) | Electronics-specific unit handling and conversions. | *Pending* |
| [`ct/ct_sys.hpp`](https://www.google.com/search?q=../ct/ct_sys.hpp) | System environment and OS-level interactions. | *Pending* |
| [`ct/ct_core.hpp`](https://www.google.com/search?q=../ct/ct_core.hpp) | Base templates and core framework definitions. | *Pending* |

-----

## 📂 Navigation Info

  * **`cb/`**: Contains the C implementation files. Use these if your target environment is a legacy ATE or an embedded system requiring strict C89/C99 compliance.
  * **`ct/`**: Contains C++ implementation and template files (`.hpp`, `.cpp`, `.tpp`). These take advantage of modern C++ features for more complex PC-based tools.
  * **`examples/`**: Contains non-compiled code snippets demonstrating how to integrate these modules into your specific electronics test projects.

-----

### Pro-Tip for Navigation

If you are using a modern IDE (VS Code, CLion), you can `Ctrl + Click` the file paths in the table above to jump directly to the header definitions for a quick look at the function signatures.
