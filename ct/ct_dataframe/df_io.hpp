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
         * @brief Exports numeric columns to a ct::array::Array2D<double>.
         * @param df The source DataFrame.
         * @param columnNames List of numeric columns to extract.
         * @return Array2D<double> A matrix containing the selected data.
         * @throws std::runtime_error if a column is non-numeric.
         */
        ct::array::Array2D<double> toArray2D(const DataFrame& df, const std::vector<std::string>& columnNames);

    }    // namespace data
}    // namespace ct

/**
 * @brief Pretty-prints the DataFrame to a stream.
 */
std::ostream& operator<<(std::ostream& os, const ct::data::DataFrame& df);

#include "df_io.tpp"

#endif    // CT_DF_IO_HPP