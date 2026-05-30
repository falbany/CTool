/**
 * @file CBridge.h
 * @author Florent ALBANY - FAL
 * @brief Main entry point for the CBridge pure c library.
 * @version 1.0
 * @date 2026-03-19
 */

#pragma once

#ifndef CBRIDGE_MAIN_HPP
    #define CBRIDGE_MAIN_HPP

    #include "internal/libctool.h"

    #include "cbridge/string.h"
    #include "cbridge/vector.h"
    #include "cbridge/file.h"
    #include "cbridge/math.h"
    #include "cbridge/net.h"

/**
 * @struct CBridge_Namespace
 * @brief Global singleton struct holding pointers to all module namespaces.
 * 
 * This struct serves as the unified entry point for accessing the functionalities
 * provided by the CBridge library. It is designed to be non-owning; it simply
 * provides constant access to the implementations of modules like Strings, Vectors,
 * Files, and Math.
 * 
 * @note The lifetime of this namespace is tied to the library initialization.
 * Users must not attempt to modify the function pointers or the structure itself.
 * Access is strictly read-only via the external global instance `CBridge`.
 * 
 * @section Usage Example
 * @code{c}
 * // Initialize modules if required (some might be lazy-loaded)
 * cbridge_init();
 * 
 * // Access string functions
 * const struct string_namespace* str = CBridge.str;
 * char* result = str->concat("Hello ", "World");
 * 
 * // Access math functions
 * const struct math_namespace* math = CBridge.math;
 * float pi = math->get_pi();
 * @endcode
 */
struct CBridge_Namespace {
    const struct cbridge_string_namespace* str;
    const struct cbridge_vector_namespace* vector;
    const struct cbridge_file_namespace*   file;
    const struct cbridge_math_namespace*   math;
    const struct cbridge_net_namespace*    net;
};

extern const struct CBridge_Namespace CBridge;

#endif    // CBRIDGE_MAIN_HPP
