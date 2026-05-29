# cmake/CodeCoverage.cmake

option(ENABLE_COVERAGE "Enable code coverage with GCC/Gcov" OFF)

if(ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled")
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()
