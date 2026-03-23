#include "ct_log.hpp"
#include <chrono>
#include <iomanip>
#include <mutex>
#include <ctime>
#include <sstream>

namespace ct {
    namespace log {

        static std::mutex log_mutex;

        static std::string getTimestamp() {
            auto              now       = std::chrono::system_clock::now();
            auto              in_time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
            return ss.str();
        }

        static std::string levelToString(Level level) {
            switch (level) {
                case Level::Error:   return "[ERROR]";
                case Level::Warning: return "[WARN ]";
                case Level::Info:    return "[INFO ]";
                case Level::Debug:   return "[DEBUG]";
                default:             return "[LOG  ]";
            }
        }

        void message(Level msgLevel, const std::string& msg, std::ostream& stream, Level currentLevel) {
            if (msgLevel > currentLevel || msgLevel == Level::Quiet) return;

            std::lock_guard<std::mutex> lock(log_mutex);
            stream << getTimestamp() << " " << levelToString(msgLevel) << " " << msg << std::endl;
        }

        void toCout(Level msgLevel, const std::string& msg, Level currentLevel) { message(msgLevel, msg, std::cout, currentLevel); }

        void toCerr(Level msgLevel, const std::string& msg, Level currentLevel) { message(msgLevel, msg, std::cerr, currentLevel); }

    }    // namespace log
}    // namespace ct