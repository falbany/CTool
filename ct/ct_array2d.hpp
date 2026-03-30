/**
 * @file ct_array.hpp
 * @author Florent ALBANY - FAL
 * @brief A type-safe, dynamic 2D array container.
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#ifndef CT_ARRAY_HPP
    #define CT_ARRAY_HPP

    #include <vector>
    #include <utility>
    #include <stdexcept>
    #include <string>
    #include <ostream>
    #include <cmath>
    #include "../libctool.h"

namespace ct {
    // ------------------------------------------------------------------------
    // ARRAY API
    // ------------------------------------------------------------------------

    /**
     * @namespace ct::array
     * @brief Array2D Tool - 2D dynamic array manipulation.
     */
    namespace array {

        /**
         * @brief A generic 2D array container with dimension checking and bounds safety.
         * @tparam T The data type stored in the array (e.g., double, int).
         */
        template <typename T>
        class Array2D {
          public:
            /**
             * @brief Constructs an empty array.
             */
            Array2D();

            /**
             * @brief Constructs a 2D array with specified dimensions.
             * @param rows Number of rows (height).
             * @param cols Number of columns (width).
             * @param init_value Value to initialize elements with (default: default constructed T).
             * @throws std::invalid_argument if rows or cols are negative.
             */
            Array2D(size_t rows, size_t cols, const T& init_value = T());

            /**
             * @brief Creates an array from a vector of vectors.
             * @param data The source 2D data.
             * @throws std::invalid_argument if rows are inconsistent (jagged array).
             */
            Array2D(const std::vector<std::vector<T>>& data);

            // --- Copy/Move Semantics ---
            Array2D(const Array2D&)                = default;
            Array2D& operator=(const Array2D&)     = default;
            Array2D(Array2D&&) noexcept            = default;
            Array2D& operator=(Array2D&&) noexcept = default;

            // --- Size & Dimensions ---
            /** @brief Returns the number of rows. */
            size_t rows() const noexcept;
            /** @brief Returns the number of columns. */
            size_t cols() const noexcept;
            /** @brief Returns the total number of elements. */
            size_t size() const noexcept;
            /** @brief Checks if the array is empty. */
            bool empty() const noexcept;
            /** @brief Resets the array to empty. */
            void clear();

            // --- Element Access ---
            /**
             * @brief Accesses an element with bounds checking.
             * @param r Row index.
             * @param c Column index.
             * @return Reference to the element.
             * @throws std::out_of_range if indices are invalid.
             */
            T&       at(size_t r, size_t c);
            const T& at(size_t r, size_t c) const;

            /**
             * @brief Unchecked access (no bounds checking, faster).
             * @param r Row index.
             * @param c Column index.
             * @return Reference to the element.
             * @note Only use if bounds are guaranteed.
             */
            T&       operator()(size_t r, size_t c);
            const T& operator()(size_t r, size_t c) const;

            // --- Operations ---
            /**
             * @brief Fills the entire array with a value.
             * @param val Value to set.
             */
            void fill(const T& val);

            /**
             * @brief Adds a new row with specified size.
             * @param cols Number of columns in the new row.
             * @param val Initial value for the row.
             */
            void pushRow(size_t cols, const T& val = T());

            /**
             * @brief Resizes the array.
             * @param rows New number of rows.
             * @param cols New number of columns.
             * @param val Value for new elements.
             * @note Existing elements are preserved. New elements are set to val.
             */
            void resize(size_t rows, size_t cols, const T& val = T());

            // --- Math / Transformation (Common operations) ---
            /**
             * @brief Scales all elements by a factor.
             * @param factor The multiplier.
             * @return Reference to this array for chaining.
             */
            Array2D& scale(double factor);

            /**
             * @brief Adds a constant value to all elements.
             * @param val The value to add.
             * @return Reference to this array.
             */
            Array2D& add(double val);

            // --- Utility ---
            /**
             * @brief Returns a copy of the underlying vector of vectors.
             * @return std::vector<std::vector<T>>
             */
            std::vector<std::vector<T>> toVector() const;

            /**
             * @brief Extracts a specific row as a 1D vector.
             * @param r Row index (0 to rows()-1).
             * @return std::vector<T> The row data.
             * @throws std::out_of_range if index is invalid.
             */
            std::vector<T> sliceRow(size_t r) const;

            /**
             * @brief Extracts a specific column as a 1D vector.
             * @param c Column index (0 to cols()-1).
             * @return std::vector<T> The column data.
             * @throws std::out_of_range if index is invalid.
             */
            std::vector<T> sliceColumn(size_t c) const;

            /**
             * @brief Extracts a sub-array (sub-matrix) from the current array.
             * @param r Top-left row index.
             * @param c Top-left column index.
             * @param rows Number of rows in the sub-array.
             * @param cols Number of columns in the sub-array.
             * @return Array2D<T> The extracted sub-array.
             * @throws std::out_of_range if the sub-array exceeds bounds.
             */
            Array2D<T> subArray(size_t r, size_t c, size_t rows, size_t cols) const;

            /**
             * @brief Finds the coordinates of the first occurrence of a value.
             * @param target The value to search for.
             * @return std::pair<size_t, size_t> (row, col) index.
             * @throws std::runtime_error if the value is not found.
             */
            std::pair<size_t, size_t> findValue(const T& target) const;

            /**
             * @brief Finds the coordinates of all occurrences of a value.
             * @param target The value to search for.
             * @return std::vector<std::pair<size_t, size_t>> List of (row, col) indices.
             * @return Empty vector if not found.
             */
            std::vector<std::pair<size_t, size_t>> findAllValues(const T& target) const;

            /**
             * @brief Finds the coordinates of the element closest to a target value.
             * @param target The target value to approach.
             * @return std::pair<size_t, size_t> (row, col) index of the nearest element.
             * @note Uses absolute difference for comparison.
             */
            std::pair<size_t, size_t> findNearest(const T& target) const;

            /**
             * @brief Creates a transposed copy of the array.
             * @note The original array is not modified.
             *       If the input is $M \times N$, the result is $N \times M$.
             *       Specifically, result(r, c) = original(c, r).
             * @return Array2D<T> The transposed matrix.
             */
            Array2D<T> transpose() const;
            
            /**
             * @brief Returns the maximum value in the array.
             * @return T Maximum element.
             * @throws std::runtime_error if array is empty.
             */
            T max() const;

            /**
             * @brief Returns the minimum value in the array.
             * @return T Minimum element.
             * @throws std::runtime_error if array is empty.
             */
            T min() const;

          private:
            std::vector<std::vector<T>> m_data;
        };

        /**
         * @brief Stream output operator for Array.
         * @param os Output stream.
         * @param arr Array2D to print.
         * @return Output stream.
         */
        template <typename T>
        std::ostream& operator<<(std::ostream& os, const Array2D<T>& arr);

    }    // namespace array
}    // namespace ct

    #include "ct_array2d.tpp"

#endif    // CT_ARRAY_HPP