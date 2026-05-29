# CMake Toolchain File for LLVM/Clang on Windows
# Usage: cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/Clang-LLVM.cmake

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# LLVM Installation Path (adjust to your path)
set(LLVM_ROOT "C:/Program Files/LLVM")

# Specify Compilers
set(CMAKE_C_COMPILER "${LLVM_ROOT}/bin/clang.exe")
set(CMAKE_CXX_COMPILER "${LLVM_ROOT}/bin/clang++.exe")

# Search paths
set(CMAKE_FIND_ROOT_PATH "${LLVM_ROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Compiler Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
