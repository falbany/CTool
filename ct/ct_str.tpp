#include <sstream>

namespace ct {
    namespace str {
        template <typename T>
        std::string toString(T value) {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
    }    // namespace str
}    // namespace ct