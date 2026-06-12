/**
 * @file array.tpp
 * @brief Template implementations for ctool_array.hpp.
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

namespace ctool {
    namespace array {

        // --- Constructors ---
        template <typename T>
        Array2D<T>::Array2D() : m_rows(0), m_cols(0) {}

        template <typename T>
        Array2D<T>::Array2D(size_t rows, size_t cols, const T& initValue) 
            : m_data(rows * cols, initValue), m_rows(rows), m_cols(cols) {}

        template <typename T>
        Array2D<T>::Array2D(const std::vector<std::vector<T>>& data) {
            if (data.empty()) {
                m_rows = 0;
                m_cols = 0;
                return;
            }

            m_rows = data.size();
            m_cols = data[0].size();
            m_data.reserve(m_rows * m_cols);

            for (const auto& row : data) {
                if (row.size() != m_cols) {
                    throw std::invalid_argument("ctool::array: Input data must be a rectangular matrix (consistent row lengths).");
                }
                m_data.insert(m_data.end(), row.begin(), row.end());
            }
        }

        // --- Size & Dimensions ---
        template <typename T>
        size_t Array2D<T>::rows() const noexcept {
            return m_rows;
        }

        template <typename T>
        size_t Array2D<T>::cols() const noexcept {
            return m_cols;
        }

        template <typename T>
        size_t Array2D<T>::size() const noexcept {
            return m_data.size();
        }

        template <typename T>
        bool Array2D<T>::empty() const noexcept {
            return m_data.empty();
        }

        template <typename T>
        void Array2D<T>::clear() {
            m_data.clear();
            m_rows = 0;
            m_cols = 0;
        }

        // --- Element Access ---
        template <typename T>
        T& Array2D<T>::at(size_t row, size_t col) {
            if (row >= m_rows || col >= m_cols) {
                throw std::out_of_range("ctool::array::at: Index out of bounds.");
            }
            return m_data[index(row, col)];
        }

        template <typename T>
        const T& Array2D<T>::at(size_t row, size_t col) const {
            if (row >= m_rows || col >= m_cols) {
                throw std::out_of_range("ctool::array::at: Index out of bounds.");
            }
            return m_data[index(row, col)];
        }

        template <typename T>
        T& Array2D<T>::operator()(size_t row, size_t col) {
            return m_data[index(row, col)];
        }

        template <typename T>
        const T& Array2D<T>::operator()(size_t row, size_t col) const {
            return m_data[index(row, col)];
        }

        template <typename T>
        T* Array2D<T>::data() noexcept {
            return m_data.data();
        }

        template <typename T>
        const T* Array2D<T>::data() const noexcept {
            return m_data.data();
        }

        // --- Operations ---
        template <typename T>
        void Array2D<T>::fill(const T& val) {
            std::fill(m_data.begin(), m_data.end(), val);
        }

        template <typename T>
        void Array2D<T>::fillRow(size_t row, const T& val) {
            if (row >= m_rows) {
                throw std::out_of_range("ctool::array::fillRow: Row index out of bounds.");
            }
            std::fill(m_data.begin() + (row * m_cols), m_data.begin() + ((row + 1) * m_cols), val);
        }

        template <typename T>
        void Array2D<T>::fillColumn(size_t col, const T& val) {
            if (col >= m_cols) {
                throw std::out_of_range("ctool::array::fillColumn: Column index out of bounds.");
            }
            for (size_t i = 0; i < m_rows; ++i) {
                m_data[index(i, col)] = val;
            }
        }

        template <typename T>
        void Array2D<T>::pushRow(size_t cols, const T& val) {
            if (m_cols != 0 && cols != m_cols) {
                throw std::invalid_argument("ctool::array::pushRow: Column count mismatch.");
            }
            if (m_cols == 0) m_cols = cols;
            m_data.insert(m_data.end(), cols, val);
            m_rows++;
        }

        template <typename T>
        void Array2D<T>::resize(size_t rows, size_t cols, const T& val) {
            if (rows == 0 || cols == 0) {
                clear();
                return;
            }

            if (cols == m_cols) {
                m_data.resize(rows * cols, val);
                m_rows = rows;
            } else {
                // Changing columns requires re-mapping
                std::vector<T> newData(rows * cols, val);
                size_t minRows = std::min(rows, m_rows);
                size_t minCols = std::min(cols, m_cols);

                for (size_t i = 0; i < minRows; ++i) {
                    for (size_t j = 0; j < minCols; ++j) {
                        newData[i * cols + j] = m_data[index(i, j)];
                    }
                }
                m_data = std::move(newData);
                m_rows = rows;
                m_cols = cols;
            }
        }

        // --- Math / Transformation ---
        template <typename T>
        Array2D<T>& Array2D<T>::scale(double factor) {
            for (auto& val : m_data) {
                val = static_cast<T>(val * factor);
            }
            return *this;
        }

        template <typename T>
        Array2D<T>& Array2D<T>::add(double val) {
            for (auto& element : m_data) {
                element = static_cast<T>(element + val);
            }
            return *this;
        }

        // --- Utility ---
        template <typename T>
        std::vector<std::vector<T>> Array2D<T>::toVector() const {
            std::vector<std::vector<T>> result(m_rows, std::vector<T>(m_cols));
            for (size_t i = 0; i < m_rows; ++i) {
                for (size_t j = 0; j < m_cols; ++j) {
                    result[i][j] = m_data[index(i, j)];
                }
            }
            return result;
        }

        template <typename T>
        std::vector<T> Array2D<T>::sliceRow(size_t row) const {
            if (row >= m_rows) {
                throw std::out_of_range("ctool::array::sliceRow: Row index out of bounds.");
            }
            return std::vector<T>(m_data.begin() + (row * m_cols), m_data.begin() + ((row + 1) * m_cols));
        }

        template <typename T>
        std::vector<T> Array2D<T>::sliceColumn(size_t col) const {
            if (col >= m_cols) {
                throw std::out_of_range("ctool::array::sliceColumn: Column index out of bounds.");
            }
            std::vector<T> result_col;
            result_col.reserve(m_rows);
            for (size_t i = 0; i < m_rows; ++i) {
                result_col.push_back(m_data[index(i, col)]);
            }
            return result_col;
        }

        template <typename T>
        Array2D<T> Array2D<T>::subArray(size_t row, size_t col, size_t rows, size_t cols) const {
            if (empty()) {
                return Array2D<T>(0, 0);
            }

            if (row >= m_rows || col >= m_cols) {
                throw std::out_of_range("ctool::array::subArray: Top-left index out of bounds.");
            }

            if (row + rows > m_rows || col + cols > m_cols) {
                throw std::out_of_range("ctool::array::subArray: Requested sub-array dimensions exceed bounds.");
            }

            Array2D<T> result(rows, cols);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result(i, j) = m_data[index(row + i, col + j)];
                }
            }
            return result;
        }

        // --- Find Functionality ---

        template <typename T>
        std::pair<size_t, size_t> Array2D<T>::findValue(const T& target) const {
            if (empty()) {
                throw std::runtime_error("ctool::array::findValue: Cannot search an empty array.");
            }

            for (size_t i = 0; i < m_rows; ++i) {
                for (size_t j = 0; j < m_cols; ++j) {
                    if (m_data[index(i, j)] == target) {
                        return std::make_pair(i, j);
                    }
                }
            }
            throw std::runtime_error("ctool::array::findValue: Value not found in array.");
        }

        template <typename T>
        std::vector<std::pair<size_t, size_t>> Array2D<T>::findAllValues(const T& target) const {
            std::vector<std::pair<size_t, size_t>> indices;
            if (empty()) {
                return indices;
            }

            for (size_t i = 0; i < m_rows; ++i) {
                for (size_t j = 0; j < m_cols; ++j) {
                    if (m_data[index(i, j)] == target) {
                        indices.push_back(std::make_pair(i, j));
                    }
                }
            }
            return indices;
        }

        template <typename T>
        std::pair<size_t, size_t> Array2D<T>::findNearest(const T& target) const {
            if (empty()) {
                throw std::runtime_error("ctool::array::findNearest: Cannot search an empty array.");
            }

            size_t bestRow   = 0;
            size_t bestCol   = 0;
            double minDiff = std::numeric_limits<double>::max();

            for (size_t i = 0; i < m_rows; ++i) {
                for (size_t j = 0; j < m_cols; ++j) {
                    double diff = std::abs(static_cast<double>(m_data[index(i, j)]) - static_cast<double>(target));
                    if (diff < minDiff) {
                        minDiff = diff;
                        bestRow   = i;
                        bestCol   = j;
                    }
                }
            }

            return std::make_pair(bestRow, bestCol);
        }

        template <typename T>
        Array2D<T> Array2D<T>::transpose() const {
            if (empty()) {
                return Array2D<T>(0, 0);
            }

            Array2D<T> result(m_cols, m_rows);

            for (size_t r = 0; r < m_rows; ++r) {
                for (size_t c = 0; c < m_cols; ++c) {
                    result(c, r) = m_data[index(r, c)];
                }
            }

            return result;
        }

        template <typename T>
        T Array2D<T>::max() const {
            if (empty()) {
                throw std::runtime_error("ctool::array::max: Cannot get max of empty array.");
            }
            return *std::max_element(m_data.begin(), m_data.end());
        }

        template <typename T>
        T Array2D<T>::min() const {
            if (empty()) {
                throw std::runtime_error("ctool::array::min: Cannot get min of empty array.");
            }
            return *std::min_element(m_data.begin(), m_data.end());
        }

        // --- Stream Output ---
        template <typename T>
        std::ostream& operator<<(std::ostream& os, const Array2D<T>& arr) {
            os << "[\n";
            size_t rowsCount = arr.rows();
            size_t colsCount = arr.cols();
            for (size_t i = 0; i < rowsCount; ++i) {
                os << "  [";
                for (size_t j = 0; j < colsCount; ++j) {
                    os << arr(i, j);
                    if (j < colsCount - 1) os << ", ";
                }
                os << "]";
                if (i < rowsCount - 1) os << ",";
                os << "\n";
            }
            os << "]";
            return os;
        }

    }    // namespace array
}    // namespace ctool
