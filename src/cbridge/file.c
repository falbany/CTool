#include "file.h"
#include "string.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #include <dirent.h>
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

#ifdef _WIN32
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
    if (startLine == 0) {startLine = 1; }
    if (endLine == 0) {endLine = 1; }

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
    char        line[4096];
    size_t      currentLine = 0;
    string_t*   result      = NULL;
    bool        inRange     = false;

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

const struct cbridge_file_namespace cbridge_file = {
    .exists                = impl_exists,
    .get_size              = impl_get_size,
    .read_all              = impl_read_all,
    .get_working_directory = impl_get_working_directory,
    .get_parameter         = impl_get_parameter,
    .read_lines            = impl_read_lines,
    .get_files             = impl_get_files
};