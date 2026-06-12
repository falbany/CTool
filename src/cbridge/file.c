#include "file.h"
#include "string.h"
#include "vector.h"
#include "../internal/platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if PLATFORM_WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #define GetCurrentDir getcwd
#endif

static bool impl_exists(const char* path) {
    if (!path) return false;
    FILE* f = fopen(path, "r");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}

static long long impl_get_size(const char* path) {
    if (!path) return 0;
    FILE* f = fopen(path, "rb");
    if (!f) return 0;
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return 0;
    }
    long long size = ftell(f);
    fclose(f);
    return (size >= 0) ? size : 0;
}

static string_t* impl_read_all(const char* path) {
    if (!path) return NULL;
    FILE* f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = (char*)malloc(size + 1);
    if (buffer) {
        fread(buffer, 1, size, f);
        buffer[size] = '\0';
    }
    fclose(f);

    string_t* res = cbridge_string.create(buffer);
    free(buffer);
    return res;
}

static string_t* impl_get_working_directory() {
    char buf[1024];
    if (GetCurrentDir(buf, sizeof(buf))) {
        return cbridge_string.create(buf);
    }
    return cbridge_string.create("");
}

static string_t* impl_get_parameter(const char* path, const char* key, const char* separator) {
    FILE* f = fopen(path, "r");
    if (!f) return cbridge_string.create("");

    char      line[1024];
    string_t* result = cbridge_string.create("");

    while (fgets(line, sizeof(line), f)) {
        string_t* s_line = cbridge_string.create(line);
        cbridge_string.trim(s_line);

        // Skip comments or empty lines
        if (cbridge_string.empty(s_line) || cbridge_string.starts_with(s_line, "#") || cbridge_string.starts_with(s_line, "//")) {
            cbridge_string.free(s_line);
            continue;
        }

        if (cbridge_string.find(s_line, key, 0) != CBRIDGE_STR_NPOS) {
            // Find the separator in the trimmed line, then extract everything after it
            size_t sep_pos = cbridge_string.find(s_line, separator, 0);
            if (sep_pos != CBRIDGE_STR_NPOS) {
                sep_pos += strlen(separator);
                size_t    remaining = cbridge_string.length(s_line) - sep_pos;
                string_t* val       = cbridge_string.substr(s_line, sep_pos, remaining);
                cbridge_string.trim(val);

                // Swap to result and break
                cbridge_string.free(result);
                result = val;
            }
            cbridge_string.free(s_line);
            break;
        }
        cbridge_string.free(s_line);
    }
    fclose(f);
    return result;
}

static vector_t* impl_get_files(const char* directory, const char* prefix, const char* suffix) {
    vector_t* vec = cbridge_vector.create();
    if (!directory) return vec;

#if PLATFORM_WINDOWS
    char search_path[MAX_PATH];
    snprintf(search_path, sizeof(search_path), "%s\\*", directory);
    WIN32_FIND_DATA fd;
    HANDLE          hFind = FindFirstFile(search_path, &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                string_t* name  = cbridge_string.create(fd.cFileName);
                bool      match = true;
                if (prefix && !cbridge_string.starts_with(name, prefix)) match = false;
                if (suffix && !cbridge_string.ends_with(name, suffix)) match = false;

                if (match)
                    cbridge_vector.push_back(vec, name);
                else
                    cbridge_string.free(name);
            }
        } while (FindNextFile(hFind, &fd));
        FindClose(hFind);
    }
#else
    DIR* d = opendir(directory);
    if (d) {
        struct dirent* dir;
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {    // Regular file
                string_t* name  = cbridge_string.create(dir->d_name);
                bool      match = true;
                if (prefix && !cbridge_string.starts_with(name, prefix)) match = false;
                if (suffix && !cbridge_string.ends_with(name, suffix)) match = false;

                if (match)
                    cbridge_vector.push_back(vec, name);
                else
                    cbridge_string.free(name);
            }
        }
        closedir(d);
    }
#endif
    return vec;
}

