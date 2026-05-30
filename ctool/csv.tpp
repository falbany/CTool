#include "csv.hpp"
#include "string.hpp"

namespace ctool {
    namespace csv {

        template <typename T>
        void Writer::writeRow(const std::vector<T>& values) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!m_file.is_open()) return;

            for (size_t i = 0; i < values.size(); ++i) {
                m_file << ctool::str::toString(values[i]);
                if (i < values.size() - 1) {
                    m_file << m_delimiter;
                }
            }
            m_file << "\n";
        }

    }    // namespace csv
}    // namespace ctool