/**
 * @file libctool.h
 * @author Florent ALBANY (FAL)
 * @brief Logic for DLL export/import and static linking.
 * @version 1.1
 * @date 2026-03-24
 */

#ifndef LIBCTOOL_H
#define LIBCTOOL_H

/*
 * COMPILATION LOGIC:
 * 1. Default: Static linking (LIBCTOOL_API is empty).
 * 2. Define CTOOL_DYNAMIC: Enables DLL export/import logic.
 * - Define BUILDING_CTOOL_DLL: To export symbols (when building the DLL).
 * - Don't define it: To import symbols (when using the DLL).
 */

#ifdef CTOOL_DYNAMIC
    // Mode DLL activé
    #if defined(_WIN32) || defined(__CYGWIN__)
        #ifdef BUILDING_CTOOL_DLL
            #define LIBCTOOL_API __declspec(dllexport)
        #else
            #define LIBCTOOL_API __declspec(dllimport)
        #endif
    #else
        // Linux / macOS : Visibilité par défaut pour GCC/Clang
        #if __GNUC__ >= 4
            #define LIBCTOOL_API __attribute__((visibility("default")))
        #else
            #define LIBCTOOL_API
        #endif
    #endif
#else
    // DEFAULT MODE : Static compilation
    // The code is compiled in standard mode.
    #define LIBCTOOL_API
#endif

#endif    // LIBCTOOL_H