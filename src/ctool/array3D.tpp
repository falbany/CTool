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
#include "array2D.hpp"

namespace ctool {
    namespace array {

        // --- Constructors ---
        template <typename T>
        Array3D<T>::Array3D() : m_depth(0), m_rows(0), m_cols(0) {}

        template <typename T>
        Array3D<T>::Array3D(size_t depth, size_t rows, size_t cols, const T& initValue) 
            : m_data(depth * rows * cols, initValue), m_depth(depth), m_rows(rows), m_cols(cols) {}

        template <typename T>
        Array3D<T>::Array3D(const std::vector<std::vector<std::vector<T>>>& data) {
            if (data.empty()) {
                m_depth = 0;
                m_rows = 0;
                m_cols = 0;
                return;
            }

            m_depth = data.size();
            m_rows = data[0].size();
            m_cols = (m_rows > 0) ? data[0][0].size() : 0;
            m_data.reserve(m_depth * m_rows * m_cols);

            for (const auto& slice : data) {
                if (slice.size() != m_rows) {
                    throw std::invalid_argument("ctool::array: Input data must have consistent row counts per slice.");
                }
                for (const auto& row : slice) {
                    if (row.size() != m_cols) {
                        throw std::invalid_argument("ctool::array: Input data must have consistent column counts.");
                    }
                    m_data.insert(m_data.end(), row.begin(), row.end());
                }
            }
        }

        // --- Dimensions ---
        template <typename T>
        size_t Array3D<T>::depth() const noexcept {
            return m_depth;
        }

        template <typename T>
        size_t Array3D<T>::rows() const noexcept {
            return m_rows;
        }

        template <typename T>
        size_t Array3D<T>::cols() const noexcept {
            return m_cols;
        }

        template <typename T>
        size_t Array3D<T>::size() const noexcept {
            return m_data.size();
        }

        template <typename T>
        bool Array3D<T>::empty() const noexcept {
            return m_data.empty();
        }

        template <typename T>
        void Array3D<T>::clear() {
            m_data.clear();
            m_depth = 0;
            m_rows = 0;
            m_cols = 0;
        }

        // --- Element Access ---
        template <typename T>
        T& Array3D<T>::at(size_t depth, size_t row, size_t col) {
            if (depth >= m_depth || row >= m_rows || col >= m_cols) {
                throw std::out_of_range("ctool::array::at: Index out of bounds.");
            }
            return m_data[index(depth, row, col)];
        }

        template <typename T>
        const T& Array3D<T>::at(size_t depth, size_t row, size_t col) const {
            if (depth >= m_depth || row >= m_rows || col >= m_cols) {
                throw std::out_of_range("ctool::array::at: Index out of bounds.");
            }
            return m_data[index(depth, row, col)];
        }

        template <typename T>
        T& Array3D<T>::operator()(size_t depth, size_t row, size_t col) {
            return m_data[index(depth, row, col)];
        }

        template <typename T>
        const T& Array3D<T>::operator()(size_t depth, size_t row, size_t col) const {
            return m_data[index(depth, row, col)];
        }

        template <typename T>
        T* Array3D<T>::data() noexcept {
            return m_data.data();
        }

        template <typename T>
        const T* Array3D<T>::data() const noexcept {
            return m_data.data();
        }

        // --- Operations ---
        template <typename T>
        void Array3D<T>::fill(const T& val) {
            std::fill(m_data.begin(), m_data.end(), val);
        }

        template <typename T>
        void Array3D<T>::fillDepth(size_t depth, const T& val) {
            if (depth >= m_depth) {
                throw std::out_of_range("ctool::array::fillDepth: Slice index out of bounds.");
            }
            std::fill(m_data.begin() + (depth * m_rows * m_cols), 
                      m_data.begin() + ((depth + 1) * m_rows * m_cols), val);
        }

        template <typename T>
        void Array3D<T>::fillRow(size_t row, const T& val) {
            if (row >= m_rows) {
                throw std::out_of_range("ctool::array::fillRow: Row index out of bounds.");
            }
            for (size_t d = 0; d < m_depth; ++d) {
                std::fill(m_data.begin() + index(d, row, 0), 
                          m_data.begin() + index(d, row, m_cols), val);
            }
        }

