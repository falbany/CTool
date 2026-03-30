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

    }    // namespace math
}    // namespace ct