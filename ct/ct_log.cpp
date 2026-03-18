#include "ct_log.hpp"
#include "ct_time.hpp"

namespace ct {
    namespace log {
        void message(Level msgLevel, const std::string& msg, std::ostream& stream, Level currentLevel) {
            if (static_cast<int>(msgLevel) <= static_cast<int>(currentLevel)) {
                stream << "[" << ct::time::getCurrentTimestamp() << "] ";
                switch (msgLevel) {
                    case Level::Error:   stream << "[ERROR] "; break;
                    case Level::Warning: stream << "[WARNING] "; break;
                    case Level::Info:    stream << "[INFO] "; break;
                    case Level::Debug:   stream << "[DEBUG] "; break;
                    default:             break;
                }
                stream << msg << std::endl;
            }
        }

        void toCout(Level msgLevel, const std::string& msg, Level currentLevel) { message(msgLevel, msg, std::cout, currentLevel); }
        void toCerr(Level msgLevel, const std::string& msg, Level currentLevel) { message(msgLevel, msg, std::cerr, currentLevel); }
    }    // namespace log
}    // namespace ct