static string_t* impl_read_lines(const char* path, size_t startLine, size_t endLine) {
    /* --- Input validation (before any I/O) --- */
    if (!path) {
        return cbridge_string.create("");
    }

    /* Treat 0 as 1 for both startLine and endLine (1-based indexing) */
    if (startLine == 0) {
        startLine = 1;
    }
    if (endLine == 0) {
        endLine = 1;
    }

    /* Invalid range: start must not exceed end */
    if (startLine > endLine) {
        return cbridge_string.create("");
    }

    /* --- Open file --- */
    FILE* f = fopen(path, "r");
    if (!f) {
        return cbridge_string.create("");
    }

    /* --- Single pass: skip to startLine, then collect until endLine --- */
    char      line[4096];
    size_t    currentLine = 0;
    string_t* result      = NULL;
    bool      inRange     = false;

    while (fgets(line, sizeof(line), f)) {
        currentLine++;

        /* Check if we have reached the end of the requested range */
        if (currentLine > endLine) {
            break;
        }

        /* Skip lines before the requested range */
        if (currentLine < startLine) {
            continue;
        }

        /* --- We are in the requested range [startLine, endLine] --- */

        /* Remove trailing newline/carriage-return */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
            if (len > 0 && line[len - 1] == '\r') {
                line[len - 1] = '\0';
                len--;
            }
        }

        /* Initialize result on first in-range line */
        if (!inRange) {
            result  = cbridge_string.create(line);
            inRange = true;
        } else {
            cbridge_string.append(result, "\n");
            cbridge_string.append(result, line);
        }
    }

    fclose(f);

    /* If no lines were in range (file ended before startLine), return empty */
    if (!inRange) {
        return cbridge_string.create("");
    }

    return result;
}

static bool impl_write_all(const char* path, const char* content) {
    if (!path) return false;
    /* Use binary mode to avoid CRLF translation on Windows so tests observe exact bytes */
    FILE* f = fopen(path, "wb");
    if (!f) return false;
    if (content) fputs(content, f);
    fclose(f);
    return true;
}

static bool impl_append_all(const char* path, const char* content) {
    if (!path) return false;
    /* Use binary append to avoid CRLF translation on Windows */
    FILE* f = fopen(path, "ab");
    if (!f) return false;
    if (content) fputs(content, f);
    fclose(f);
    return true;
}

static string_t* impl_get_extension(const char* path) {
    if (!path) return cbridge_string.create("");

    // Find last dot in the filename portion
    const char* dot = strrchr(path, '.');
    if (!dot) return cbridge_string.create("");

    // Make sure the dot is in the filename, not just a path separator
    // e.g., "/path/to/.hidden" should return "" (no extension)
    const char* lastSlash = NULL;
    const char* p         = path;
    while (*p) {
        if (*p == '/' || *p == '\\') lastSlash = p + 1;
        p++;
    }
    if (!lastSlash) lastSlash = path;

    if (dot <= lastSlash) return cbridge_string.create("");

    return cbridge_string.create(dot);
}

static bool impl_remove(const char* path) {
    if (!path) return false;
    return (remove(path) == 0);
}

static bool impl_copy(const char* src, const char* dest) {
    if (!src || !dest) return false;

    FILE* fsrc = fopen(src, "rb");
    if (!fsrc) return false;

    FILE* fdest = fopen(dest, "wb");
    if (!fdest) {
        fclose(fsrc);
        return false;
    }

    char   buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), fsrc)) > 0) {
        if (fwrite(buf, 1, n, fdest) != n) {
            fclose(fsrc);
            fclose(fdest);
            return false;
        }
    }

    fclose(fsrc);
    bool ok = (fclose(fdest) == 0);
    return ok;
}

static bool impl_is_directory(const char* path) {
    if (!path) return false;

#if PLATFORM_WINDOWS
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) return false;
    return (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISDIR(st.st_mode);
#endif
}

static string_t* impl_get_filename(const char* path) {
    if (!path || *path == '\0') return cbridge_string.create("");

    // Find the last path separator (support both / and \)
    const char* lastSep = path;
    const char* p       = path;
    while (*p) {
        if (*p == '/' || *p == '\\') lastSep = p + 1;
        p++;
    }

    return cbridge_string.create(lastSep);
}

static bool impl_create_directory(const char* path) {
    if (!path) return false;

    // If path already exists as a file, return false
    if (impl_exists(path) && !impl_is_directory(path)) return false;

    // If directory already exists, return false
    if (impl_is_directory(path)) return false;

#if PLATFORM_WINDOWS
    return (_mkdir(path) == 0);
#else
    return (mkdir(path, 0755) == 0);
#endif
}

