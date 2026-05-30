#include "time.hpp"
#include "log.hpp"
#include "internal/platform.h"
#include <chrono>
#include <thread>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>

#if PLATFORM_WINDOWS
    #define getcwd _getcwd
#endif

namespace ctool {
    namespace time {
        std::string getCurrentTimestamp() {
            auto        now   = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            char        buf[20];
            std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
            return std::string(buf);
        }

        void sleepMs(long ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }
        void sleepS(long s) { std::this_thread::sleep_for(std::chrono::seconds(s)); }

        long long getUptimeMs() { return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(); }
        long long getUptimeNs() { return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(); }

        // Timer Implementation
        Timer::Timer(const std::string& label) : label_(label), start_(std::chrono::steady_clock::now()) { std::cout << "[Timer Start] " << label_ << std::endl; }

        Timer::~Timer() {
            auto end  = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
            std::cout << "[Timer End]   " << label_ << " took " << (static_cast<double>(diff.count()) / 1000.0) << " ms" << std::endl;
        }

    }    // namespace time
}    // namespace ctool