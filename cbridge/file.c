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
    fseek(f, 0, SEEK_END);
    long long size = ftell(f);
    fclose(f);
    return size;
}

static string_t* impl_read_all(const char* path) {
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
            string_t* val = cbridge_string.catch_in_range(s_line, separator, "\n");
            cbridge_string.trim(val);

            // Swap to result and break
            cbridge_string.free(result);
            result = val;
            cbridge_string.free(s_line);
            break;
        }
        cbridge_string.free(s_line);
    }
    fclose(f);
    return result;
}

static cbridge_vector_t* impl_get_files(const char* directory, const char* prefix, const char* suffix) {
    cbridge_vector_t* vec = cbridge_vector.create();
    if (!directory) return vec;

#ifdef _WIN32
    char search_path[MAX_PATH];
    sprintf(search_path, "%s\\*", directory);
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

const struct cbridge_file_namespace cbridge_file = {.exists                = impl_exists,
                                                    .get_size              = impl_get_size,
                                                    .read_all              = impl_read_all,
                                                    .get_working_directory = impl_get_working_directory,
                                                    .get_parameter         = impl_get_parameter,
                                                    .get_files             = impl_get_files};