static bool impl_move(const char* src, const char* dest) {
    if (!src || !dest) return false;
    if (!impl_exists(src)) return false;

    if (rename(src, dest) == 0) return true;

    // Fallback: copy + remove (handles cross-device moves on Windows)
    if (impl_copy(src, dest)) {
        impl_remove(src);
        return true;
    }

    return false;
}

static string_t* impl_read_last_lines(const char* path, size_t count) {
    if (!path || count == 0) return cbridge_string.create("");

    FILE* f = fopen(path, "r");
    if (!f) return cbridge_string.create("");

    // Single-pass sliding buffer approach:
    // Maintain a circular buffer of `count` lines.
    // At the end, the buffer contains the last `count` lines in order.

    // Allocate array of string_t* pointers (circular buffer)
    string_t** buf      = (string_t**)malloc(sizeof(string_t*) * count);
    bool*      has_line = (bool*)calloc(count, sizeof(bool));
    if (!buf || !has_line) {
        fclose(f);
        free(buf);
        free(has_line);
        return cbridge_string.create("");
    }

    char   line[4096];
    size_t totalLines = 0;

    while (fgets(line, sizeof(line), f)) {
        // Strip trailing newline/carriage-return
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
            if (len > 0 && line[len - 1] == '\r') {
                line[len - 1] = '\0';
            }
        }

        size_t pos = totalLines % count;
        if (has_line[pos]) {
            cbridge_string.free(buf[pos]);
        }
        buf[pos]      = cbridge_string.create(line);
        has_line[pos] = true;
        totalLines++;
    }

    fclose(f);

    // Build result from the circular buffer in correct order
    string_t* result = cbridge_string.create("");

    if (totalLines == 0) {
        // Free buffer entries
        for (size_t i = 0; i < count; i++) {
            if (has_line[i]) cbridge_string.free(buf[i]);
        }
        free(buf);
        free(has_line);
        return result;
    }

    // Starting index in the circular buffer
    size_t start    = (totalLines >= count) ? (totalLines % count) : 0;
    size_t numLines = (totalLines < count) ? totalLines : count;

    for (size_t i = 0; i < numLines; i++) {
        size_t pos = (start + i) % count;
        if (i > 0) cbridge_string.append(result, "\n");
        cbridge_string.append(result, cbridge_string.c_str(buf[pos]));
        cbridge_string.free(buf[pos]);
    }

    free(buf);
    free(has_line);
    return result;
}

static string_t* impl_get_directory(const char* path) {
    if (!path || *path == '\0') return cbridge_string.create("");

    // Find the last path separator (support both / and \)
    const char* lastSep = NULL;
    const char* p       = path;
    while (*p) {
        if (*p == '/' || *p == '\\') lastSep = p;
        p++;
    }

    // No separator found -> bare filename, no directory component
    if (!lastSep) return cbridge_string.create("");

    // Extract everything before the last separator
    size_t len = (size_t)(lastSep - path);

    // Special-case: when separator is at position 0 (e.g. "/file.txt"),
    // the directory is the root "/" (not an empty string).
    if (len == 0) {
        if (path[0] == '/') return cbridge_string.create("/");
        if (path[0] == '\\') return cbridge_string.create("\\");
        return cbridge_string.create("");
    }

    // Strip trailing separators from the result
    while (len > 0 && (path[len - 1] == '/' || path[len - 1] == '\\')) len--;

    if (len == 0) {
        if (path[0] == '/') return cbridge_string.create("/");
        if (path[0] == '\\') return cbridge_string.create("\\");
        return cbridge_string.create("");
    }

    // Allocate and copy the directory portion
    char* dir = (char*)malloc(len + 1);
    if (!dir) return cbridge_string.create("");
    memcpy(dir, path, len);
    dir[len] = '\0';

    // If the original path uses forward slashes anywhere, normalize any
    // backslashes in the extracted directory to forward slashes so that
    // mixed-separator inputs like "/path\\to\\file.txt" become
    // "/path/to" as expected by tests.
    if (strchr(path, '/') != NULL) {
        for (size_t i = 0; i < len; i++) {
            if (dir[i] == '\\') dir[i] = '/';
        }
    }

    string_t* res = cbridge_string.create(dir);
    free(dir);
    return res;
}

