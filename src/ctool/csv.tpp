#include "csv.hpp"
#include "string.hpp"

namespace ctool {
    namespace csv {

        template <typename T>
        void Writer::writeRow(const std::vector<T>& values) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!m_file.is_open()) return;

            for (size_t i = 0; i < values.size(); ++i) {
                // escapeAndQuote is declared only in csv.cpp, we could either move it to a shared header or duplicate its logic for templates.
                // Let's implement escaping inline to keep it self-contained.
                std::string field = ctool::str::toString(values[i]);
                bool needsQuotes = field.find(m_delimiter) != std::string::npos ||
                                   field.find(m_quote) != std::string::npos ||
                                   field.find('\n') != std::string::npos ||
                                   field.find('\r') != std::string::npos;
                
                if (needsQuotes) {
                    std::string escaped;
                    escaped += m_quote;
                    for (char c : field) {
                        if (c == m_quote) escaped += m_quote;
                        escaped += c;
                    }
                    escaped += m_quote;
                    m_file << escaped;
                } else {
                    m_file << field;
                }
                
                if (i < values.size() - 1) {
                    m_file << m_delimiter;
                }
            }
            m_file << "\n";
        }

    }    // namespace csv
}    // namespace ctool