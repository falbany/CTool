/**
 * @file ct_num.hpp
 * @author Florent ALBANY - FAL
 * @brief High-performance NumPy-like N-dimensional array container
 * @version 1.0
 * @date 2026-03-18
 *
 * @details
 * Provides a contiguous memory block for data storage, mimicking the numpy.ndarray behavior.
 * Supports element-wise arithmetic operators and broadcasting logic (for scalar and matching shapes).
 *
 * @note Memory ownership: NumArray owns the underlying std::vector.
 */

#pragma once

#ifndef CT_NUM_HPP
    #define CT_NUM_HPP

    #include <vector>
    #include <utility>
    #include <stdexcept>
    #include <ostream>
    #include <cmath>
    #include <algorithm>
    #include <limits>
    #include "../libctool.h"

namespace ct {
    // ------------------------------------------------------------------------
    // NUM API
    // ------------------------------------------------------------------------

    /**
     * @namespace ct::num
     * @brief Numerical Tool - NumPy-like array operations.
     */
    namespace num {

        /**
         * @brief A contiguous 2D array container with NumPy-like API.
         * @tparam T The data type stored (e.g., double, int).
         */
        template <typename T>
        class NumArray {
          public:
            // --- Construction ---
            /** @brief Constructs an empty array. */
            NumArray();

            /**
             * @brief Constructs an array of zeros.
             * @param rows Number of rows.
             * @param cols Number of columns.
             */
            NumArray(size_t rows, size_t cols);

            /**
             * @brief Constructs from nested initializer lists (e.g., {{1, 2}, {3, 4}}).
             * @param list Initializer list.
             * @note Throws if row lengths are inconsistent (jagged).
             */
            NumArray(const std::initializer_list<std::initializer_list<T>>& list);

            // --- Size & Shape ---
            /** @brief Returns number of rows (Shape[0]). */
            size_t rows() const noexcept;
            /** @brief Returns number of columns (Shape[1]). */
            size_t cols() const noexcept;
            /** @brief Returns total elements (Shape[0] * Shape[1]). */
            size_t size() const noexcept;
            /** @brief Returns true if the array contains no elements. */
            bool empty() const noexcept;
            /** @brief Resets the array to zero size. */
            void clear();
            /** @brief Returns {rows, cols}. */
            std::pair<size_t, size_t> shape() const noexcept;

            // --- Element Access ---
            /**
             * @brief Element access with bounds checking.
             * @exception std::out_of_range if indices are invalid.
             */
            T&       at(size_t r, size_t c);
            const T& at(size_t r, size_t c) const;

            /** @brief Unchecked element access (faster). */
            T&       operator()(size_t r, size_t c);
            const T& operator()(size_t r, size_t c) const;

            /** @brief Raw pointer access to contiguous data (interoperability). */
            T*       data() noexcept;
            const T* data() const noexcept;

            // --- Copy/Move ---
            NumArray(const NumArray&)                = default;
            NumArray& operator=(const NumArray&)     = default;
            NumArray(NumArray&&) noexcept            = default;
            NumArray& operator=(NumArray&&) noexcept = default;

          private:
            std::vector<T> m_data;
            size_t         m_rows;
            size_t         m_cols;

            // Helper to calculate 1D index
            inline size_t index(size_t r, size_t c) const { return r * m_cols + c; }
        };

        // ------------------------------------------------------------------------
        // ARITHMETIC OPERATORS (Element-wise)
        // ------------------------------------------------------------------------

        /** @brief Element-wise addition: Array + Array */
        template <typename T>
        NumArray<T> operator+(const NumArray<T>& lhs, const NumArray<T>& rhs);

        /** @brief Element-wise addition: Array + Scalar */
        template <typename T>
        NumArray<T> operator+(const NumArray<T>& lhs, const T& scalar);

        /** @brief Element-wise addition: Scalar + Array */
        template <typename T>
        NumArray<T> operator+(const T& scalar, const NumArray<T>& rhs);

        /** @brief Element-wise subtraction: Array - Array */
        template <typename T>
        NumArray<T> operator-(const NumArray<T>& lhs, const NumArray<T>& rhs);

        /** @brief Element-wise subtraction: Array - Scalar */
        template <typename T>
        NumArray<T> operator-(const NumArray<T>& lhs, const T& scalar);

        /** @brief Element-wise multiplication: Array * Array */
        template <typename T>
        NumArray<T> operator*(const NumArray<T>& lhs, const NumArray<T>& rhs);

        /** @brief Element-wise multiplication: Array * Scalar */
        template <typename T>
        NumArray<T> operator*(const NumArray<T>& lhs, const T& scalar);

        /** @brief Element-wise division: Array / Scalar */
        template <typename T>
        NumArray<T> operator/(const NumArray<T>& lhs, const T& scalar);

        // ------------------------------------------------------------------------
        // UNIVERSAL FUNCTIONS (Ufuncs)
        // ------------------------------------------------------------------------

        /**
         * @brief Computes the square root of every element.
         * @note Equivalent to numpy.sqrt()
         */
        template <typename T>
        NumArray<double> sqrt(const NumArray<T>& arr);

        /**
         * @brief Computes the sine of every element.
         * @note Equivalent to numpy.sin()
         */
        template <typename T>
        NumArray<double> sin(const NumArray<T>& arr);

        /**
         * @brief Computes the absolute value of every element.
         */
        template <typename T>
        NumArray<T> abs(const NumArray<T>& arr);

        // ------------------------------------------------------------------------
        // REDUCTION OPERATIONS
        // ------------------------------------------------------------------------

        /** @brief Sum of all elements. */
        template <typename T>
        double sum(const NumArray<T>& arr);

        /** @brief Mean (average) of all elements. */
        template <typename T>
        double mean(const NumArray<T>& arr);

        /** @brief Maximum value in the array. */
        template <typename T>
        T max(const NumArray<T>& arr);

        /** @brief Minimum value in the array. */
        template <typename T>
        T min(const NumArray<T>& arr);

        /** @brief Stream output (Matrix style). */
        template <typename T>
        std::ostream& operator<<(std::ostream& os, const NumArray<T>& arr);

    }    // namespace num
}    // namespace ct

    #include "ct_num.tpp"

#endif    // CT_NUM_HPP