static string_t* impl_get_basename(const char* path) {
    if (!path || *path == '\0') return cbridge_string.create("");

    // Find the filename portion (after the last separator)
    const char* filename = path;
    const char* p        = path;
    while (*p) {
        if (*p == '/' || *p == '\\') filename = p + 1;
        p++;
    }

    // Empty filename (e.g., path ends with separator)
    if (*filename == '\0') return cbridge_string.create("");

    // Find the last dot in the filename portion
    const char* dot = strrchr(filename, '.');
    if (!dot) return cbridge_string.create(filename);

    // Ensure the dot is actually in the filename, not a leading dot for hidden files
    // e.g., ".gitignore" has no extension
    if (dot == filename) return cbridge_string.create(filename);

    // Extract basename (filename without extension)
    size_t len  = (size_t)(dot - filename);
    char*  base = (char*)malloc(len + 1);
    if (!base) return cbridge_string.create("");
    memcpy(base, filename, len);
    base[len] = '\0';

    string_t* res = cbridge_string.create(base);
    free(base);
    return res;
}

static bool impl_create_directories(const char* path) {
    if (!path) return false;

    // If the full path already exists as a directory, return true (idempotent)
    if (impl_is_directory(path)) return true;

    // If path exists but is a file, return false
    if (impl_exists(path)) return false;

    // Make a mutable copy of the path
    size_t len  = strlen(path);
    char*  work = (char*)malloc(len + 1);
    if (!work) return false;
    memcpy(work, path, len + 1);

    // Normalize separators to '/' for uniform processing
    for (size_t i = 0; i < len; i++) {
        if (work[i] == '\\') work[i] = '/';
    }

    // Walk from root to leaf, creating each missing component
    bool success = true;
    for (size_t i = 1; i < len; i++) {
        if (work[i] == '/') {
            work[i] = '\0';

            // Skip if this component already exists as a directory
            if (!impl_is_directory(work)) {
                // If it exists as a file, fail
                if (impl_exists(work)) {
                    success = false;
                    break;
                }

#if PLATFORM_WINDOWS
                if (_mkdir(work) != 0) {
                    success = false;
                    break;
                }
#else
                if (mkdir(work, 0755) != 0) {
                    success = false;
                    break;
                }
#endif
            }

            work[i] = '/';
        }
    }

    // Create the final component (the leaf directory)
    if (success && !impl_is_directory(work)) {
        if (impl_exists(work)) {
            success = false;
        } else {
#if PLATFORM_WINDOWS
            if (_mkdir(work) != 0) success = false;
#else
            if (mkdir(work, 0755) != 0) success = false;
#endif
        }
    }

    free(work);
    return success;
}

static bool impl_remove_directory(const char* path) {
    if (!path) return false;

    // Must exist and be a directory
    if (!impl_is_directory(path)) return false;

#if PLATFORM_WINDOWS
    return (RemoveDirectoryA(path) != 0);
#else
    return (rmdir(path) == 0);
#endif
}

const struct cbridge_file_namespace cbridge_file = {.exists                = impl_exists,
                                                    .get_size              = impl_get_size,
                                                    .read_all              = impl_read_all,
                                                    .get_working_directory = impl_get_working_directory,
                                                    .get_parameter         = impl_get_parameter,
                                                    .read_lines            = impl_read_lines,
                                                    .get_files             = impl_get_files,
                                                    .write_all             = impl_write_all,
                                                    .append_all            = impl_append_all,
                                                    .get_extension         = impl_get_extension,
                                                    .remove                = impl_remove,
                                                    .copy                  = impl_copy,
                                                    .is_directory          = impl_is_directory,
                                                    .get_filename          = impl_get_filename,
                                                    .create_directory      = impl_create_directory,
                                                    .move                  = impl_move,
                                                    .read_last_lines       = impl_read_last_lines,
                                                    .get_directory         = impl_get_directory,
                                                    .get_basename          = impl_get_basename,
                                                    .create_directories    = impl_create_directories,
                                                    .remove_directory      = impl_remove_directory};