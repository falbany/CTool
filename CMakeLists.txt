cmake_minimum_required(VERSION 3.24)
project(ctool VERSION 1.0.0 LANGUAGES C CXX)

cmake_policy(SET CMP0135 NEW)

# ============================================================================
# Project Standards
# ============================================================================
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(BuildTypes)
include(StaticAnalysis)
include(Sanitizers)
include(CodeCoverage)

# ============================================================================
# Options
# ============================================================================
option(BUILD_TESTS "Build tests" ON)

# ============================================================================
# CBridge Library (Shared DLL)
# Used internally for test linking.
# For static library builds, use the existing Makefile.
# ============================================================================
file(GLOB_RECURSE CB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/platform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/libctool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cbridge/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CBridge.c
)

add_library(ctool_cb SHARED ${CB_SOURCES})
target_include_directories(ctool_cb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(ctool_cb PRIVATE -Wall -Wextra)

# Link Windows Sockets library on Windows
if(WIN32)
    target_link_libraries(ctool_cb PRIVATE ws2_32)
endif()

# ============================================================================
# CTool Library (Static, C++11)
# Used internally for test linking.
# ============================================================================
file(GLOB_RECURSE CT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/platform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/libctool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ctool/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CTool.cpp
)

add_library(ctool_ct SHARED ${CT_SOURCES})
target_include_directories(ctool_ct PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(ctool_ct PRIVATE -Wall -Wextra)
if(WIN32)
    target_link_libraries(ctool_ct PRIVATE ws2_32)
endif()

# ============================================================================
# Tests
# ============================================================================
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
