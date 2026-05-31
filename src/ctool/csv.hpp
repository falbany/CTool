/**
 * @file csv.hpp
 * @author Florent ALBANY - FAL
 * @brief Thread-safe CSV streaming and parsing for test data logging.
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#ifndef CT_CSV_HPP
    #define CT_CSV_HPP

    #include <string>
    #include <vector>
    #include <fstream>
    #include <mutex>
    #include "../internal/libctool.h"

namespace ctool {
    // ------------------------------------------------------------------------
    // CSV API
    // ------------------------------------------------------------------------

    /**
     * @namespace ctool::csv
     * @brief CSV tools - Write and read csv files using 2D vectors.
     */
    namespace csv {

        /**
         * @class Writer
         * @brief A streaming CSV writer designed for real-time data logging.
         * 
         * Handles automatic header management and ensures thread-safe 
         * writing to the filesystem.
         */
        class LIBCTOOL_API Writer {
          public:
            /**
             * @brief Construct a new Writer object.
             * @param filename Path to the output file.
             * @param delimiter Character used to separate values (default: ',').
             * @param quote Character used to quote values containing the delimiter (default: '"').
             * @param append If true, appends to existing file. If false, overwrites.
             */
            explicit Writer(const std::string& filename, char delimiter = ',', char quote = '"', bool append = false);
            ~Writer();

            /**
             * @brief Sets the header for the CSV file. 
             * 
             * This should be called before any data is written. If the file is 
             * being overwritten, the header is written immediately.
             * 
             * @param columns Vector of column names.
             */
            void setHeader(const std::vector<std::string>& columns);

            /**
             * @brief Writes a single row of data to the file.
             * @tparam T Types must be convertible to string via ctool::str::toString.
             * @param values Vector of values for the row.
             */
            template <typename T>
            void writeRow(const std::vector<T>& values);

            /**
             * @brief Manually flushes the buffer to disk.
             */
            void flush();

          private:
            std::ofstream m_file;
            char          m_delimiter;
            char          m_quote;
            bool          m_headerWritten;
            std::mutex    m_mutex;
            std::string   m_filename;
        };

        /**
         * @brief Reads a CSV file and returns the content as a 2D vector of strings.
         * @param filename Path to the file.
         * @param delimiter Separation character.
         * @param quote Quote character.
         * @param skipHeader If true, the first line is ignored.
         * @return std::vector<std::vector<std::string>> The parsed data.
         */
        LIBCTOOL_API std::vector<std::vector<std::string>> read(const std::string& filename, char delimiter = ',', char quote = '"', bool skipHeader = true);

        /**
         * @brief Writes a 2D vector of data to a CSV file.
         * @param filename Path to the file.
         * @param data 2D vector of strings representing the grid of data to write.
         * @param delimiter Separation character (default: ',').
         * @param quote Quote character (default: '"').
         * @return bool True on success, false on failure.
         */
        LIBCTOOL_API bool writeAll(const std::string& filename, const std::vector<std::vector<std::string>>& data, char delimiter = ',', char quote = '"');

        /**
         * @brief Extracts a single column from a parsed CSV (2D vector).
         * @param data The 2D vector resulting from read().
         * @param colIndex The zero-based index of the column to extract.
         * @return std::vector<std::string> The column data. Returns empty vector if index is out of bounds for all rows.
         */
        LIBCTOOL_API std::vector<std::string> getColumn(const std::vector<std::vector<std::string>>& data, size_t colIndex);

        /**
         * @brief Extracts a single column from a parsed CSV matching a header name.
         * @param filename Path to the file.
         * @param columnName The exact string matching the header column to extract.
         * @param delimiter Separation character (default: ',').
         * @param quote Quote character (default: '"').
         * @return std::vector<std::string> The column data (excluding the header). Returns empty vector if header not found.
         */
        LIBCTOOL_API std::vector<std::string> getColumnByName(const std::string& filename, const std::string& columnName, char delimiter = ',', char quote = '"');

        /**
         * @brief Extracts a single row from a parsed CSV (2D vector).
         * @param data The 2D vector resulting from read().
         * @param rowIndex The zero-based index of the row to extract.
         * @return std::vector<std::string> The row data. Returns empty vector if index is out of bounds.
         */
        LIBCTOOL_API std::vector<std::string> getRow(const std::vector<std::vector<std::string>>& data, size_t rowIndex);

        /**
         * @brief Extracts a single row from a parsed CSV matching a value in a specific column.
         * @param filename Path to the file.
         * @param rowName The string to match against the key column.
         * @param keyColIndex The index of the column to search in (default: 0).
         * @param delimiter Separation character (default: ',').
         * @param quote Quote character (default: '"').
         * @return std::vector<std::string> The row data. Returns empty vector if not found.
         */
        LIBCTOOL_API std::vector<std::string> getRowByName(const std::string& filename, const std::string& rowName, size_t keyColIndex = 0, char delimiter = ',',
                                                           char quote = '"');

    }    // namespace csv
}    // namespace ctool

    #include "csv.tpp"
#endif    // CT_CSV_HPP