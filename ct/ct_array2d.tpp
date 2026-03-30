/**
 * @file ct_array.tpp
 * @brief Template implementations for ct_array.hpp.
 * @author Florent ALBANY - FAL
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cmath>
#include <limits>
#include <utility>

namespace ct {
    namespace array {

        // --- Constructors ---
        template <typename T>
        Array2D<T>::Array2D() : m_data() {}

        template <typename T>
        Array2D<T>::Array2D(size_t rows, size_t cols, const T& init_value) {
            if (rows == 0 || cols == 0) {
                m_data.clear();
                return;
            }
            m_data.resize(rows, std::vector<T>(cols, init_value));
        }

        template <typename T>
        Array2D<T>::Array2D(const std::vector<std::vector<T>>& data) {
            if (data.empty()) {
                return;
            }

            size_t expected_cols = data[0].size();
            for (const auto& row : data) {
                if (row.size() != expected_cols) {
                    throw std::invalid_argument("ct::array: Input data must be a rectangular matrix (consistent row lengths).");
                }
            }
            m_data = data;
        }

        // --- Size & Dimensions ---
        template <typename T>
        size_t Array2D<T>::rows() const noexcept {
            return m_data.size();
        }

        template <typename T>
        size_t Array2D<T>::cols() const noexcept {
            return m_data.empty() ? 0 : m_data[0].size();
        }

        template <typename T>
        size_t Array2D<T>::size() const noexcept {
            return m_data.size() * cols();
        }

        template <typename T>
        bool Array2D<T>::empty() const noexcept {
            return m_data.empty() || m_data[0].empty();
        }

        template <typename T>
        void Array2D<T>::clear() {
            m_data.clear();
        }

        // --- Element Access ---
        template <typename T>
        T& Array2D<T>::at(size_t r, size_t c) {
            if (r >= m_data.size() || c >= m_data[0].size()) {
                throw std::out_of_range("ct::array::at: Index out of bounds.");
            }
            return m_data[r][c];
        }

        template <typename T>
        const T& Array2D<T>::at(size_t r, size_t c) const {
            if (r >= m_data.size() || c >= m_data[0].size()) {
                throw std::out_of_range("ct::array::at: Index out of bounds.");
            }
            return m_data[r][c];
        }

        template <typename T>
        T& Array2D<T>::operator()(size_t r, size_t c) {
            return m_data[r][c];
        }

        template <typename T>
        const T& Array2D<T>::operator()(size_t r, size_t c) const {
            return m_data[r][c];
        }

        // --- Operations ---
        template <typename T>
        void Array2D<T>::fill(const T& val) {
            for (auto& row : m_data) {
                row.assign(row.size(), val);
            }
        }

        template <typename T>
        void Array2D<T>::pushRow(size_t cols, const T& val) {
            m_data.emplace_back(cols, val);
        }

        template <typename T>
        void Array2D<T>::resize(size_t rows, size_t cols, const T& val) {
            if (rows == 0 && cols == 0) {
                m_data.clear();
                return;
            }

            if (rows > m_data.size()) {
                // Add new rows
                m_data.resize(rows, std::vector<T>(cols, val));
            } else {
                // Shrink or keep existing rows
                if (m_data.size() > rows) {
                    m_data.resize(rows);
                }
            }

            // Adjust columns for existing rows
            for (auto& row : m_data) {
                if (row.size() < cols) {
                    row.resize(cols, val);
                } else if (row.size() > cols) {
                    row.resize(cols);
                }
            }
        }

        // --- Math / Transformation ---
        template <typename T>
        Array2D<T>& Array2D<T>::scale(double factor) {
            for (auto& row : m_data) {
                for (auto& val : row) {
                    val = static_cast<T>(val * factor);
                }
            }
            return *this;
        }

        template <typename T>
        Array2D<T>& Array2D<T>::add(double val) {
            for (auto& row : m_data) {
                for (auto& element : row) {
                    element = static_cast<T>(element + val);
                }
            }
            return *this;
        }

        // --- Utility ---
        template <typename T>
        std::vector<std::vector<T>> Array2D<T>::toVector() const {
            return m_data;
        }

        template <typename T>
        std::vector<T> Array2D<T>::sliceRow(size_t r) const {
            if (r >= m_data.size()) {
                throw std::out_of_range("ct::array::sliceRow: Row index out of bounds.");
            }
            return m_data[r];    // Returns a copy of the row vector
        }

        template <typename T>
        std::vector<T> Array2D<T>::sliceColumn(size_t c) const {
            if (m_data.empty() || c >= m_data[0].size()) {
                throw std::out_of_range("ct::array::sliceColumn: Column index out of bounds.");
            }
            std::vector<T> col;
            col.reserve(m_data.size());
            for (const auto& row : m_data) {
                col.push_back(row[c]);
            }
            return col;
        }

        template <typename T>
        Array2D<T> Array2D<T>::subArray(size_t r, size_t c, size_t rows, size_t cols) const {
            if (m_data.empty()) {
                return Array2D<T>(0, 0);
            }

            // Validate starting position
            if (r >= m_data.size() || c >= m_data[0].size()) {
                throw std::out_of_range("ct::array::subArray: Top-left index out of bounds.");
            }

            // Validate dimensions
            if (r + rows > m_data.size() || c + cols > m_data[0].size()) {
                throw std::out_of_range("ct::array::subArray: Requested sub-array dimensions exceed bounds.");
            }

            Array2D<T> result(rows, cols);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result(i, j) = m_data[r + i][c + j];
                }
            }
            return result;
        }

        // --- Find Functionality ---

        template <typename T>
        std::pair<size_t, size_t> Array2D<T>::findValue(const T& target) const {
            if (empty()) {
                throw std::runtime_error("ct::array::findValue: Cannot search an empty array.");
            }

            for (size_t i = 0; i < m_data.size(); ++i) {
                for (size_t j = 0; j < m_data[i].size(); ++j) {
                    if (m_data[i][j] == target) {
                        return std::make_pair(i, j);
                    }
                }
            }
            throw std::runtime_error("ct::array::findValue: Value not found in array.");
        }

        template <typename T>
        std::vector<std::pair<size_t, size_t>> Array2D<T>::findAllValues(const T& target) const {
            std::vector<std::pair<size_t, size_t>> indices;
            if (empty()) {
                return indices;
            }

            for (size_t i = 0; i < m_data.size(); ++i) {
                for (size_t j = 0; j < m_data[i].size(); ++j) {
                    if (m_data[i][j] == target) {
                        indices.push_back(std::make_pair(i, j));
                    }
                }
            }
            return indices;
        }

        template <typename T>
        std::pair<size_t, size_t> Array2D<T>::findNearest(const T& target) const {
            if (empty()) {
                throw std::runtime_error("ct::array::findNearest: Cannot search an empty array.");
            }

            // Use double for distance calculation to avoid overflow and handle non-integer types
            size_t best_r   = 0;
            size_t best_c   = 0;
            double min_diff = std::numeric_limits<double>::max();

            for (size_t i = 0; i < m_data.size(); ++i) {
                for (size_t j = 0; j < m_data[i].size(); ++j) {
                    double diff = std::abs(static_cast<double>(m_data[i][j]) - static_cast<double>(target));
                    if (diff < min_diff) {
                        min_diff = diff;
                        best_r   = i;
                        best_c   = j;
                    }
                }
            }

            return std::make_pair(best_r, best_c);
        }

        template <typename T>
        Array2D<T> Array2D<T>::transpose() const {
            if (empty()) {
                return Array2D<T>(0, 0);
            }

            // Original: rows x cols
            // Transpose: cols x rows
            size_t new_rows = m_data[0].size();    // Original columns
            size_t new_cols = m_data.size();       // Original rows

            Array2D<T> result(new_rows, new_cols);

            // Map original(r, c) to result(c, r)
            for (size_t r = 0; r < m_data.size(); ++r) {
                for (size_t c = 0; c < m_data[r].size(); ++c) {
                    result(c, r) = m_data[r][c];
                }
            }

            return result;
        }

        template <typename T>
        T Array2D<T>::max() const {
            if (empty()) {
                throw std::runtime_error("ct::array::max: Cannot get max of empty array.");
            }
            T m = m_data[0][0];
            for (const auto& row : m_data) {
                T row_max = *std::max_element(row.begin(), row.end());
                if (row_max > m) m = row_max;
            }
            return m;
        }

        template <typename T>
        T Array2D<T>::min() const {
            if (empty()) {
                throw std::runtime_error("ct::array::min: Cannot get min of empty array.");
            }
            T m = m_data[0][0];
            for (const auto& row : m_data) {
                T row_min = *std::min_element(row.begin(), row.end());
                if (row_min < m) m = row_min;
            }
            return m;
        }

        // --- Stream Output ---
        template <typename T>
        std::ostream& operator<<(std::ostream& os, const Array2D<T>& arr) {
            os << "[\n";
            size_t r_count = arr.rows();
            for (size_t i = 0; i < r_count; ++i) {
                os << "  [";
                size_t c_count = arr.cols();
                for (size_t j = 0; j < c_count; ++j) {
                    os << arr(i, j);
                    if (j < c_count - 1) os << ", ";
                }
                os << "]";
                if (i < r_count - 1) os << ",";
                os << "\n";
            }
            os << "]";
            return os;
        }

    }    // namespace array
}    // namespace ct