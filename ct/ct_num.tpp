/**
 * @file ct_num.tpp
 * @author Florent ALBANY - FAL
 * @brief Template implementations for ct_num.hpp.
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#include <numeric>

namespace ct {
    namespace num {

        // --- Constructors ---
        template <typename T>
        NumArray<T>::NumArray() : m_rows(0), m_cols(0) {}

        template <typename T>
        NumArray<T>::NumArray(size_t rows, size_t cols) : m_data(rows * cols, T()), m_rows(rows), m_cols(cols) {}

        template <typename T>
        NumArray<T>::NumArray(const std::initializer_list<std::initializer_list<T>>& list) {
            if (list.size() == 0) {
                m_rows = 0;
                m_cols = 0;
                return;
            }

            m_rows = list.size();
            m_cols = list.begin()->size();
            m_data.reserve(m_rows * m_cols);

            size_t r_idx = 0;
            for (const auto& row : list) {
                if (row.size() != m_cols) {
                    throw std::invalid_argument("ct::num::NumArray: Jagged arrays are not supported.");
                }
                for (const auto& val : row) {
                    m_data.push_back(val);
                }
                r_idx++;
            }
        }

        // --- Size & Shape ---
        template <typename T>
        size_t NumArray<T>::rows() const noexcept {
            return m_rows;
        }

        template <typename T>
        size_t NumArray<T>::cols() const noexcept {
            return m_cols;
        }

        template <typename T>
        size_t NumArray<T>::size() const noexcept {
            return m_data.size();
        }

        template <typename T>
        std::pair<size_t, size_t> NumArray<T>::shape() const noexcept {
            return {m_rows, m_cols};
        }

        template <typename T>
        bool NumArray<T>::empty() const noexcept {
            return m_data.empty();
        }

        template <typename T>
        void NumArray<T>::clear() {
            m_data.clear();
            m_rows = 0;
            m_cols = 0;
        }

        // --- Access ---
        template <typename T>
        T& NumArray<T>::at(size_t r, size_t c) {
            if (r >= m_rows || c >= m_cols) throw std::out_of_range("ct::num::NumArray::at");
            return m_data[index(r, c)];
        }

        template <typename T>
        const T& NumArray<T>::at(size_t r, size_t c) const {
            if (r >= m_rows || c >= m_cols) throw std::out_of_range("ct::num::NumArray::at");
            return m_data[index(r, c)];
        }

        template <typename T>
        T& NumArray<T>::operator()(size_t r, size_t c) {
            return m_data[index(r, c)];
        }

        template <typename T>
        const T& NumArray<T>::operator()(size_t r, size_t c) const {
            return m_data[index(r, c)];
        }

        template <typename T>
        T* NumArray<T>::data() noexcept {
            return m_data.data();
        }

        template <typename T>
        const T* NumArray<T>::data() const noexcept {
            return m_data.data();
        }

        // --- Arithmetic Operators ---
        // Note: These operations optimize using the contiguous 1D vector.

        template <typename T>
        NumArray<T> operator+(const NumArray<T>& lhs, const NumArray<T>& rhs) {
            if (lhs.shape() != rhs.shape()) {
                throw std::invalid_argument("ct::num::operator+: Shapes do not match.");
            }
            NumArray<T> res = lhs;    // Copy LHS
            for (size_t i = 0; i < lhs.size(); ++i) {
                res.m_data[i] += rhs.m_data[i];
            }
            return res;
        }

        template <typename T>
        NumArray<T> operator+(const NumArray<T>& lhs, const T& scalar) {
            NumArray<T> res = lhs;
            for (auto& val : res.m_data) {
                val += scalar;
            }
            return res;
        }

        template <typename T>
        NumArray<T> operator+(const T& scalar, const NumArray<T>& rhs) {
            return rhs + scalar;    // Commutative
        }

        template <typename T>
        NumArray<T> operator-(const NumArray<T>& lhs, const NumArray<T>& rhs) {
            if (lhs.shape() != rhs.shape()) {
                throw std::invalid_argument("ct::num::operator-: Shapes do not match.");
            }
            NumArray<T> res = lhs;
            for (size_t i = 0; i < lhs.size(); ++i) {
                res.m_data[i] -= rhs.m_data[i];
            }
            return res;
        }

        template <typename T>
        NumArray<T> operator-(const NumArray<T>& lhs, const T& scalar) {
            NumArray<T> res = lhs;
            for (auto& val : res.m_data) {
                val -= scalar;
            }
            return res;
        }

        template <typename T>
        NumArray<T> operator*(const NumArray<T>& lhs, const NumArray<T>& rhs) {
            if (lhs.shape() != rhs.shape()) {
                throw std::invalid_argument("ct::num::operator*: Shapes do not match.");
            }
            NumArray<T> res = lhs;
            for (size_t i = 0; i < lhs.size(); ++i) {
                res.m_data[i] *= rhs.m_data[i];
            }
            return res;
        }

        template <typename T>
        NumArray<T> operator*(const NumArray<T>& lhs, const T& scalar) {
            NumArray<T> res = lhs;
            for (auto& val : res.m_data) {
                val *= scalar;
            }
            return res;
        }

        template <typename T>
        NumArray<T> operator/(const NumArray<T>& lhs, const T& scalar) {
            NumArray<T> res = lhs;
            for (auto& val : res.m_data) {
                val /= scalar;
            }
            return res;
        }

        // --- Universal Functions ---

        template <typename T>
        NumArray<double> sqrt(const NumArray<T>& arr) {
            NumArray<double> res(arr.rows(), arr.cols());
            for (size_t i = 0; i < arr.size(); ++i) {
                res.m_data[i] = static_cast<double>(std::sqrt(static_cast<double>(arr.data()[i])));
            }
            return res;
        }

        template <typename T>
        NumArray<double> sin(const NumArray<T>& arr) {
            NumArray<double> res(arr.rows(), arr.cols());
            for (size_t i = 0; i < arr.size(); ++i) {
                res.m_data[i] = static_cast<double>(std::sin(static_cast<double>(arr.data()[i])));
            }
            return res;
        }

        template <typename T>
        NumArray<T> abs(const NumArray<T>& arr) {
            NumArray<T> res(arr.rows(), arr.cols());
            for (size_t i = 0; i < arr.size(); ++i) {
                res.m_data[i] = static_cast<T>(std::abs(arr.data()[i]));
            }
            return res;
        }

        // --- Reductions ---

        template <typename T>
        double sum(const NumArray<T>& arr) {
            double result = 0.0;
            for (const auto& val : arr.m_data) {
                result += static_cast<double>(val);
            }
            return result;
        }

        template <typename T>
        double mean(const NumArray<T>& arr) {
            if (arr.empty()) return 0.0;
            return sum(arr) / static_cast<double>(arr.size());
        }

        template <typename T>
        T max(const NumArray<T>& arr) {
            if (arr.empty()) throw std::runtime_error("ct::num::max: Empty array.");
            return *std::max_element(arr.m_data.begin(), arr.m_data.end());
        }

        template <typename T>
        T min(const NumArray<T>& arr) {
            if (arr.empty()) throw std::runtime_error("ct::num::min: Empty array.");
            return *std::min_element(arr.m_data.begin(), arr.m_data.end());
        }

        // --- Stream Output ---
        template <typename T>
        std::ostream& operator<<(std::ostream& os, const NumArray<T>& arr) {
            os << "NumArray(\n";
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
}    // namespace ct