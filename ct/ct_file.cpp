#include "ct_file.hpp"
#include "ct_str.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #include <dirent.h>
    #define GetCurrentDir getcwd
#endif

namespace ct {
    namespace file {

        bool exists(const std::string& path) {
            std::ifstream f(path.c_str());
            return f.good();
        }

        long long getSize(const std::string& filename) {
            struct stat stat_buf;
            int         rc = stat(filename.c_str(), &stat_buf);
            return rc == 0 ? static_cast<long long>(stat_buf.st_size) : -1;
        }

        std::string readAll(const std::string& path) {
            std::ifstream in(path, std::ios::in | std::ios::binary);
            if (!in) return "";
            std::ostringstream s;
            s << in.rdbuf();
            std::string content = s.str();
            // Normalize line endings
            return ct::str::findAndReplace(content, "\r", "");
        }

        std::vector<std::string> readLines(const std::string& filename) {
            std::vector<std::string> lines;
            std::ifstream            file(filename);
            std::string              line;
            while (std::getline(file, line)) {
                lines.push_back(line);
            }
            return lines;
        }

        bool writeAll(const std::string& filename, const std::string& content) {
            std::ofstream out(filename);
            if (!out) return false;
            out << content;
            return true;
        }

        bool appendLine(const std::string& filename, const std::string& line) {
            std::ofstream out(filename, std::ios::app);
            if (!out) return false;
            out << line << "\n";
            return true;
        }

        std::string getParameter(const std::string& filename, const std::string& key, const std::string& separator) {
            std::ifstream file(filename);
            if (!file) return "";

            std::string line;
            while (std::getline(file, line)) {
                // 1. Remove comments
                size_t commentPos = line.find_first_of("/#*;!");
                if (commentPos != std::string::npos) {
                    line = line.substr(0, commentPos);
                }

                // 2. Search for key
                size_t sepPos = line.find(separator);
                if (sepPos != std::string::npos) {
                    std::string currentKey = ct::str::trim(line.substr(0, sepPos));
                    if (currentKey == key) {
                        return ct::str::trim(line.substr(sepPos + separator.length()));
                    }
                }
            }
            return "";
        }

        std::string getWorkingDirectory() {
            char buf[1024];
            return GetCurrentDir(buf, sizeof(buf)) ? std::string(buf) : "";
        }

        std::string getExtension(const std::string& path) {
            size_t dotPos = path.find_last_of('.');
            if (dotPos != std::string::npos) {
                return path.substr(dotPos);
            }
            return "";
        }

        bool remove(const std::string& filename) { return std::remove(filename.c_str()) == 0; }

#ifdef _WIN32
        std::vector<std::string> getFiles(const std::string& directory, const std::string& prefix, const std::string& suffix) {
            std::vector<std::string> result;
            std::string              searchPath = directory + "\\*";
            WIN32_FIND_DATAA         fd;
            HANDLE                   hFind = FindFirstFileA(searchPath.c_str(), &fd);

            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                        std::string name        = fd.cFileName;
                        bool        matchPrefix = prefix.empty() || name.find(prefix) == 0;
                        bool        matchSuffix =
                            suffix.empty() || (name.length() >= suffix.length() && name.compare(name.length() - suffix.length(), suffix.length(), suffix) == 0);

                        if (matchPrefix && matchSuffix) result.push_back(name);
                    }
                } while (FindNextFileA(hFind, &fd));
                FindClose(hFind);
            }
            return result;
        }
#else
        std::vector<std::string> getFiles(const std::string& directory, const std::string& prefix, const std::string& suffix) {
            std::vector<std::string> result;
            DIR*                     dir = opendir(directory.c_str());
            if (dir) {
                struct dirent* ent;
                while ((ent = readdir(dir)) != nullptr) {
                    if (ent->d_type == DT_REG) {
                        std::string name        = ent->d_name;
                        bool        matchPrefix = prefix.empty() || name.find(prefix) == 0;
                        bool        matchSuffix =
                            suffix.empty() || (name.length() >= suffix.length() && name.compare(name.length() - suffix.length(), suffix.length(), suffix) == 0);

                        if (matchPrefix && matchSuffix) result.push_back(name);
                    }
                }
                closedir(dir);
            }
            return result;
        }
#endif

    }    // namespace file
}    // namespace ct