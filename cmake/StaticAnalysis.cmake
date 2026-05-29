# cmake/StaticAnalysis.cmake
# Provides clang-tidy and cppcheck integration for static code analysis.

option(ENABLE_STATIC_ANALYSIS "Enable static analysis tools (clang-tidy/cppcheck)" OFF)

if(ENABLE_STATIC_ANALYSIS)
    message(STATUS "Static analysis enabled")

    # ------------------------------------------------------------------------
    # clang-tidy (C++ only)
    # ------------------------------------------------------------------------
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

    if(CLANG_TIDY_EXE)
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")

        # Allow custom check overrides via cache variable
        set(STATIC_ANALYSIS_CLANG_TIDY_CHECKS
            "-*,bugprone-*,modernize-*,performance-*,readability-*"
            CACHE STRING "Comma-separated clang-tidy check pattern"
        )

        # Prefer a .clang-tidy config file if present at the project root
        if(EXISTS "${CMAKE_SOURCE_DIR}/.clang-tidy")
            set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
            message(STATUS "Using .clang-tidy config from project root")
        else()
            set(CMAKE_CXX_CLANG_TIDY
                "${CLANG_TIDY_EXE};-checks=${STATIC_ANALYSIS_CLANG_TIDY_CHECKS}"
            )
            message(STATUS "clang-tidy checks: ${STATIC_ANALYSIS_CLANG_TIDY_CHECKS}")
        endif()
    else()
        message(WARNING "clang-tidy not found. C++ static analysis will be skipped.")
    endif()

    # ------------------------------------------------------------------------
    # cppcheck (C and C++)
    # ------------------------------------------------------------------------
    find_program(CPPCHECK_EXE NAMES "cppcheck")

    if(CPPCHECK_EXE)
        message(STATUS "cppcheck found: ${CPPCHECK_EXE}")

        add_custom_target(
            run_cppcheck
            COMMAND ${CPPCHECK_EXE}
                --std=c99
                --std=c++11
                --enable=warning,performance,portability
                --suppress=missingIncludeSystem
                --quiet
                --project "${CMAKE_BINARY_DIR}"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            COMMENT "Running cppcheck static analysis..."
            VERBATIM
        )
    else()
        message(WARNING "cppcheck not found. C/C++ static analysis via cppcheck will be skipped.")
    endif()
endif()
