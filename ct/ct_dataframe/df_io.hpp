/**
 * @file df_io.hpp
 * @author Florent ALBANY (FAL)
 * @brief I/O utilities for DataFrame (CSV, Console).
 */

#ifndef CT_DF_IO_HPP
#define CT_DF_IO_HPP

#include "df_core.hpp"
#include "../ct_csv.hpp"
#include "../ct_array2d.hpp"
#include "../ct_num.hpp"
#include <iostream>

namespace ct {
    namespace data {

        /**
         * @brief Exports the DataFrame to a CSV file.
         * @param df The source DataFrame.
         * @param filename Destination path.
         * @param delim Column separator (default: ',').
         * @return true if successful.
         */
        bool toCSV(const DataFrame& df, const std::string& filename, char delim = ',');

        /**
         * @brief Loads a CSV file into a DataFrame.
         * @param filename Path to the CSV file.
         * @param delim Column separator.
         * @param hasHeader If true, uses the first line for column names.
         * @return DataFrame populated with data.
         */
        DataFrame fromCSV(const std::string& filename, char delim = ',', bool hasHeader = true);

        /**
         * @brief Creates a DataFrame from a ct::array::Array2D<double>.
         * @param array The source numeric matrix.
         * @param columnNames Optional list of names. If empty, default names "col0", "col1"... are used.
         * @return DataFrame containing the matrix data as numeric Cells.
         * @throws std::invalid_argument if columnNames size doesn't match array width.
         */
        DataFrame fromArray2D(const ct::array::Array2D<double>& array, const std::vector<std::string>& columnNames = std::vector<std::string>());

        /**
         * @brief Exports numeric columns to a ct::array::Array2D<double>.
         * @param df The source DataFrame.
         * @param columnNames List of numeric columns to extract.
         * @return Array2D<double> A matrix containing the selected data.
         * @throws std::runtime_error if a column is non-numeric.
         */
        ct::array::Array2D<double> toArray2D(const DataFrame& df, const std::vector<std::string>& columnNames);

        /**
         * @brief Creates a DataFrame from a ct::num::NumArray<double>.
         * @param array The source numeric matrix (contiguous memory).
         * @param columnNames Optional list of names. If empty, default names "col0", "col1"... are used.
         * @return DataFrame containing the matrix data as numeric Cells.
         * @throws std::invalid_argument if columnNames size doesn't match array width.
         * @note Optimize for contiguous memory access patterns.
         */
        DataFrame fromNumArray(const ct::num::NumArray<double>& array, const std::vector<std::string>& columnNames = std::vector<std::string>());

        /**
         * @brief Exports numeric columns to a ct::num::NumArray<double>.
         * @param df The source DataFrame.
         * @param columnNames List of numeric columns to extract.
         * @return NumArray<double> A contiguous matrix containing the selected data.
         * @throws std::runtime_error if a column is non-numeric or data is inconsistent.
         * @note Returns a NumArray for high-performance downstream math operations.
         */
        ct::num::NumArray<double> toNumArray(const DataFrame& df, const std::vector<std::string>& columnNames);

    }    // namespace data
}    // namespace ct

/**
 * @brief Pretty-prints the DataFrame to a stream.
 */
std::ostream& operator<<(std::ostream& os, const ct::data::DataFrame& df);

#include "df_io.tpp"

#endif    // CT_DF_IO_HPP
