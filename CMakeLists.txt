cmake_minimum_required(VERSION 3.14)
project(ctool VERSION 1.0.0 LANGUAGES C CXX)

# ============================================================================
# Project Standards
# ============================================================================
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/cbridge/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CBridge.c
)

add_library(ctool_cb SHARED ${CB_SOURCES})
target_include_directories(ctool_cb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
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
    ${CMAKE_CURRENT_SOURCE_DIR}/ctool/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CTool.cpp
)

add_library(ctool_ct SHARED ${CT_SOURCES})
target_include_directories(ctool_ct PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
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
