/**
 * @file ct_array3d.hpp
 * @author Florent ALBANY - FAL
 * @brief A type-safe, dynamic 3D array (tensor) container.
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#ifndef CT_ARRAY3D_HPP
    #define CT_ARRAY3D_HPP

    #include <vector>
    #include <stdexcept>
    #include <ostream>
    #include "ct_array2d.hpp"
    #include "../libctool.h"

namespace ct {
    // ------------------------------------------------------------------------
    // ARRAY3D API
    // ------------------------------------------------------------------------

    /**
     * @namespace ct::array
     * @brief Array3D Tool - 3D dynamic array (tensor) manipulation.
     */
    namespace array {

        /**
         * @brief A generic 3D array container with dimension checking and bounds safety.
         * @tparam T The data type stored in the array (e.g., double, int).
         * @note Dimensions are ordered as [depth][rows][cols].
         */
        template <typename T>
        class Array3D {
        public:
            /**
             * @brief Constructs an empty array.
             */
            Array3D();

            /**
             * @brief Constructs a 3D array with specified dimensions.
             * @param depth Number of slices (depth).
             * @param rows Number of rows per slice.
             * @param cols Number of columns per slice.
             * @param init_value Value to initialize elements with (default: default constructed T).
             * @throws std::invalid_argument if dimensions are negative.
             */
            Array3D(size_t depth, size_t rows, size_t cols, const T& init_value = T());

            /**
             * @brief Creates a 3D array from a nested vector of vectors of vectors.
             * @param data The source 3D data.
             * @throws std::invalid_argument if dimensions are inconsistent (non-rectangular).
             */
            Array3D(const std::vector<std::vector<std::vector<T>>>& data);

            // --- Copy/Move Semantics ---
            Array3D(const Array3D&) = default;
            Array3D& operator=(const Array3D&) = default;
            Array3D(Array3D&&) noexcept = default;
            Array3D& operator=(Array3D&&) noexcept = default;

            // --- Dimensions ---
            /** @brief Returns the depth (number of slices). */
            size_t depth() const noexcept;
            /** @brief Returns the number of rows per slice. */
            size_t rows() const noexcept;
            /** @brief Returns the number of columns per slice. */
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
             * @param d Slice (depth) index.
             * @param r Row index.
             * @param c Column index.
             * @return Reference to the element.
             * @throws std::out_of_range if indices are invalid.
             */
            T& at(size_t d, size_t r, size_t c);
            const T& at(size_t d, size_t r, size_t c) const;

            /**
             * @brief Unchecked access (no bounds checking, faster).
             * @param d Slice (depth) index.
             * @param r Row index.
             * @param c Column index.
             * @return Reference to the element.
             * @note Only use if bounds are guaranteed.
             */
            T& operator()(size_t d, size_t r, size_t c);
            const T& operator()(size_t d, size_t r, size_t c) const;

            // --- Operations ---
            /**
             * @brief Fills the entire array with a value.
             * @param val Value to set.
             */
            void fill(const T& val);

            /**
             * @brief Resizes the array.
             * @param depth New depth.
             * @param rows New rows per slice.
             * @param cols New cols per slice.
             * @param val Value for new elements.
             */
            void resize(size_t depth, size_t rows, size_t cols, const T& val = T());

            // --- Math / Transformation ---
            /**
             * @brief Scales all elements by a factor.
             * @param factor The multiplier.
             * @return Reference to this array.
             */
            Array3D& scale(double factor);

            /**
             * @brief Adds a constant value to all elements.
             * @param val The value to add.
             * @return Reference to this array.
             */
            Array3D& add(double val);

            // --- Utility ---
            /**
             * @brief Returns a copy of the underlying nested vectors.
             * @return std::vector<std::vector<std::vector<T>>>
             */
            std::vector<std::vector<std::vector<T>>> toVector() const;

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

            /**
             * @brief Extracts a specific slice (2D layer) as a ct::array::Array.
             * @param d Slice index (0 to depth-1).
             * @return ct::array::Array2D<T> The 2D slice.
             */
            ct::array::Array2D<T> slice(size_t d) const;

        private:
            std::vector<std::vector<std::vector<T>>> m_data;
        };

        /**
         * @brief Stream output operator for Array3D.
         * @param os Output stream.
         * @param arr Array3D to print.
         * @return Output stream.
         */
        template <typename T>
        std::ostream& operator<<(std::ostream& os, const Array3D<T>& arr);

    }    // namespace array
}    // namespace ct

    #include "ct_array3d.tpp"

#endif    // CT_ARRAY3D_HPP