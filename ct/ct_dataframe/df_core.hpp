/**
 * @file df_core.hpp
 * @author Florent ALBANY (FAL)
 * @brief Main DataFrame class definition for heterogeneous data analysis.
 * @version 1.0
 * @date 2026-03-24
 */

#pragma once

#ifndef CT_DF_CORE_HPP
    #define CT_DF_CORE_HPP

    #include "df_column.hpp"
    #include "../ct_array2d.hpp"
    #include "../ct_num.hpp"
    #include <vector>
    #include <string>
    #include "../../libctool.h"

namespace ct {
    // ------------------------------------------------------------------------
    // DATA API
    // ------------------------------------------------------------------------

    /**
     * @namespace ct::data
     * @brief Data Analysis Tool - Lightweight DataFrame implementation.
     * 
     * This namespace provides a C++ equivalent to Python's Pandas DataFrame,
     * designed for handling heterogeneous tabular data (mix of double, int, string).
     * It allows for row/column construction, filtering, and basic data inspection
     * without external dependencies.
     * 
     * @note Data is stored column-oriented for better cache locality during column-wise operations.
     */
    namespace data {

        /**
         * @brief A lightweight, heterogeneous 2D data container (DataFrame).
         * 
         * The DataFrame class stores data in a column-major format, where each column
         * can hold a different data type (double, int, string). This mimics the behavior
         * of Pandas DataFrames, allowing for mixed-type columns.
         * 
         * @note Internally, the DataFrame store `Cell` values,
         * ensuring type safety while maintaining flexibility.
         * 
         * @warning All rows must have the same number of elements. Pushing a row with
         * mismatched column counts will throw an exception.
         * 
         * @example
         * \code{.cpp}
         * ct::data::DataFrame df;
         * df.addColumn("Name");
         * df.addColumn("Age");
         * df.pushRow({"Alice", 30});
         * df.pushRow({"Bob", 25});
         * 
         * const ct::data::Cell& name = df.at(0, "Name");
         * std::cout << std::get<std::string>(name) << "\n";
         * \endcode
         */
        class LIBCTOOL_API DataFrame {
          public:
            /**
             * @brief Constructs an empty DataFrame.
             */
            DataFrame() = default;

            // --- Construction ---

            /**
             * @brief Adds a new column to the DataFrame.
             * 
             * The column is created with a given name. No data is added yet.
             * Existing rows are not expanded; new data is added via `pushRow`.
             * 
             * @param name The unique identifier for the column.
             * @note Column names are case-sensitive and must be unique.
             * @note If a column with the same name already exists, it is replaced.
             */
            void addColumn(const std::string& name);

            /**
             * @brief Appends a new row of data to the DataFrame.
             * 
             * The size of the row must match the number of columns currently defined.
             * If the number of columns is zero, a new row creates columns implicitly
             * based on the provided vector size.
             * 
             * @param row A vector of `Cell` values, one for each column.
             * @throws std::runtime_error If the row size does not match the number of columns
             *         (unless the DataFrame is empty).
             * @note This operation is $O(C)$ where $C$ is the number of columns.
             */
            void pushRow(const std::vector<Cell>& row);

            // --- Lifecycle ---

            /**
             * @brief Clears all data and removes all columns.
             * 
             * Resets the DataFrame to an empty state.
             */
            void clear();

            // --- Dimensions ---

            /**
             * @brief Returns the total number of rows in the DataFrame.
             * @return size_t The number of rows.
             * @return 0 if the DataFrame is empty.
             */
            size_t rows() const;

            /**
             * @brief Returns the total number of columns in the DataFrame.
             * @return size_t The number of columns.
             */
            size_t cols() const;

            /**
             * @brief Returns a list of all column names in order of creation.
             * @return std::vector<std::string> Vector of column names.
             */
            std::vector<std::string> columnNames() const;

            // --- Access ---

            /**
             * @brief Accesses a specific cell by row index and column name.
             * 
             * Returns a constant reference to the value. Throws if the row
             * index or column name is invalid.
             * 
             * @param row The zero-based row index.
             * @param colName The name of the column.
             * @return const Cell& Reference to the cell value.
             * @throws std::out_of_range If `row` is out of bounds.
             * @throws std::runtime_error If `colName` is not found.
             */
            const Cell& at(size_t row, const std::string& colName) const;

            // --- Slicing & Inspection ---

            /**
             * @brief Returns a new DataFrame containing the first `n` rows.
             * 
             * Useful for inspecting the beginning of large datasets.
             * 
             * @param n Number of rows to include. Defaults to 5.
             * @return DataFrame A new DataFrame containing the first `n` rows (or all if n > rows).
             * @note The returned DataFrame is a shallow copy of the structure but deep copy of data.
             */
            DataFrame head(size_t n = 5) const;

            /**
             * @brief Returns a copy of the data in a specific row.
             * 
             * @param index The zero-based row index.
             * @return std::vector<Cell> A vector containing the values of the row.
             * @throws std::out_of_range If `index` is out of bounds.
             */
            std::vector<Cell> getRow(size_t index) const;

            /**
             * @brief Returns a copy of the data in a specific column.
             * 
             * @param name The name of the column.
             * @return std::vector<Cell> A vector containing the values of the column.
             * @throws std::runtime_error If `name` is not found.
             */
            std::vector<Cell> getColumn(const std::string& name) const;

            // --- I/O & Conversion Methods ---
            // Note: Implementations are in df_io.tpp for separation of concerns.

            /**
             * @brief Exports the DataFrame to a CSV file.
             * @param filename Destination path.
             * @param delim Column separator (default: ',').
             * @return true if successful, false if an error occurred.
             * @note Throws std::runtime_error on I/O failure if strict mode was enabled.
             */
            bool toCSV(const std::string& filename, char delim = ',');

            /**
             * @brief Exports numeric columns to a legacy Array2D<double>.
             * @param columnNames List of numeric columns to extract.
             * @return Array2D<double> A matrix containing the selected data.
             * @throws std::runtime_error if a column is non-numeric or names are invalid.
             * @note Returns an empty Array2D if columnNames is empty.
             */
            ct::array::Array2D<double> toArray2D(const std::vector<std::string>& columnNames) const;

            /**
             * @brief Exports numeric columns to a high-performance NumArray<double>.
             * @param columnNames List of numeric columns to extract.
             * @return NumArray<double> A contiguous matrix containing the selected data.
             * @throws std::runtime_error if a column is non-numeric or names are invalid.
             * @note Uses contiguous memory for faster downstream math operations.
             */
            ct::num::NumArray<double> toNumArray(const std::vector<std::string>& columnNames) const;

          private:
            /**
             * @brief Internal helper to find the index of a column by name.
             * @param name The column name to search for.
             * @return int The zero-based index, or -1 if not found.
             */
            int findIndex(const std::string& name) const;

            std::vector<Column> m_columns;
        };

    }    // namespace data
}    // namespace ct

    #include "df_core.tpp"

#endif    // CT_DF_CORE_HPP