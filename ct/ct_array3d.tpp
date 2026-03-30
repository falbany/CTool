/**
 * @file ct_array3d.tpp
 * @brief Template implementations for ct_array3d.hpp.
 * @author Florent ALBANY - FAL
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#include <algorithm>
#include <stdexcept>
#include <cmath>
#include "ct_array2d.hpp" // Include the 2D array header to use ct::array::Array

namespace ct {
    namespace array {

        // --- Constructors ---
        template <typename T>
        Array3D<T>::Array3D() : m_data() {}

        template <typename T>
        Array3D<T>::Array3D(size_t depth, size_t rows, size_t cols, const T& init_value) {
            if (depth == 0 || rows == 0 || cols == 0) {
                m_data.clear();
                return;
            }
            // Create 2D vector for each slice
            m_data.resize(depth, std::vector<std::vector<T>>(rows, std::vector<T>(cols, init_value)));
        }

        template <typename T>
        Array3D<T>::Array3D(const std::vector<std::vector<std::vector<T>>>& data) {
            if (data.empty()) {
                return;
            }

            size_t expected_rows = data[0].size();
            size_t expected_cols = (expected_rows > 0) ? data[0][0].size() : 0;

            for (const auto& slice : data) {
                if (slice.size() != expected_rows) {
                    throw std::invalid_argument("ct::arrayInput data must have consistent row counts per slice.");
                }
                for (const auto& row : slice) {
                    if (row.size() != expected_cols) {
                        throw std::invalid_argument("ct::array: Input data must have consistent column counts.");
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
        T& Array3D<T>::at(size_t d, size_t r, size_t c) {
            if (d >= m_data.size() || r >= m_data[d].size() || c >= m_data[d][0].size()) {
                throw std::out_of_range("ct::array::at: Index out of bounds.");
            }
            return m_data[d][r][c];
        }

        template <typename T>
        const T& Array3D<T>::at(size_t d, size_t r, size_t c) const {
            if (d >= m_data.size() || r >= m_data[d].size() || c >= m_data[d][0].size()) {
                throw std::out_of_range("ct::array::at: Index out of bounds.");
            }
            return m_data[d][r][c];
        }

        template <typename T>
        T& Array3D<T>::operator()(size_t d, size_t r, size_t c) {
            return m_data[d][r][c];
        }

        template <typename T>
        const T& Array3D<T>::operator()(size_t d, size_t r, size_t c) const {
            return m_data[d][r][c];
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
        std::vector<std::vector<std::vector<T>>> Array3D<T>::toVector() const {
            return m_data;
        }

        template <typename T>
        T Array3D<T>::max() const {
            if (empty()) {
                throw std::runtime_error("ct::array::max: Cannot get max of empty array.");
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
                throw std::runtime_error("ct::array::min: Cannot get min of empty array.");
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
        ct::array::Array2D<T> Array3D<T>::slice(size_t d) const {
            if (d >= m_data.size()) {
                throw std::out_of_range("ct::array::slice: Slice index out of bounds.");
            }
            return ct::array::Array2D<T>(m_data[d]);
        }

        // --- Stream Output ---
        template <typename T>
        std::ostream& operator<<(std::ostream& os, const Array3D<T>& arr) {
            os << "Array3D [Depth: " << arr.depth() 
               << ", Rows: " << arr.rows() 
               << ", Cols: " << arr.cols() << "]:\n";
            
            size_t d_count = arr.depth();
            for (size_t i = 0; i < d_count; ++i) {
                os << "Slice " << i << ":\n";
                const auto& slice = arr.m_data[i];
                size_t r_count = slice.size();
                for (size_t r = 0; r < r_count; ++r) {
                    os << "  Row " << r << ": [";
                    size_t c_count = slice[r].size();
                    for (size_t c = 0; c < c_count; ++c) {
                        os << slice[r][c];
                        if (c < c_count - 1) os << ", ";
                    }
                    os << "]\n";
                }
            }
            return os;
        }

    }    // namespace array
}    // namespace ct