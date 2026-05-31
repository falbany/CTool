# ============================================================================
# Test module for CTool/CBridge using Google Test
# ============================================================================

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# Force shared CRT for MSVC compatibility
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test executables
add_executable(cb_string_test cbridge/string_test.cpp)
add_executable(ct_string_test ctool/string_test.cpp)
add_executable(ct_ini_test ctool/ini_test.cpp)
add_executable(ct_time_test ctool/time_test.cpp)
add_executable(ct_unit_test ctool/unit_test.cpp)
add_executable(ct_file_test ctool/file_test.cpp)
add_executable(ct_array2D_test ctool/array2D_test.cpp)
add_executable(ct_array3D_test ctool/array3D_test.cpp)
add_executable(ct_arrayND_test ctool/arraynd_test.cpp)
add_executable(ct_math_test ctool/math_test.cpp)
add_executable(ct_log_test ctool/log_test.cpp)
add_executable(cb_math_test cbridge/math_test.cpp)
add_executable(ct_dataframe_test ctool/dataframe_test.cpp)

# Place test executables next to shared DLLs so they can resolve them at runtime
foreach(targ cb_string_test ct_string_test ct_ini_test ct_time_test ct_unit_test ct_file_test ct_array2D_test ct_array3D_test ct_arrayND_test ct_math_test ct_log_test cb_math_test ct_dataframe_test)
    set_target_properties(${targ} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endforeach()

# Link the CBridge static library and Google Test's main entry point
target_link_libraries(cb_string_test PRIVATE ctool_cb gtest_main)

# Link the CTool static library and Google Test's main entry point
target_link_libraries(ct_string_test PRIVATE ctool_ct gtest_main)
target_link_libraries(ct_ini_test PRIVATE ctool_ct gtest_main)
target_link_libraries(ct_unit_test PRIVATE ctool_ct gtest_main)
target_link_libraries(ct_time_test PRIVATE ctool_ct gtest_main)
target_link_libraries(ct_file_test PRIVATE ctool_ct gtest_main)
target_link_libraries(ct_array2D_test PRIVATE ctool_ct gtest_main)
target_link_libraries(ct_array3D_test PRIVATE ctool_ct gtest_main)
target_link_libraries(ct_arrayND_test PRIVATE ctool_ct gtest_main)
target_link_libraries(ct_math_test PRIVATE ctool_ct gtest_main)
target_link_libraries(ct_log_test PRIVATE ctool_ct gtest_main)
target_link_libraries(cb_math_test PRIVATE ctool_cb gtest_main)
target_link_libraries(ct_dataframe_test PRIVATE ctool_ct gtest_main)
# Link Windows Sockets library on Windows
if(WIN32)
    target_link_libraries(ct_time_test PRIVATE ws2_32)
endif()

# Make project headers available (for #include "cb/string.h", #include "ct/ini.hpp", and #include "ct/time.hpp")
target_include_directories(cb_string_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
target_include_directories(ct_string_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
target_include_directories(ct_ini_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
target_include_directories(ct_time_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
target_include_directories(ct_unit_test PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(ct_file_test PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(ct_array2D_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
target_include_directories(ct_array3D_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
target_include_directories(ct_arrayND_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
target_include_directories(ct_log_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
target_include_directories(cb_math_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
target_include_directories(ct_dataframe_test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})

# Register GTest so CTest discovers individual tests
include(GoogleTest)
gtest_discover_tests(cb_string_test)
gtest_discover_tests(ct_string_test)
gtest_discover_tests(ct_ini_test)
gtest_discover_tests(ct_time_test)
gtest_discover_tests(ct_unit_test)
gtest_discover_tests(ct_file_test)
gtest_discover_tests(ct_array2D_test)
gtest_discover_tests(ct_array3D_test)
gtest_discover_tests(ct_arrayND_test)
gtest_discover_tests(ct_log_test)
gtest_discover_tests(cb_math_test)
