#include "cb_file.h"
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

static cb_string_t* impl_read_all(const char* path) {
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

    cb_string_t* res = cb_str.create(buffer);
    free(buffer);
    return res;
}

static cb_string_t* impl_get_working_directory() {
    char buf[1024];
    if (GetCurrentDir(buf, sizeof(buf))) {
        return cb_str.create(buf);
    }
    return cb_str.create("");
}

static cb_string_t* impl_get_parameter(const char* path, const char* key, const char* separator) {
    FILE* f = fopen(path, "r");
    if (!f) return cb_str.create("");

    char         line[1024];
    cb_string_t* result = cb_str.create("");

    while (fgets(line, sizeof(line), f)) {
        cb_string_t* s_line = cb_str.create(line);
        cb_str.trim(s_line);

        // Skip comments or empty lines
        if (cb_str.empty(s_line) || cb_str.starts_with(s_line, "#") || cb_str.starts_with(s_line, "//")) {
            cb_str.free(s_line);
            continue;
        }

        if (cb_str.find(s_line, key, 0) != CB_STR_NPOS) {
            cb_string_t* val = cb_str.catch_in_range(s_line, separator, "\n");
            cb_str.trim(val);

            // Swap to result and break
            cb_str.free(result);
            result = val;
            cb_str.free(s_line);
            break;
        }
        cb_str.free(s_line);
    }
    fclose(f);
    return result;
}

static cb_vector_t* impl_get_files(const char* directory, const char* prefix, const char* suffix) {
    cb_vector_t* vec = cb_vector.create();
    if (!directory) return vec;

#ifdef _WIN32
    char search_path[MAX_PATH];
    sprintf(search_path, "%s\\*", directory);
    WIN32_FIND_DATA fd;
    HANDLE          hFind = FindFirstFile(search_path, &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                cb_string_t* name  = cb_str.create(fd.cFileName);
                bool         match = true;
                if (prefix && !cb_str.starts_with(name, prefix)) match = false;
                if (suffix && !cb_str.ends_with(name, suffix)) match = false;

                if (match)
                    cb_vector.push_back(vec, name);
                else
                    cb_str.free(name);
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
                cb_string_t* name  = cb_str.create(dir->d_name);
                bool         match = true;
                if (prefix && !cb_str.starts_with(name, prefix)) match = false;
                if (suffix && !cb_str.ends_with(name, suffix)) match = false;

                if (match)
                    cb_vector.push_back(vec, name);
                else
                    cb_str.free(name);
            }
        }
        closedir(d);
    }
#endif
    return vec;
}

const struct cb_file_namespace cb_file = {.exists                = impl_exists,
                                          .get_size              = impl_get_size,
                                          .read_all              = impl_read_all,
                                          .get_working_directory = impl_get_working_directory,
                                          .get_parameter         = impl_get_parameter,
                                          .get_files             = impl_get_files};