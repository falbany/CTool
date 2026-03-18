#include "ct_sys.hpp"
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <cstdlib>

#if defined(_WIN32)
    #include <windows.h>
    #include <direct.h>
    #include <process.h>
    #define getpid _getpid
    #define mkdir(path, mode) _mkdir(path)
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

namespace ct {
    namespace sys {

        std::string executeCommand(const std::string& command) {
            std::array<char, 128> buffer;
            std::string           result;

#if defined(_WIN32)
            std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
#else
            std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
#endif

            if (!pipe) {
                return "Error: Failed to open pipe.";
            }

            while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
                result += buffer.data();
            }
            return result;
        }

        std::string getEnv(const std::string& varName) {
            const char* val = std::getenv(varName.c_str());
            return val == nullptr ? std::string("") : std::string(val);
        }

        bool makeDirectory(const std::string& path) {
#if defined(_WIN32)
            int res = _mkdir(path.c_str());
#else
            int res = mkdir(path.c_str(), 0777);
#endif
            return (res == 0);
        }

        std::string getHostname() {
            char buffer[256];
#if defined(_WIN32)
            DWORD size = sizeof(buffer);
            if (GetComputerNameA(buffer, &size)) return std::string(buffer);
#else
            if (gethostname(buffer, sizeof(buffer)) == 0) return std::string(buffer);
#endif
            return "unknown_host";
        }

        int getProcessId() { return static_cast<int>(getpid()); }

        bool isLittleEndian() {
            volatile uint32_t i = 0x01234567;
            return (*reinterpret_cast<volatile uint8_t*>(&i) == 0x67);
        }

    }    // namespace sys
}    // namespace ct