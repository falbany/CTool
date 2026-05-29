# 🤖 Test Agent Protocol (tests/AGENTS.md)

This document defines the mandatory conventions for adding or modifying tests in the CTool/CBridge project.

## 1. Framework

- **Google Test (GTest)** is the sole test framework. Do not introduce custom test macros.
- Test files are **C++ (`.cpp`)** — even when testing pure C modules (CBridge). GTest requires C++.
- The `gtest_main` library provides the `main()` entry point. Do **not** write a custom `main()`.
- C headers are included via `extern "C" { #include "cb/cb_str.h" }` when they lack `__cplusplus` guards.

## 2. File & Naming Conventions

- Test file name: `<module>_test.cpp` (e.g., `cb_str_test.cpp`, `cb_math_test.cpp`).
- Each test executable maps to one source file.
- Test suite naming: `TEST(<Module><Section>, TestName)` where `<Module><Section>` is PascalCase (e.g., `CbStrLifecycle`, `CtStrTrim`).
- Test case names are PascalCase (e.g., `CreateNullString`, `TrimBothSides`).
- File must include a Doxygen header (`@file`, `@author`, `@brief`, `@version`, `@date`).

## 3. Conventions

### Assertions
- Prefer **`EXPECT_*`** over `ASSERT_*` (non-fatal gives more feedback per run).
- Use `ASSERT_*` only when continuing after failure would crash or corrupt state (e.g., dereferencing a null pointer).
- Use `EXPECT_THAT` with matchers for complex assertions where applicable.
- Use `<< "message"` chained on assertions to clarify test intent when the expression is not self-documenting.

### String Comparisons
- `EXPECT_STREQ(expected, actual)` for C-string equality.
- `EXPECT_EQ(expected, actual)` for numeric equality.

### Memory & Resources
- All heap-allocated objects (`cb_string_t*`, `cb_str_parts_t`, etc.) **must** be freed in the test body.
- Use raw `cb_str.free(s)` / `cb_str_parts_free(&parts)` — no fixtures or RAII wrappers.

### NULL / nullptr
- Use `nullptr` (C++ keyword) in GTest comparisons: `EXPECT_NE(s, nullptr)`.
- Use `NULL` only when calling C APIs that expect `NULL` (e.g., `cb_str.create(NULL)`).

## 4. Build Integration

### CMake Targets
- Each test executable is registered in `tests/CMakeLists.txt`:
  ```cmake
  add_executable(<name> <name>.cpp)
  target_link_libraries(<name> PRIVATE ctool_cb gtest_main)
  target_include_directories(<name> PRIVATE ${CMAKE_SOURCE_DIR})
  ```
- Link `ctool_cb` for CBridge tests, `ctool_ct` for CTool tests, or both.
- GTest is fetched automatically via `FetchContent` — no manual installation required.
- Each executable must be registered with `gtest_discover_tests(<name>)` for CTest integration.

### Adding a New Test File
1. Create `<module>_test.cpp` in `tests/`.
2. Add the CMake target to `tests/CMakeLists.txt`.
3. Build: `cmake -B build && cmake --build build`.
4. Run: `ctest --test-dir build` or `./build/tests/<module>_test`.

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
 * @file cb_str_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Comprehensive test suite for CBridge string module (cb_str) using Google Test.
 * @version 2.0
 * @date 2026-05-29
 */
```

## 6. Directory Standards
- **Test source files** — `tests/*.cpp` (one per module).
- **CMake integration** — `tests/CMakeLists.txt`.
- **Documentation** — `tests/README.md` (this directory's guide), `tests/AGENTS.md` (this file).
- Do **not** place test data or fixtures outside `tests/`.

## 7. What Not To Do
- Do **not** use `TEST_F` or test fixtures unless shared setup/teardown between tests justifies it.
- Do **not** use custom assert macros — GTest provides all needed assertions.
- Do **not** write a custom `main()` — `gtest_main` handles it.
- Do **not** suppress compiler warnings in test code.
- Do **not** add test targets to the root `Makefile` — tests are built via CMake only.
