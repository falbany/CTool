# CMake Toolchain File for MSYS2 UCRT64
# Usage: cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/MSYS2-UCRT64.cmake

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# MSYS2 UCRT64 Installation Path
set(MSYS2_ROOT "C:/msys64/ucrt64")

# Specify Compilers
set(CMAKE_C_COMPILER "${MSYS2_ROOT}/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "${MSYS2_ROOT}/bin/g++.exe")
set(CMAKE_RC_COMPILER "${MSYS2_ROOT}/bin/windres.exe")

# Search for programs, libraries and include files only in the target environment
set(CMAKE_FIND_ROOT_PATH "${MSYS2_ROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set Compiler Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
