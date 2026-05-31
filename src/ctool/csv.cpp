#include "csv.hpp"
#include "string.hpp"

namespace ctool {
    namespace csv {

        // Helper function to escape and quote a CSV field
        static std::string escapeAndQuote(const std::string& field, char delimiter, char quote) {
            bool needsQuotes = field.find(delimiter) != std::string::npos || field.find(quote) != std::string::npos || field.find('\n') != std::string::npos
                               || field.find('\r') != std::string::npos;

            if (!needsQuotes) {
                return field;
            }

            std::string escaped;
            escaped += quote;
            for (char c : field) {
                if (c == quote) {
                    escaped += quote;    // Double the quote character to escape it
                }
                escaped += c;
            }
            escaped += quote;
            return escaped;
        }

        Writer::Writer(const std::string& filename, char delimiter, char quote, bool append)
            : m_delimiter(delimiter), m_quote(quote), m_headerWritten(append), m_filename(filename) {
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
                m_file << escapeAndQuote(columns[i], m_delimiter, m_quote);
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

        std::vector<std::vector<std::string>> read(const std::string& filename, char delimiter, char quote, bool skipHeader) {
            std::vector<std::vector<std::string>> data;
            std::ifstream                         file(filename);
            if (!file.is_open()) return data;

            std::string line;
            bool        firstLine = true;

            // To handle multiline fields, we build raw content
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            if (content.empty()) return data;

            size_t                   pos = 0;
            size_t                   len = content.length();
            std::vector<std::string> currentRow;
            std::string              currentField;
            bool                     inQuotes = false;

            while (pos < len) {
                char c = content[pos];

                if (inQuotes) {
                    if (c == quote) {
                        if (pos + 1 < len && content[pos + 1] == quote) {
                            currentField += quote;
                            pos++;    // Skip the escaped quote
                        } else {
                            inQuotes = false;
                        }
                    } else {
                        currentField += c;
                    }
                } else {
                    if (c == quote) {
                        inQuotes = true;
                    } else if (c == delimiter) {
                        currentRow.push_back(currentField);
                        currentField.clear();
                    } else if (c == '\n') {
                        // Avoid adding an empty row at the very end
                        if (!(currentRow.empty() && currentField.empty() && pos == len - 1)) {
                            currentRow.push_back(currentField);

                            if (!(firstLine && skipHeader)) {
                                data.push_back(currentRow);
                            }
                            firstLine = false;
                            currentRow.clear();
                            currentField.clear();
                        }
                    } else if (c == '\r') {
                        // ignore carriage return if it precedes newline
                    } else {
                        currentField += c;
                    }
                }
                pos++;
            }

            if (!currentRow.empty() || !currentField.empty()) {
                currentRow.push_back(currentField);
                if (!(firstLine && skipHeader)) {
                    data.push_back(currentRow);
                }
            }

            return data;
        }

        bool writeAll(const std::string& filename, const std::vector<std::vector<std::string>>& data, char delimiter, char quote) {
            Writer writer(filename, delimiter, quote, false);
            if (data.empty()) return true;    // Handled as success (opened and closed file)

            for (const auto& row : data) {
                writer.writeRow(row);
            }
            return true;
        }

        std::vector<std::string> getColumn(const std::vector<std::vector<std::string>>& data, size_t colIndex) {
            std::vector<std::string> col;
            col.reserve(data.size());
            for (const auto& row : data) {
                if (colIndex < row.size()) {
                    col.push_back(row[colIndex]);
                } else {
                    col.push_back("");    // Pad if row is short
                }
            }
            return col;
        }

        std::vector<std::string> getColumnByName(const std::string& filename, const std::string& columnName, char delimiter, char quote) {
            std::vector<std::string> col;
            auto                     data = read(filename, delimiter, quote, false);    // Read with headers

            if (data.empty()) return col;

            // Find the index of the column
            size_t      colIndex = std::string::npos;
            const auto& header   = data[0];
            for (size_t i = 0; i < header.size(); ++i) {
                if (header[i] == columnName) {
                    colIndex = i;
                    break;
                }
            }

            if (colIndex == std::string::npos) {
                return col;    // Header not found
            }

            // Extract the column, skipping the first row (header)
            col.reserve(data.size() - 1);
            for (size_t i = 1; i < data.size(); ++i) {
                if (colIndex < data[i].size()) {
                    col.push_back(data[i][colIndex]);
                } else {
                    col.push_back("");    // Pad if row is short
                }
            }

            return col;
        }

        std::vector<std::string> getRow(const std::vector<std::vector<std::string>>& data, size_t rowIndex) {
            if (rowIndex < data.size()) {
                return data[rowIndex];
            }
            return {};
        }

        std::vector<std::string> getRowByName(const std::string& filename, const std::string& rowName, size_t keyColIndex, char delimiter, char quote) {
            auto data = read(filename, delimiter, quote, false);    // Read all

            for (const auto& row : data) {
                if (keyColIndex < row.size() && row[keyColIndex] == rowName) {
                    return row;
                }
            }

            return {};
        }

    }    // namespace csv
}    // namespace ctool