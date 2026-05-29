# CMake Modules & Toolchains

This directory contains CMake toolchain files and modules for the CTool/CBridge project.

## Contents

### Toolchain Files

Toolchain files define the **compiler and target environment**. They are selected at **configure time** via `-DCMAKE_TOOLCHAIN_FILE=...`. Only **one** toolchain can be used per build.

| File | Description |
|---|---|
| [`MSYS2-UCRT64.cmake`](./MSYS2-UCRT64.cmake) | MSYS2 UCRT64 GCC toolchain (default Windows environment) |
| [`Clang-LLVM.cmake`](./Clang-LLVM.cmake) | LLVM/Clang toolchain for Windows |
| [`MinGW.cmake`](./MinGW.cmake) | MinGW-w64 GCC toolchain |
| [`macOS-Clang.cmake`](./macOS-Clang.cmake) | Clang toolchain for macOS |

#### Usage

```bash
# Use MSYS2 UCRT64
cmake -B build-msys2 -DCMAKE_TOOLCHAIN_FILE=cmake/MSYS2-UCRT64.cmake

# Use Clang/LLVM (Windows)
cmake -B build-clang -DCMAKE_TOOLCHAIN_FILE=cmake/Clang-LLVM.cmake

# Use macOS Clang
cmake -B build-mac -DCMAKE_TOOLCHAIN_FILE=cmake/macOS-Clang.cmake

# Use MinGW-w64
cmake -B build-mingw -DCMAKE_TOOLCHAIN_FILE=cmake/MinGW.cmake
```

> **Note:** Adjust the `*_ROOT` paths inside each toolchain file to match your local installation.

---

### CMake Modules

Modules add **extra functionality** and are included automatically in `CMakeLists.txt`. Multiple modules can be enabled simultaneously.

| File | Description | Option |
|---|---|---|
| [`BuildTypes.cmake`](./BuildTypes.cmake) | Sets default build type (`Release`) and per-type compiler flags | N/A (always included) |
| [`StaticAnalysis.cmake`](./StaticAnalysis.cmake) | Enables `clang-tidy` integration with configurable check subsets | `ENABLE_STATIC_ANALYSIS` |
| [`Sanitizers.cmake`](./Sanitizers.cmake) | Enables AddressSanitizer and UndefinedBehaviorSanitizer | `ENABLE_SANITIZERS` |
| [`CodeCoverage.cmake`](./CodeCoverage.cmake) | Enables GCC/Gcov code coverage instrumentation | `ENABLE_COVERAGE` |

#### Usage

```bash
# Enable static analysis
cmake -B build -DENABLE_STATIC_ANALYSIS=ON

# Enable sanitizers
cmake -B build -DENABLE_SANITIZERS=ON

# Enable code coverage
cmake -B build -DENABLE_COVERAGE=ON

# Combine multiple options
cmake -B build-debug \
  -DCMAKE_TOOLCHAIN_FILE=cmake/MSYS2-UCRT64.cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_STATIC_ANALYSIS=ON \
  -DENABLE_SANITIZERS=ON
```

---

## Common Combinations

| Scenario | Command |
|---|---|
| Debug with MSYS2 + sanitizers + static analysis | `cmake -B build-debug -DCMAKE_TOOLCHAIN_FILE=cmake/MSYS2-UCRT64.cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON -DENABLE_STATIC_ANALYSIS=ON` |
| Release with Clang + coverage | `cmake -B build-coverage -DCMAKE_TOOLCHAIN_FILE=cmake/Clang-LLVM.cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_COVERAGE=ON` |
| Default Release build | `cmake -B build` |

---

## Adding a New Toolchain

1. Create `cmake/<Name>.cmake`
2. Set `CMAKE_SYSTEM_NAME`, `CMAKE_SYSTEM_PROCESSOR`, and compiler paths
3. Configure `CMAKE_FIND_ROOT_PATH` variables
4. Document in the table above

## Adding a New Module

1. Create `cmake/<ModuleName>.cmake`
2. Define an `option()` toggle
3. Guard logic with `if(ENABLE_MODULE_NAME)`
4. Add `include(ModuleName)` to `CMakeLists.txt`
5. Document in the table above
