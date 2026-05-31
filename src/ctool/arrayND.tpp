/**
 * @file arrayND.tpp
 * @author Florent ALBANY - FAL
 * @brief Template implementations for arrayND.hpp.
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#include "ctool/arrayND.hpp"
#include <numeric>

namespace ctool {
    namespace array {

        // --- Constructors ---
        template <typename T>
        ArrayND<T>::ArrayND() : m_rows(0), m_cols(0) {}

        template <typename T>
        ArrayND<T>::ArrayND(size_t rows, size_t cols) : m_data(rows * cols, T()), m_rows(rows), m_cols(cols) {}

        template <typename T>
        ArrayND<T>::ArrayND(const std::initializer_list<std::initializer_list<T>>& list) {
            if (list.size() == 0) {
                m_rows = 0;
                m_cols = 0;
                return;
            }

            m_rows = list.size();
            m_cols = list.begin()->size();
            m_data.reserve(m_rows * m_cols);

            for (const auto& row : list) {
                if (row.size() != m_cols) {
                    throw std::invalid_argument("ctool::num::ArrayND: Jagged arrays are not supported.");
                }
                for (const auto& val : row) {
                    m_data.push_back(val);
                }
            }
        }

        // --- Size & Shape ---
        template <typename T>
        size_t ArrayND<T>::rows() const noexcept {
            return m_rows;
        }

        template <typename T>
        size_t ArrayND<T>::cols() const noexcept {
            return m_cols;
        }

        template <typename T>
        size_t ArrayND<T>::size() const noexcept {
            return m_data.size();
        }

        template <typename T>
        std::pair<size_t, size_t> ArrayND<T>::shape() const noexcept {
            return {m_rows, m_cols};
        }

        template <typename T>
        bool ArrayND<T>::empty() const noexcept {
            return m_data.empty();
        }

        template <typename T>
        void ArrayND<T>::clear() {
            m_data.clear();
            m_rows = 0;
            m_cols = 0;
        }

        // --- Access ---
        template <typename T>
        T& ArrayND<T>::at(size_t row, size_t col) {
            if (row >= m_rows || col >= m_cols) throw std::out_of_range("ctool::num::ArrayND::at");
            return m_data[index(row, col)];
        }

        template <typename T>
        const T& ArrayND<T>::at(size_t row, size_t col) const {
            if (row >= m_rows || col >= m_cols) throw std::out_of_range("ctool::num::ArrayND::at");
            return m_data[index(row, col)];
        }

        template <typename T>
        T& ArrayND<T>::operator()(size_t row, size_t col) {
            return m_data[index(row, col)];
        }

        template <typename T>
        const T& ArrayND<T>::operator()(size_t row, size_t col) const {
            return m_data[index(row, col)];
        }

        template <typename T>
        T* ArrayND<T>::data() noexcept {
            return m_data.data();
        }

        template <typename T>
        const T* ArrayND<T>::data() const noexcept {
            return m_data.data();
        }

        // --- Arithmetic Operators ---
        // Note: These operations optimize using the contiguous 1D vector.

        template <typename T>
        ArrayND<T> operator+(const ArrayND<T>& lhs, const ArrayND<T>& rhs) {
            if (lhs.shape() != rhs.shape()) {
                throw std::invalid_argument("ctool::num::operator+: Shapes do not match.");
            }
            ArrayND<T> res = lhs;    // Copy LHS
            for (size_t i = 0; i < lhs.size(); ++i) {
                res.data()[i] += rhs.data()[i];
            }
            return res;
        }

        template <typename T>
        ArrayND<T> operator+(const ArrayND<T>& lhs, const T& scalar) {
            ArrayND<T> res = lhs;
            for (auto& val : res) {
                val += scalar;
            }
            return res;
        }

        template <typename T>
        ArrayND<T> operator+(const T& scalar, const ArrayND<T>& rhs) {
            return rhs + scalar;    // Commutative
        }

        template <typename T>
        ArrayND<T> operator-(const ArrayND<T>& lhs, const ArrayND<T>& rhs) {
            if (lhs.shape() != rhs.shape()) {
                throw std::invalid_argument("ctool::num::operator-: Shapes do not match.");
            }
            ArrayND<T> res = lhs;
            for (size_t i = 0; i < lhs.size(); ++i) {
                res.data()[i] -= rhs.data()[i];
            }
            return res;
        }

        template <typename T>
        ArrayND<T> operator-(const ArrayND<T>& lhs, const T& scalar) {
            ArrayND<T> res = lhs;
            for (size_t i = 0; i < res.size(); ++i) {
                res.data()[i] -= scalar;
            }
            return res;
        }

        template <typename T>
        ArrayND<T> operator*(const ArrayND<T>& lhs, const ArrayND<T>& rhs) {
            if (lhs.shape() != rhs.shape()) {
                throw std::invalid_argument("ctool::num::operator*: Shapes do not match.");
            }
            ArrayND<T> res = lhs;
            for (size_t i = 0; i < lhs.size(); ++i) {
                res.data()[i] *= rhs.data()[i];
            }
            return res;
        }

        template <typename T>
        ArrayND<T> operator*(const ArrayND<T>& lhs, const T& scalar) {
            ArrayND<T> res = lhs;
            for (size_t i = 0; i < res.size(); ++i) {
                res.data()[i] *= scalar;
            }
            return res;
        }

        template <typename T>
        ArrayND<T> operator/(const ArrayND<T>& lhs, const T& scalar) {
            ArrayND<T> res = lhs;
            for (size_t i = 0; i < res.size(); ++i) {
                res.data()[i] /= scalar;
            }
            return res;
        }

        // --- Universal Functions ---

        template <typename T>
        ArrayND<double> sqrt(const ArrayND<T>& arr) {
            ArrayND<double> res(arr.rows(), arr.cols());
            for (size_t i = 0; i < arr.size(); ++i) {
                res.data()[i] = static_cast<double>(std::sqrt(static_cast<double>(arr.data()[i])));
            }
            return res;
        }

        template <typename T>
        ArrayND<double> sin(const ArrayND<T>& arr) {
            ArrayND<double> res(arr.rows(), arr.cols());
            for (size_t i = 0; i < arr.size(); ++i) {
                res.data()[i] = static_cast<double>(std::sin(static_cast<double>(arr.data()[i])));
            }
            return res;
        }

        template <typename T>
        ArrayND<T> abs(const ArrayND<T>& arr) {
            ArrayND<T> res(arr.rows(), arr.cols());
            for (size_t i = 0; i < arr.size(); ++i) {
                res.data()[i] = static_cast<T>(std::abs(arr.data()[i]));
            }
            return res;
        }

        // --- Reductions ---

        template <typename T>
        double sum(const ArrayND<T>& arr) {
            double result = 0.0;
            for (const auto& val : arr) {
                result += static_cast<double>(val);
            }
            return result;
        }

        template <typename T>
        double mean(const ArrayND<T>& arr) {
            if (arr.empty()) return 0.0;
            return sum(arr) / static_cast<double>(arr.size());
        }

        template <typename T>
        T max(const ArrayND<T>& arr) {
            if (arr.empty()) throw std::runtime_error("ctool::num::max: Empty array.");
            return *std::max_element(arr.begin(), arr.end());
        }

        template <typename T>
        T min(const ArrayND<T>& arr) {
            if (arr.empty()) throw std::runtime_error("ctool::num::min: Empty array.");
            return *std::min_element(arr.begin(), arr.end());
        }

        // --- Stream Output ---
        template <typename T>
        std::ostream& operator<<(std::ostream& os, const ArrayND<T>& arr) {
            os << "ArrayND(\n";
            for (size_t i = 0; i < arr.rows(); ++i) {
                os << "  [";
                for (size_t j = 0; j < arr.cols(); ++j) {
                    os << arr(i, j) << (j + 1 < arr.cols() ? ", " : "");
                }
                os << "]\n";
            }
            os << ")";
            return os;
        }

    }    // namespace num
}    // namespace ctool