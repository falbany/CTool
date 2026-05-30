/**
 * @file math.tpp
 * @brief Template implementations for ctool_math.hpp statistical functions.
 * @author Florent ALBANY - FAL
 * @version 1.0
 * @date 2026-03-18
 */

#pragma once

#include <cmath>
#include <algorithm>
#include <stdexcept> 

namespace ctool {
    namespace math {
        template <typename T>
        T clamp(T val, T min, T max) {
            if (val < min) {
                return min;
            }
            if (val > max) {
                return max;
            }
            return val;
        }

        template <typename T>
        T median(const std::vector<T>& data) {
            if (data.empty()) {
                return T(0);
            }

            // Create a copy to sort without modifying the original const reference
            std::vector<T> sortedData = data;
            size_t         dataSize   = sortedData.size();

            // Sort the data
            std::sort(sortedData.begin(), sortedData.end());

            if (dataSize % 2 == 0) {
                // Even number of elements: average of the two middle ones
                T mid1 = sortedData[dataSize / 2 - 1];
                T mid2 = sortedData[dataSize / 2];

                // Use double for intermediate calculation to ensure precision for integer inputs
                // unless the user specifically wants integer division behavior (unlikely for median).
                // However, to return T, we cast back. If T is int, this truncates.
                // A more robust approach for integer types is to return double,
                // but the signature requires T. We will return the average as T.

                // Note: If T is integral, (mid1 + mid2) / 2 truncates.
                // If exact median is needed for integers, the caller should use double inputs.
                const double median_computed = (static_cast<double>(mid1) + static_cast<double>(mid2)) / 2.0;
                return static_cast<T>(median_computed);
            }
            
            // Odd number of elements: the middle one
            return sortedData[dataSize / 2];
        }

        template <typename T>
        T sum(const std::vector<T>& inputData) {
            double accumulator = 0.0;
            for (const auto& val : inputData) {
                accumulator += static_cast<double>(val);
            }
            return static_cast<T>(accumulator);
        }

        template <typename T>
        double percentile(const std::vector<T>& inputData, double percentileK) {
            if (inputData.empty()) {
                return 0.0;
            }
            if (percentileK < 0.0 || percentileK > 100.0) {
                throw std::invalid_argument("Percentile k must be between 0.0 and 100.0.");
            }

            if (inputData.size() == 1) {
                return static_cast<double>(inputData[0]);
            }

            std::vector<double> sortedData(inputData.begin(), inputData.end());
            std::sort(sortedData.begin(), sortedData.end());

            size_t dataSize = sortedData.size();

            // Linear interpolation method (similar to NumPy 'linear')
            // Rank = (percentileK / 100) * (dataSize - 1)
            double rank = (percentileK / 100.0) * (dataSize - 1.0);

            size_t lowerIdx = static_cast<size_t>(std::floor(rank));
            size_t upperIdx = static_cast<size_t>(std::ceil(rank));

            double fraction = rank - static_cast<double>(lowerIdx);

            if (lowerIdx == upperIdx) {
                return sortedData[lowerIdx];
            }

            return sortedData[lowerIdx] * (1.0 - fraction) + sortedData[upperIdx] * fraction;
        }

        template <typename T>
        std::vector<double> minMaxScale(const std::vector<T>& inputData) {
            std::vector<double> result;
            result.reserve(inputData.size());

            if (inputData.empty()) {
                return result;
            }

            // Find min and max
            auto minValue = static_cast<double>(inputData[0]);
            auto maxValue = static_cast<double>(inputData[0]);

            for (const auto& val : inputData) {
                double v = static_cast<double>(val);
                if (v < minValue) {
                    minValue = v;
                } else if (v > maxValue) {
                    maxValue = v;
                }
            }

            double range = maxValue - minValue;

            if (range == 0.0) {
                // If all values are identical, return 0.0 (or 0.5 depending on preference)
                // Returning 0.0 to indicate "no variation".
                result.assign(inputData.size(), 0.0);
                return result;
            }

            for (const auto& val : inputData) {
                double v = static_cast<double>(val);
                result.push_back((v - minValue) / range);
            }

            return result;
        }

    }    // namespace math
}    // namespace ctool