/**
 * \file      platform.h
 * \brief     Platform-specific configuration and detection macros
 * \author    Florent ALBANY - FAL (f.albany@serma.com)
 * \version   1.0
 * \date      2026-05-26
 *
 * \details Centralized platform detection and cross-platform compatibility layer.
 *          Replaces scattered preprocessor macros with unified constants.
 *
 * \note This header should be included before any platform-specific headers
 */

#ifndef PLATFORM_H
#define PLATFORM_H

// =============================================================================
// PLATFORM DETECTION
// =============================================================================

// Detect Windows (covers all variants: MSVC, MinGW, Cygwin)
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WIN64__) || defined(__CYGWIN__) || defined(__CYGWIN32__)
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_NAME "Windows"

// Detect Linux
#elif defined(__linux__) || defined(__linux) || defined(linux)
    #define PLATFORM_LINUX 1
    #define PLATFORM_NAME "Linux"

// Detect macOS (Apple Darwin)
#elif defined(__APPLE__) && defined(__MACH__)
    #define PLATFORM_MACOS 1
    #define PLATFORM_NAME "macOS"

// Detect BSD variants
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
    #define PLATFORM_BSD 1
    #define PLATFORM_NAME "BSD"

// Detect Solaris
#elif defined(__sun) || defined(sun)
    #define PLATFORM_SOLARIS 1
    #define PLATFORM_NAME "Solaris"

// Unknown platform
#else
    #error "Platform not supported. Please add support for your platform in platform.h"
#endif

// =============================================================================
// COMPILER DETECTION
// =============================================================================

// Detect compiler
#if defined(_MSC_VER)
    #define COMPILER_MSVC 1
    #define COMPILER_NAME "MSVC"
    #define COMPILER_VERSION _MSC_VER

#elif defined(__clang__)
    #define COMPILER_CLANG 1
    #define COMPILER_NAME "Clang"
    #define COMPILER_VERSION __clang_major__

#elif defined(__GNUC__) || defined(__GNUG__)
    #define COMPILER_GCC 1
    #define COMPILER_NAME "GCC"
    #define COMPILER_VERSION __GNUC__

#else
    #define COMPILER_UNKNOWN 1
    #define COMPILER_NAME "Unknown"
    #define COMPILER_VERSION 0
#endif

// =============================================================================
// ARCHITECTURE DETECTION
// =============================================================================

#if defined(_M_X64) || defined(__x86_64__)
    #define ARCH_X64 1
    #define ARCH_NAME "x86_64"

#elif defined(_M_IX86) || defined(__i386__)
    #define ARCH_X86 1
    #define ARCH_NAME "x86"

#elif defined(__arm__) || defined(_M_ARM)
    #define ARCH_ARM 1
    #define ARCH_NAME "ARM"

#elif defined(__aarch64__)
    #define ARCH_ARM64 1
    #define ARCH_NAME "ARM64"

#else
    #define ARCH_UNKNOWN 1
    #define ARCH_NAME "Unknown"
#endif

// =============================================================================
// CROSS-PLATFORM TYPE DEFINITIONS
// =============================================================================

#if PLATFORM_WINDOWS
    // Windows-specific types
    #include <windows.h>
// Note: unistd.h is not available on Windows; use Windows API alternatives
// For usleep(), define a compatibility macro or use Sleep()
typedef HINSTANCE DllHandle;
typedef DWORD     PlatformError;

#elif PLATFORM_LINUX || PLATFORM_MACOS || PLATFORM_BSD || PLATFORM_SOLARIS
    // Unix-like systems
    #include <dlfcn.h>
    #include <unistd.h>      // For usleep()
    #include <dirent.h>      // For opendir(), readdir()
    #include <sys/stat.h>    // For stat()
typedef void* DllHandle;
typedef int   PlatformError;
#endif

// =============================================================================
// CROSS-PLATFORM SLEEP FUNCTION (pre-C++11 compatible)
// =============================================================================

/**
 * \brief Sleep for a specified number of milliseconds (cross-platform, pre-C++11)
 *
 * Provides a platform-independent sleep function using native OS APIs.
 * Replaces external sleep_api dependency while maintaining pre-C++11 compatibility.
 *
 * \param milliseconds Duration to sleep in milliseconds
 *
 * \note Windows: Uses Sleep() API (resolution ~15ms)
 * \note Unix-like: Uses usleep() for sub-second precision
 */
static inline void sleep_ms(unsigned long milliseconds) {
#if PLATFORM_WINDOWS
    // Windows: Sleep() takes milliseconds, max value is 50000ms
    while (milliseconds > 50000) {
        Sleep(50000);
        milliseconds -= 50000;
    }
    Sleep((DWORD)milliseconds);
#else
    // Unix-like systems: usleep() takes microseconds
    while (milliseconds > 500) {
        usleep(500000);    // 500ms in microseconds
        milliseconds -= 500;
    }
    usleep((useconds_t)(milliseconds * 1000));
#endif
}

// =============================================================================
// CROSS-PLATFORM FILE PATH SEPARATOR
// =============================================================================

#if PLATFORM_WINDOWS
    #define PATH_SEPARATOR '\\'
    #define PATH_SEPARATOR_STR "\\"
#else
    #define PATH_SEPARATOR '/'
    #define PATH_SEPARATOR_STR "/"
#endif

// =============================================================================
// CROSS-PLATFORM DLL LOADING CONSTANTS
// =============================================================================

#if PLATFORM_WINDOWS
    #define DL_LAZY_LOAD LOAD_LIBRARY_SEARCH_APPLICATION_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32
    #define DL_ERROR_GET() GetLastError()
    #define DL_FREE_HANDLE(h) FreeLibrary(h)
#else
    #define DL_LAZY_LOAD RTLD_LAZY
    #define DL_ERROR_GET() errno
    #define DL_FREE_HANDLE(h) dlclose(h)
#endif

// =============================================================================
// COMPILER-SPECIFIC DIRECTIVES
// =============================================================================

// Disable warnings (compiler-specific)
#if COMPILER_MSVC
    #pragma warning(disable : 4996)    // Deprecated functions
    #define DEPRECATED(msg) __declspec(deprecated(msg))
#elif COMPILER_GCC || COMPILER_CLANG
    #define DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
    #define DEPRECATED(msg)
#endif

// Format specifiers for platform-independent printf
#if PLATFORM_WINDOWS && COMPILER_MSVC
    #define PRId32 "I32d"
    #define PRId64 "I64d"
    #define PRIu64 "I64u"
    #define PRIx64 "I64x"
#else
    #include <inttypes.h>
#endif

// =============================================================================
// UTILITY MACROS
// =============================================================================

// Stringify macro
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// Compile-time assertion
#if COMPILER_CXX11_OR_GREATER
    #define STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#else
    #define STATIC_ASSERT(cond, msg) typedef char static_assert_##msg[(cond) ? 1 : -1]
#endif

// Platform information string (for logging/debugging)
#define PLATFORM_INFO "Platform: " PLATFORM_NAME " (" ARCH_NAME ") - " COMPILER_NAME " " TOSTRING(COMPILER_VERSION)

// DLLEXPORT macro for Windows DLLs
#if PLATFORM_WINDOWS
    #define DLLEXPORT __declspec(dllexport)
#else
    #define DLLEXPORT
#endif

#endif /* PLATFORM_H */
