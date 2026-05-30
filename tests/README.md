# CTool / CBridge — Test Suite

**Author:** Florent ALBANY (FAL)  
**Framework:** Google Test (GTest)  
**Build System:** CMake (FetchContent for GTest)

## Overview

This directory contains the automated test suite for the CTool and CBridge libraries. All tests use Google Test as the testing framework and are built via CMake.

### Current Test Files

| File | Module Tested | Language | Coverage |
|---|---|---|---|
| `/str_test.cpp` | CBridge string module (`/str`) | C++ (tests C API) | Lifecycle, append, insert, erase, trim, replace_all, compare, starts_with, ends_with, find, substr, catch_in_range, split, edge cases |

## Prerequisites

- **CMake** >= 3.14
- **C/C++ compiler** (GCC, Clang, or MSVC)
- Google Test is downloaded automatically via CMake FetchContent — no manual installation required.

## Building & Running

### Build

```bash
# From the project root:
cmake -B build
cmake --build build
```

### Run All Tests

```bash
ctest --test-dir build
```

### Run a Specific Test Executable

```bash
./build/tests//str_test
```

### Run Specific Test Cases (GTest filter)

```bash
./build/tests//str_test --gtest_filter="CbStrTrim.*"
./build/tests//str_test --gtest_filter="CbStrLifecycle.CreateNullString"
```

## Adding a New Test

1. Create `<module>_test.cpp` in this directory.
2. Include the module header (with `extern "C"` if testing C code):
   ```cpp
   extern "C" {
   #include "cb/string.h"
   }
   ```
3. Write test cases using GTest macros:
   ```cpp
   TEST(CbStrLifecycle, MyNewTest) {
       /string_t* s = /str.create("test");
       EXPECT_EQ(/str.length(s), 4);
       /str.free(s);
   }
   ```
4. Register the executable in `tests/CMakeLists.txt`:
   ```cmake
   add_executable(my_module_test my_module_test.cpp)
   target_link_libraries(my_module_test PRIVATE ctool_cb gtest_main)
   target_include_directories(my_module_test PRIVATE ${CMAKE_SOURCE_DIR})
   gtest_discover_tests(my_module_test)
   ```
5. Rebuild and verify:
   ```bash
   cmake -B build && cmake --build build
   ctest --test-dir build
   ```

## Conventions

- **Test suite names**: PascalCase, `<Module><Section>` (e.g., `CbStrTrim`, `CtStrCompare`).
- **Test case names**: PascalCase, descriptive (e.g., `TrimLeadingSpaces`, `FindNotFound`).
- **Assertions**: Prefer `EXPECT_*` (non-fatal) over `ASSERT_*` (fatal).
- **Memory**: Always free C resources (`/str.free()`, `/str_parts_free()`) in the test body.
- **Null pointers**: Use `nullptr` for GTest comparisons, `NULL` for C API calls.

## Troubleshooting

| Issue | Solution |
|---|---|
| `gtest/gtest.h` not found | FetchContent handles this — ensure network access during CMake configure |
| Linker errors: undefined symbols | Check that `target_link_libraries` includes `ctool_cb` (CBridge) or `ctool_ct` (CTool) |
| Test not discovered by CTest | Ensure `gtest_discover_tests(<target>)` is called in `CMakeLists.txt` |

## Reference

- [Google Test Primer](https://google.github.io/googletest/primer.html)
- [CMake FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html)
