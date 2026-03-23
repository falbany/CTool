#include "ct_csv.hpp"
#include "ct_str.hpp"

namespace ct {
    namespace csv {

        Writer::Writer(const std::string& filename, char delimiter, bool append) : m_delimiter(delimiter), m_headerWritten(append), m_filename(filename) {
            auto mode = std::ios::out;
            if (append) mode |= std::ios::app;

            m_file.open(filename, mode);
        }

        Writer::~Writer() {
            if (m_file.is_open()) {
                m_file.close();
            }
        }

        void Writer::setHeader(const std::vector<std::string>& columns) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_headerWritten || !m_file.is_open()) return;

            for (size_t i = 0; i < columns.size(); ++i) {
                m_file << columns[i];
                if (i < columns.size() - 1) {
                    m_file << m_delimiter;
                }
            }
            m_file << "\n";
            m_headerWritten = true;
        }

        void Writer::flush() {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_file.flush();
        }

        std::vector<std::vector<std::string>> read(const std::string& filename, char delimiter, bool skipHeader) {
            std::vector<std::vector<std::string>> data;
            std::ifstream                         file(filename);
            if (!file.is_open()) return data;

            std::string line;
            bool        firstLine = true;

            while (std::getline(file, line)) {
                if (firstLine && skipHeader) {
                    firstLine = false;
                    continue;
                }
                if (line.empty()) continue;
                data.push_back(ct::str::split(line, delimiter));
            }

            return data;
        }

    }    // namespace csv
}    // namespace ct