# 🤖 Test Agent Protocol (tests/AGENTS.md)

This document defines the mandatory conventions for adding or modifying tests in the CTool/CBridge project.

## 1. Framework

- **Google Test (GTest)** is the sole test framework. Do not introduce custom test macros.
- Test files are **C++ (`.cpp`)** — even when testing pure C modules (CBridge). GTest requires C++.
- The `gtest_main` library provides the `main()` entry point. Do **not** write a custom `main()`.
- C headers are included via `extern "C" { #include "cb/string.h" }` when they lack `__cplusplus` guards.

## 2. File & Naming Conventions

- Test file name: `<module>_test.cpp` (e.g., `string_test.cpp`, `math_test.cpp`).
- Files for CTool should be in `tests/ctool/` and for CBridge in `tests/cbridge/`.
- Each test executable maps to one source file.
- **Suite-Based Categorization:** Use the `test_suite_name` parameter in `TEST(test_suite_name, test_name)` to categorize tests. Instead of a single generic suite like `<Module>Test`, split tests into logical categories (e.g., `LogVerbosity`, `LogFormatting`, `LogConcurrency`).
- Test suite naming: `TEST(<Module><Category>, TestName)` where `<Module><Category>` is PascalCase.
- Test case names are PascalCase (e.g., `Addition`, `CreateNullString`).
- File must include a Doxygen header (`@file`, `@author`, `@brief`, `@version`, `@date`).

## 3. Formatting Standards
All test code **must** be formatted using `clang-format`.
-   Run `./scripts/format.sh` from the root directory to apply formatting to all tests.
-   The CI pipeline will enforce this with `./scripts/format.sh --check`.

## 4. Conventions

### Assertions
- Prefer **`EXPECT_*`** over `ASSERT_*` (non-fatal gives more feedback per run).
- Use `ASSERT_*` only when continuing after failure would crash or corrupt state (e.g., dereferencing a null pointer).
- Use `EXPECT_THAT` with matchers for complex assertions where applicable.
- Use `<< "message"` chained on assertions to clarify test intent when the expression is not self-documenting.

### String Comparisons
- `EXPECT_STREQ(expected, actual)` for C-string equality.
- `EXPECT_EQ(expected, actual)` for numeric equality.

### Memory & Resources
- All heap-allocated objects (`string_t*`, `str_parts_t`, etc.) **must** be freed in the test body.
- Use raw functions — no fixtures or RAII wrappers unless absolutely matching custom test suites.

### NULL / nullptr
- Use `nullptr` (C++ keyword) in GTest comparisons: `EXPECT_NE(s, nullptr)`.
- Use `NULL` only when calling C APIs that expect `NULL`.

## 4. Build Integration

### CMake Targets
- Each test executable is registered in `tests/CMakeLists.txt`:
  ```cmake
  add_executable(<name> ctool/<name>.cpp) # or cbridge/<name>.cpp
  target_link_libraries(<name> PRIVATE ctool_ct gtest_main)
  target_include_directories(<name> PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
  ```
- Link `ctool_cb` for CBridge tests, `ctool_ct` for CTool tests, or both.
- Ensure the executable targets are registered with `set_target_properties` to place them in `${CMAKE_BINARY_DIR}`:
  ```cmake
  set_target_properties(<name> PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  ```
- GTest is fetched automatically via `FetchContent` — no manual installation required.
- Each executable must be registered with `gtest_discover_tests(<name>)` for CTest integration.

### Adding a New Test File
1. Create `<module>_test.cpp` under `tests/ctool/` or `tests/cbridge/`.
2. Add the CMake target to `tests/CMakeLists.txt`.
3. Build: `cmake -B build && cmake --build build`.
4. Run: `ctest --test-dir build` or `./build/<module>_test`.


### CTest Discovery
Every test target must call `gtest_discover_tests()` so CTest runs individual test cases. Without it, CTest sees only one result per executable.

## 5. Doxygen Requirements (test files)

Each test file header must include:
- `@file` — filename
- `@author` — original author
- `@brief` — what module is being tested
- `@version` — version number
- `@date` — creation/modification date

Example:
```cpp
/**
 * @file /str_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Comprehensive test suite for CBridge string module (/str) using Google Test.
 * @version 2.0
 * @date 2026-05-29
 */
```

## 6. Directory Standards
- **Test source files** — `tests/ctool/*.cpp` or `tests/cbridge/*.cpp` (one per module).
- **CMake integration** — `tests/CMakeLists.txt`.
- **Documentation** — `tests/README.md` (this directory's guide), `tests/AGENTS.md` (this file).
- Do **not** place test data or fixtures outside `tests/`.


## 7. What Not To Do
- Do **not** use `TEST_F` or test fixtures unless shared setup/teardown between tests justifies it.
- Do **not** use custom assert macros — GTest provides all needed assertions.
- Do **not** write a custom `main()` — `gtest_main` handles it.
- Do **not** suppress compiler warnings in test code.
- Do **not** add test targets to the root `Makefile` — tests are built via CMake only.
