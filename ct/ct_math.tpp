/**
 * @file ct_math.tpp
 * @brief Template implementations for ct_math.hpp statistical functions.
 * @author Florent ALBANY - FAL
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#include <cmath>
#include <algorithm>
#include <stdexcept> 

namespace ct {
    namespace math {
        template <typename T>
        T clamp(T val, T min, T max) {
            if (val < min) return min;
            if (val > max) return max;
            return val;
        }

        template <typename T>
        T median(const std::vector<T>& data) {
            if (data.empty()) {
                return T(0);
            }

            // Create a copy to sort without modifying the original const reference
            std::vector<T> sortedData = data;
            size_t         n          = sortedData.size();

            // Sort the data
            std::sort(sortedData.begin(), sortedData.end());

            if (n % 2 == 0) {
                // Even number of elements: average of the two middle ones
                T mid1 = sortedData[n / 2 - 1];
                T mid2 = sortedData[n / 2];

                // Use double for intermediate calculation to ensure precision for integer inputs
                // unless the user specifically wants integer division behavior (unlikely for median).
                // However, to return T, we cast back. If T is int, this truncates.
                // A more robust approach for integer types is to return double,
                // but the signature requires T. We will return the average as T.

                // Note: If T is integral, (mid1 + mid2) / 2 truncates.
                // If exact median is needed for integers, the caller should use double inputs.
                return static_cast<T>((static_cast<double>(mid1) + static_cast<double>(mid2)) / 2.0);
            } else {
                // Odd number of elements: the middle one
                return sortedData[n / 2];
            }
        }

        template <typename T>
        T sum(const std::vector<T>& data) {
            double accumulator = 0.0;
            for (const auto& val : data) {
                accumulator += static_cast<double>(val);
            }
            return static_cast<T>(accumulator);
        }

        template <typename T>
        double percentile(const std::vector<T>& data, double k) {
            if (data.empty()) {
                return 0.0;
            }
            if (k < 0.0 || k > 100.0) {
                throw std::invalid_argument("Percentile k must be between 0.0 and 100.0.");
            }

            if (data.size() == 1) {
                return static_cast<double>(data[0]);
            }

            std::vector<double> sortedData(data.begin(), data.end());
            std::sort(sortedData.begin(), sortedData.end());

            size_t n = sortedData.size();

            // Linear interpolation method (similar to NumPy 'linear')
            // Rank = (k / 100) * (n - 1)
            double rank = (k / 100.0) * (n - 1.0);

            size_t lower_idx = static_cast<size_t>(std::floor(rank));
            size_t upper_idx = static_cast<size_t>(std::ceil(rank));

            double fraction = rank - static_cast<double>(lower_idx);

            if (lower_idx == upper_idx) {
                return sortedData[lower_idx];
            }

            return sortedData[lower_idx] * (1.0 - fraction) + sortedData[upper_idx] * fraction;
        }

        template <typename T>
        std::vector<double> minMaxScale(const std::vector<T>& data) {
            std::vector<double> result;
            result.reserve(data.size());

            if (data.empty()) {
                return result;
            }

            // Find min and max
            double val_min = static_cast<double>(data[0]);
            double val_max = static_cast<double>(data[0]);

            for (const auto& val : data) {
                double v = static_cast<double>(val);
                if (v < val_min) val_min = v;
                if (v > val_max) val_max = v;
            }

            double range = val_max - val_min;

            if (range == 0.0) {
                // If all values are identical, return 0.0 (or 0.5 depending on preference)
                // Returning 0.0 to indicate "no variation".
                result.assign(data.size(), 0.0);
            } else {
                for (const auto& val : data) {
                    double v = static_cast<double>(val);
                    result.push_back((v - val_min) / range);
                }
            }

            return result;
        }

    }    // namespace math
}    // namespace ct