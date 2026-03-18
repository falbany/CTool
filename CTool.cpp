#include "CTool.hpp"
#include <iomanip>
#include <algorithm>
#include <cctype>

// OS-specific includes required for C++11 directory traversal
#if defined(_WIN32)
    #include <windows.h>
    #include <direct.h>
    #define getcwd _getcwd
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <string.h>
#endif

namespace ct {

// ============================================================================
// LOG API
// ============================================================================
namespace log {
    void message(Level msgLevel, const std::string& msg, std::ostream& stream, Level currentLevel) {
        if (static_cast<int>(msgLevel) <= static_cast<int>(currentLevel)) {
            stream << "[" << ct::time::getCurrentTimestamp() << "] ";
            switch (msgLevel) {
                case Level::Error:   stream << "[ERROR] "; break;
                case Level::Warning: stream << "[WARNING] "; break;
                case Level::Info:    stream << "[INFO] "; break;
                case Level::Debug:   stream << "[DEBUG] "; break;
                default:             stream << "[UNKNOWN] "; break;
            }
            stream << msg << std::endl;
        }
    }

    void toCout(Level msgLevel, const std::string& msg, Level currentLevel) {
        message(msgLevel, msg, std::cout, currentLevel);
    }

    void toCerr(Level msgLevel, const std::string& msg, Level currentLevel) {
        message(msgLevel, msg, std::cerr, currentLevel);
    }
}

// ============================================================================
// TIME API
// ============================================================================
namespace time {
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm bt = *std::localtime(&in_time_t);

        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &bt);
        return std::string(buffer);
    }

    void sleepMs(long milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    void sleepS(long seconds) {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }

    long long getUptimeMs() {
        auto now = std::chrono::steady_clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    }
}

// ============================================================================
// STRING API
// ============================================================================
namespace str {
    void assign(std::string& dest, const char* src) {
        if (src) dest.assign(src);
        else dest.clear();
    }

    std::string updateField(const std::string& currentStr, const std::string& prefix, const std::string& newValue, char delimiter) {
        std::vector<std::string> tokens = split(currentStr, delimiter);
        bool found = false;
        std::string result;

        for (size_t i = 0; i < tokens.size(); ++i) {
            if (tokens[i].compare(0, prefix.length(), prefix) == 0) {
                tokens[i] = prefix + newValue;
                found = true;
            }
            result += tokens[i] + (i == tokens.size() - 1 ? "" : std::string(1, delimiter));
        }
        if (!found) {
            result += (result.empty() ? "" : std::string(1, delimiter)) + prefix + newValue;
        }
        return result;
    }

    std::string findAndReplace(const std::string& sIn, const std::string& searchStr, const std::string& replaceStr) {
        std::string sOut = sIn;
        if (sOut.empty() || searchStr.empty()) return sOut;
        std::size_t pos = 0;
        while ((pos = sOut.find(searchStr, pos)) != std::string::npos) {
            sOut.replace(pos, searchStr.length(), replaceStr);
            pos += replaceStr.length();
        }
        return sOut;
    }

    std::string findAndReplaceRecursive(const std::string& sIn, const std::string& searchStr, const std::string& replaceStr) {
        std::string sOut = sIn;
        if (sOut.empty() || searchStr.empty()) return sOut;
        if (replaceStr.find(searchStr) != std::string::npos) {
            ct::log::toCerr(ct::log::Level::Error, "Recursive replace error: target contains search keyword.");
            return sOut;
        }
        std::size_t pos = 0;
        while ((pos = sOut.find(searchStr, pos)) != std::string::npos) {
            sOut.replace(pos, searchStr.length(), replaceStr);
        }
        return sOut;
    }

    std::string catchInRange(const std::string& sIn, const std::string& searchStr1, const std::string& searchStr2) {
        std::size_t startPos = sIn.find(searchStr1);
        if (startPos == std::string::npos) return "";
        startPos += searchStr1.length();
        std::size_t endPos = sIn.find(searchStr2, startPos);
        if (endPos == std::string::npos) return "";
        return sIn.substr(startPos, endPos - startPos);
    }

    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            if (!item.empty()) tokens.push_back(item);
        }
        return tokens;
    }

    std::string trim(const std::string& str) {
        auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); });
        auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) { return std::isspace(c); }).base();
        return (start < end ? std::string(start, end) : std::string());
    }
}

// ============================================================================
// FILE API
// ============================================================================
namespace file {
    bool exists(const std::string& filename) {
        std::ifstream f(filename.c_str());
        return f.good();
    }

    long long getSize(const std::string& filename) {
        std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
        return in.is_open() ? static_cast<long long>(in.tellg()) : 0;
    }

    std::string readAll(const std::string& filename) {
        std::ifstream in(filename, std::ios::in | std::ios::binary);
        if (in) {
            std::ostringstream contents;
            contents << in.rdbuf();
            return ct::str::findAndReplace(contents.str(), "\r", "\n");
        }
        return "";
    }

    std::string getWorkingDirectory() {
        char buffer[2048];
        if (getcwd(buffer, sizeof(buffer)) != nullptr) {
            return std::string(buffer);
        }
        ct::log::toCerr(ct::log::Level::Error, "Failed to get working directory.");
        return "";
    }

    std::string getParameter(const std::string& filename, const std::string& key, const std::string& separator) {
        if (!exists(filename)) return "";
        
        std::string sTxt = "\n" + readAll(filename) + "\n";
        sTxt = ct::str::findAndReplaceRecursive(sTxt, "\n\n", "\n");
        
        // Strip out basic comments
        sTxt = ct::str::findAndReplace(sTxt, "//", "  ");
        sTxt = ct::str::findAndReplace(sTxt, "#", " ");
        sTxt = ct::str::findAndReplace(sTxt, "\t", " ");
        
        sTxt = ct::str::findAndReplaceRecursive(sTxt, " " + separator, separator);
        sTxt = ct::str::findAndReplaceRecursive(sTxt, separator + " ", separator);

        return ct::str::catchInRange(sTxt, "\n" + key + "=", "\n");
    }

    std::vector<std::string> getFiles(const std::string& directory, const std::string& prefix, const std::string& suffix) {
        std::vector<std::string> files;
        
#if defined(_WIN32)
        WIN32_FIND_DATAA fdFile;
        HANDLE hFind = NULL;
        std::string sPath = directory + "\\*";

        if ((hFind = FindFirstFileA(sPath.c_str(), &fdFile)) != INVALID_HANDLE_VALUE) {
            do {
                if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
                    if (!(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                        std::string filename = fdFile.cFileName;
                        bool pTest = prefix.empty() || filename.find(prefix) == 0;
                        bool sTest = suffix.empty() || (filename.length() >= suffix.length() && filename.compare(filename.length() - suffix.length(), suffix.length(), suffix) == 0);
                        if (pTest && sTest) files.push_back(filename);
                    }
                }
            } while (FindNextFileA(hFind, &fdFile));
            FindClose(hFind);
        }
#else
        DIR* dir = opendir(directory.c_str());
        if (dir != nullptr) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    std::string filename = entry->d_name;
                    bool pTest = prefix.empty() || filename.find(prefix) == 0;
                    bool sTest = suffix.empty() || (filename.length() >= suffix.length() && filename.compare(filename.length() - suffix.length(), suffix.length(), suffix) == 0);
                    if (pTest && sTest) files.push_back(filename);
                }
            }
            closedir(dir);
        }
#endif
        return files;
    }
}

} // namespace ct