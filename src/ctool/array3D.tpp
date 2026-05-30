/**
 * @file array3D.tpp
 * @brief Template implementations for ctool_array3D.hpp.
 * @author Florent ALBANY - FAL
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#include <algorithm>
#include <stdexcept>
#include <cmath>
#include "array2D.hpp" // Include the 2D array header to use ctool::array::Array

namespace ctool {
    namespace array {

        // --- Constructors ---
        template <typename T>
        Array3D<T>::Array3D() = default;

        template <typename T>
        Array3D<T>::Array3D(size_t depth, size_t rows, size_t cols, const T& initValue) {
            if (depth == 0 || rows == 0 || cols == 0) {
                m_data.clear();
                return;
            }
            // Create 2D vector for each slice
            m_data.resize(depth, std::vector<std::vector<T>>(rows, std::vector<T>(cols, initValue)));
        }

        template <typename T>
        Array3D<T>::Array3D(const std::vector<std::vector<std::vector<T>>>& data) {
            if (data.empty()) {
                return;
            }

            size_t expectedRows = data[0].size();
            size_t expectedCols = (expectedRows > 0) ? data[0][0].size() : 0;

            for (const auto& slice : data) {
                if (slice.size() != expectedRows) {
                    throw std::invalid_argument("ctool::arrayInput data must have consistent row counts per slice.");
                }
                for (const auto& row : slice) {
                    if (row.size() != expectedCols) {
                        throw std::invalid_argument("ctool::array: Input data must have consistent column counts.");
                    }
                }
            }
            m_data = data;
        }

        // --- Dimensions ---
        template <typename T>
        size_t Array3D<T>::depth() const noexcept {
            return m_data.size();
        }

        template <typename T>
        size_t Array3D<T>::rows() const noexcept {
            return m_data.empty() ? 0 : m_data[0].size();
        }

        template <typename T>
        size_t Array3D<T>::cols() const noexcept {
            return m_data.empty() || m_data[0].empty() ? 0 : m_data[0][0].size();
        }

        template <typename T>
        size_t Array3D<T>::size() const noexcept {
            return depth() * rows() * cols();
        }

        template <typename T>
        bool Array3D<T>::empty() const noexcept {
            return m_data.empty() || m_data[0].empty() || m_data[0][0].empty();
        }

        template <typename T>
        void Array3D<T>::clear() {
            m_data.clear();
        }

        // --- Element Access ---
        template <typename T>
        T& Array3D<T>::at(size_t depth, size_t row, size_t col) {
            if (depth >= m_data.size() || row >= m_data[depth].size() || col >= m_data[depth][0].size()) {
                throw std::out_of_range("ctool::array::at: Index out of bounds.");
            }
            return m_data[depth][row][col];
        }

        template <typename T>
        const T& Array3D<T>::at(size_t depth, size_t row, size_t col) const {
            if (depth >= m_data.size() || row >= m_data[depth].size() || col >= m_data[depth][0].size()) {
                throw std::out_of_range("ctool::array::at: Index out of bounds.");
            }
            return m_data[depth][row][col];
        }

        template <typename T>
        T& Array3D<T>::operator()(size_t depth, size_t row, size_t col) {
            return m_data[depth][row][col];
        }

        template <typename T>
        const T& Array3D<T>::operator()(size_t depth, size_t row, size_t col) const {
            return m_data[depth][row][col];
        }

        // --- Operations ---
        template <typename T>
        void Array3D<T>::fill(const T& val) {
            for (auto& slice : m_data) {
                for (auto& row : slice) {
                    row.assign(row.size(), val);
                }
            }
        }

        template <typename T>
        void Array3D<T>::fillDepth(size_t depth, const T& val) {
            if (depth >= m_data.size()) {
                throw std::out_of_range("ctool::array::fillDepth: Slice index out of bounds.");
            }
            auto& slice = m_data[depth];
            for (auto& row : slice) {
                std::fill(row.begin(), row.end(), val);
            }
        }

        template <typename T>
        void Array3D<T>::fillRow(size_t row, const T& val) {
            if (empty() || row >= m_data[0].size()) {
                throw std::out_of_range("ctool::array::fillRow: Row index out of bounds.");
            }
            for (auto& slice : m_data) {
                std::fill(slice[row].begin(), slice[row].end(), val);
            }
        }

        template <typename T>
        void Array3D<T>::fillColumn(size_t col, const T& val) {
            if (empty() || col >= m_data[0][0].size()) {
                throw std::out_of_range("ctool::array::fillColumn: Column index out of bounds.");
            }
            for (auto& slice : m_data) {
                for (auto& row : slice) {
                    row[col] = val;
                }
            }
        }

        template <typename T>
        void Array3D<T>::resize(size_t depth, size_t rows, size_t cols, const T& val) {
            if (depth == 0 && rows == 0 && cols == 0) {
                m_data.clear();
                return;
            }

            // Resize outer vector (depth)
            if (depth > m_data.size()) {
                m_data.resize(depth, std::vector<std::vector<T>>(rows, std::vector<T>(cols, val)));
            } else {
                m_data.resize(depth);
            }

            // Resize inner vectors (rows and cols)
            for (auto& slice : m_data) {
                if (slice.size() < rows) {
                    slice.resize(rows, std::vector<T>(cols, val));
                } else if (slice.size() > rows) {
                    slice.resize(rows);
                }
                // Resize columns within each row
                for (auto& row : slice) {
                    if (row.size() < cols) {
                        row.resize(cols, val);
                    } else if (row.size() > cols) {
                        row.resize(cols);
                    }
                }
            }
        }

        // --- Math / Transformation ---
        template <typename T>
        Array3D<T>& Array3D<T>::scale(double factor) {
            for (auto& slice : m_data) {
                for (auto& row : slice) {
                    for (auto& val : row) {
                        val = static_cast<T>(val * factor);
                    }
                }
            }
            return *this;
        }

        template <typename T>
        Array3D<T>& Array3D<T>::add(double val) {
            for (auto& slice : m_data) {
                for (auto& row : slice) {
                    for (auto& element : row) {
                        element = static_cast<T>(element + val);
                    }
                }
            }
            return *this;
        }

        // --- Utility ---
        template <typename T>
        const std::vector<std::vector<std::vector<T>>>& Array3D<T>::toVector() const {
            return m_data;
        }

        template <typename T>
        T Array3D<T>::max() const {
            if (empty()) {
                throw std::runtime_error("ctool::array::max: Cannot get max of empty array.");
            }
            T m = m_data[0][0][0];
            for (const auto& slice : m_data) {
                for (const auto& row : slice) {
                    T row_max = *std::max_element(row.begin(), row.end());
                    if (row_max > m) m = row_max;
                }
            }
            return m;
        }

        template <typename T>
        T Array3D<T>::min() const {
            if (empty()) {
                throw std::runtime_error("ctool::array::min: Cannot get min of empty array.");
            }
            T m = m_data[0][0][0];
            for (const auto& slice : m_data) {
                for (const auto& row : slice) {
                    T row_min = *std::min_element(row.begin(), row.end());
                    if (row_min < m) m = row_min;
                }
            }
            return m;
        }

        template <typename T>
        ctool::array::Array2D<T> Array3D<T>::slice(size_t d) const {
            if (d >= m_data.size()) {
                throw std::out_of_range("ctool::array::slice: Slice index out of bounds.");
            }
            return ctool::array::Array2D<T>(m_data[d]);
        }

        // --- Stream Output ---
        template <typename T>
        std::ostream& operator<<(std::ostream& os, const Array3D<T>& arr) {
            os << "Array3D [Depth: " << arr.depth() 
               << ", Rows: " << arr.rows() 
               << ", Cols: " << arr.cols() << "]:\n";
            
            size_t depthsCount = arr.depth();
            for (size_t i = 0; i < depthsCount; ++i) {
                os << "Slice " << i << ":\n";
                const auto& slice = arr.m_data[i];
                size_t rowsCount = slice.size();
                for (size_t r = 0; r < rowsCount; ++r) {
                    os << "  Row " << r << ": [";
                    size_t colsCount = slice[r].size();
                    for (size_t c = 0; c < colsCount; ++c) {
                        os << slice[r][c];
                        if (c < colsCount - 1) os << ", ";
                    }
                    os << "]\n";
                }
            }
            return os;
        }

    }    // namespace array
}    // namespace ctool