        template <typename T>
        void Array3D<T>::fillColumn(size_t col, const T& val) {
            if (col >= m_cols) {
                throw std::out_of_range("ctool::array::fillColumn: Column index out of bounds.");
            }
            for (size_t d = 0; d < m_depth; ++d) {
                for (size_t r = 0; r < m_rows; ++r) {
                    m_data[index(d, r, col)] = val;
                }
            }
        }

        template <typename T>
        void Array3D<T>::resize(size_t depth, size_t rows, size_t cols, const T& val) {
            if (depth == 0 || rows == 0 || cols == 0) {
                clear();
                return;
            }

            if (rows == m_rows && cols == m_cols) {
                m_data.resize(depth * rows * cols, val);
                m_depth = depth;
            } else {
                std::vector<T> newData(depth * rows * cols, val);
                size_t minDepth = std::min(depth, m_depth);
                size_t minRows = std::min(rows, m_rows);
                size_t minCols = std::min(cols, m_cols);

                for (size_t d = 0; d < minDepth; ++d) {
                    for (size_t r = 0; r < minRows; ++r) {
                        for (size_t c = 0; c < minCols; ++c) {
                            newData[(d * rows * cols) + (r * cols) + c] = m_data[index(d, r, c)];
                        }
                    }
                }
                m_data = std::move(newData);
                m_depth = depth;
                m_rows = rows;
                m_cols = cols;
            }
        }

        // --- Math / Transformation ---
        template <typename T>
        Array3D<T>& Array3D<T>::scale(double factor) {
            for (auto& val : m_data) {
                val = static_cast<T>(val * factor);
            }
            return *this;
        }

        template <typename T>
        Array3D<T>& Array3D<T>::add(double val) {
            for (auto& element : m_data) {
                element = static_cast<T>(element + val);
            }
            return *this;
        }

        // --- Utility ---
        template <typename T>
        std::vector<std::vector<std::vector<T>>> Array3D<T>::toVector() const {
            std::vector<std::vector<std::vector<T>>> result(m_depth, 
                std::vector<std::vector<T>>(m_rows, std::vector<T>(m_cols)));
            
            for (size_t d = 0; d < m_depth; ++d) {
                for (size_t r = 0; r < m_rows; ++r) {
                    for (size_t c = 0; c < m_cols; ++c) {
                        result[d][r][c] = m_data[index(d, r, c)];
                    }
                }
            }
            return result;
        }

        template <typename T>
        T Array3D<T>::max() const {
            if (empty()) {
                throw std::runtime_error("ctool::array::max: Cannot get max of empty array.");
            }
            return *std::max_element(m_data.begin(), m_data.end());
        }

        template <typename T>
        T Array3D<T>::min() const {
            if (empty()) {
                throw std::runtime_error("ctool::array::min: Cannot get min of empty array.");
            }
            return *std::min_element(m_data.begin(), m_data.end());
        }

        template <typename T>
        ctool::array::Array2D<T> Array3D<T>::slice(size_t d) const {
            if (d >= m_depth) {
                throw std::out_of_range("ctool::array::slice: Slice index out of bounds.");
            }
            
            ctool::array::Array2D<T> result(m_rows, m_cols);
            for (size_t r = 0; r < m_rows; ++r) {
                for (size_t c = 0; c < m_cols; ++c) {
                    result(r, c) = m_data[index(d, r, c)];
                }
            }
            return result;
        }

        // --- Stream Output ---
        template <typename T>
        std::ostream& operator<<(std::ostream& os, const Array3D<T>& arr) {
            os << "Array3D [Depth: " << arr.depth() 
               << ", Rows: " << arr.rows() 
               << ", Cols: " << arr.cols() << "]:\n";
            
            for (size_t i = 0; i < arr.depth(); ++i) {
                os << "Slice " << i << ":\n";
                for (size_t r = 0; r < arr.rows(); ++r) {
                    os << "  Row " << r << ": [";
                    for (size_t c = 0; c < arr.cols(); ++c) {
                        os << arr(i, r, c);
                        if (c < arr.cols() - 1) os << ", ";
                    }
                    os << "]\n";
                }
            }
            return os;
        }

    }    // namespace array
}    // namespace ctool
