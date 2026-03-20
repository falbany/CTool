/**
 * @file ct_csv.hpp
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

namespace ct {
    // ------------------------------------------------------------------------
    // CSV API
    // ------------------------------------------------------------------------

    /**
     * @namespace ct::csv
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
        class Writer {
          public:
            /**
             * @brief Construct a new Writer object.
             * @param filename Path to the output file.
             * @param delimiter Character used to separate values (default: ',').
             * @param append If true, appends to existing file. If false, overwrites.
             */
            explicit Writer(const std::string& filename, char delimiter = ',', bool append = false);
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
             * @tparam T Types must be convertible to string via ct::str::toString.
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
            bool          m_headerWritten;
            std::mutex    m_mutex;
            std::string   m_filename;
        };

        /**
         * @brief Reads a CSV file and returns the content as a 2D vector of strings.
         * @param filename Path to the file.
         * @param delimiter Separation character.
         * @param skipHeader If true, the first line is ignored.
         * @return std::vector<std::vector<std::string>> The parsed data.
         */
        std::vector<std::vector<std::string>> read(const std::string& filename, char delimiter = ',', bool skipHeader = true);

    }    // namespace csv
}    // namespace ct

    #include "ct_csv.tpp"
#endif    // CT_CSV_HPP