#include "math.hpp"
#include <numeric>
#include <algorithm>
#include <stdexcept>

namespace ctool {
    namespace math {

        RegResult linearFit(const std::vector<double>& inputX, const std::vector<double>& inputY) {
            size_t dataSize = inputX.size();
            if (dataSize != inputY.size() || dataSize < 2) {
                return {0.0, 0.0, 0.0};
            }

            double sX  = 0;
            double sY  = 0;
            double sXX = 0;
            double sYY = 0;
            double sXY = 0;
            for (size_t idx = 0; idx < dataSize; ++idx) {
                sX += inputX[idx];
                sY += inputY[idx];
                sXX += inputX[idx] * inputX[idx];
                sYY += inputY[idx] * inputY[idx];
                sXY += inputX[idx] * inputY[idx];
            }

            double denom = ((dataSize * sXX) - (sX * sX));
            if (std::abs(denom) < 1e-20) {
                return {0.0, 0.0, 0.0};
            }

            RegResult result;
            result.slope     = ((dataSize * sXY) - (sX * sY)) / denom;
            result.intercept = (sY - (result.slope * sX)) / dataSize;

            // Calculate R-Squared
            double ssTot = sYY - ((sY * sY) / dataSize);
            double ssRes = 0;
            for (size_t idx = 0; idx < dataSize; ++idx) {
                double fit = (result.slope * inputX[idx]) + result.intercept;
                ssRes += (inputY[idx] - fit) * (inputY[idx] - fit);
            }
            result.rSquared = (ssTot > 0) ? 1.0 - (ssRes / ssTot) : 1.0;

            return result;
        }

        SigmaBounds calculateSigmaBounds(const std::vector<double>& inputValues, double sigmaMultiplier) {
            if (inputValues.size() <= 1) {
                return {0.0, 0.0};
            }

            double avg    = mean(inputValues);
            double stdDev = standardDeviation(inputValues);

            SigmaBounds bounds;
            bounds.lowerBound = avg - (sigmaMultiplier * stdDev);
            bounds.upperBound = avg + (sigmaMultiplier * stdDev);

            return bounds;
        }

        RegResult logLinearFit(const std::vector<double>& xValues, const std::vector<double>& yValues) {
            size_t dataSize = xValues.size();
            if (dataSize != yValues.size() || dataSize < 2) {
                return {0.0, 0.0, 0.0};
            }

            // Check for non-positive y values
            for (size_t idx = 0; idx < dataSize; ++idx) {
                if (yValues[idx] <= 0.0) {
                    throw std::invalid_argument("logLinearFit requires all y values to be strictly positive.");
                }
            }

            double sX     = 0;
            double sYlog  = 0;
            double sXX    = 0;
            double sYYlog = 0;
            double sXYlog = 0;
            for (size_t idx = 0; idx < dataSize; ++idx) {
                double yLog = std::log(yValues[idx]);
                sX += xValues[idx];
                sYlog += yLog;
                sXX += xValues[idx] * xValues[idx];
                sYYlog += yLog * yLog;
                sXYlog += xValues[idx] * yLog;
            }

            double denom = ((dataSize * sXX) - (sX * sX));
            if (std::abs(denom) < 1e-20) {
                return {0.0, 0.0, 0.0};
            }

            RegResult result;
            result.slope     = ((dataSize * sXYlog) - (sX * sYlog)) / denom;    // This is B
            result.intercept = (sYlog - (result.slope * sX)) / dataSize;        // This is ln(A)

            // Calculate R-Squared on the log-transformed data
            double ssTot = sYYlog - ((sYlog * sYlog) / dataSize);
            double ssRes = 0;
            for (size_t idx = 0; idx < dataSize; ++idx) {
                double fit = (result.slope * xValues[idx]) + result.intercept;
                ssRes += (std::log(yValues[idx]) - fit) * (std::log(yValues[idx]) - fit);
            }
            result.rSquared = (ssTot > 0) ? 1.0 - (ssRes / ssTot) : 1.0;

            return result;
        }

        /**
         * @brief Calculates residuals for a linear model.
         */
        std::vector<double> residuals(const std::vector<double>& xValues, const std::vector<double>& yValues, double slope, double intercept) {
            std::vector<double> result;
            size_t              dataSize = xValues.size();
            if (dataSize != yValues.size()) {
                throw std::invalid_argument("Residuals: x and y vectors must be of equal size.");
            }

            result.reserve(dataSize);
            for (size_t idx = 0; idx < dataSize; ++idx) {
                double predicted = (slope * xValues[idx]) + intercept;
                result.push_back(yValues[idx] - predicted);
            }
            return result;
        }

        /**
         * @brief Helper: Gaussian elimination for small matrices (degree <= 4).
         */
        static std::vector<double> solveLinearSystem(std::vector<std::vector<double>> matrixA, std::vector<double> vectorB) {
            size_t matrixSize = matrixA.size();
            // Forward elimination
            for (size_t i = 0; i < matrixSize; ++i) {
                // Pivot
                size_t maxRow = i;
                for (size_t k = i + 1; k < matrixSize; ++k) {
                    if (std::abs(matrixA[k][i]) > std::abs(matrixA[maxRow][i])) {
                        maxRow = k;
                    }
                }
                std::swap(matrixA[i], matrixA[maxRow]);
                std::swap(vectorB[i], vectorB[maxRow]);

                if (std::abs(matrixA[i][i]) < 1e-20) continue;    // Singular matrix

                for (size_t k = i + 1; k < matrixSize; ++k) {
                    double factor = matrixA[k][i] / matrixA[i][i];
                    for (size_t j = i; j < matrixSize; ++j) {
                        matrixA[k][j] -= factor * matrixA[i][j];
                    }
                    vectorB[k] -= factor * vectorB[i];
                }
            }

            // Back substitution
            std::vector<double> resultVec(matrixSize, 0.0);
            for (int i = static_cast<int>(matrixSize) - 1; i >= 0; --i) {
                double sum = 0.0;
                for (size_t j = i + 1; j < matrixSize; ++j) {
                    sum += matrixA[i][j] * resultVec[j];
                }
                if (std::abs(matrixA[i][i]) > 1e-20) {
                    resultVec[i] = (vectorB[i] - sum) / matrixA[i][i];
                }
            }
            return resultVec;
        }

        /**
         * @brief Fits a polynomial of degree n.
         */
        std::vector<double> polynomialFit(const std::vector<double>& xValues, const std::vector<double>& yValues, int degree) {
            if (degree < 1 || degree > 4) {
                throw std::invalid_argument("polynomialFit: Degree must be between 1 and 4.");
            }
            size_t dataSize = xValues.size();
            if (dataSize != yValues.size() || dataSize < static_cast<size_t>(degree) + 1) {
                return {};    // Not enough data points
            }

            // Build the Normal Equation system: (X^T X) * beta = X^T y
            // Matrix size: (degree+1) x (degree+1)
            size_t                           m = static_cast<size_t>(degree) + 1;
            std::vector<std::vector<double>> A(m, std::vector<double>(m, 0.0));
            std::vector<double>              b(m, 0.0);

            // Compute sums of powers
            for (size_t idx = 0; idx < dataSize; ++idx) {
                double              xVal = xValues[idx];
                double              yVal = yValues[idx];
                std::vector<double> xPowers(m);
                xPowers[0] = 1.0;
                for (size_t j = 1; j < m; ++j) {
                    xPowers[j] = xPowers[j - 1] * xVal;
                }

                for (size_t i = 0; i < m; ++i) {
                    b[i] += xPowers[i] * yVal;
                    for (size_t j = 0; j < m; ++j) {
                        A[i][j] += xPowers[i] * xPowers[j];
                    }
                }
            }

            // Solve
            return solveLinearSystem(A, b);
        }


        double min(const std::vector<double>& inputData) {
            if (inputData.empty()) {
                return 0.0;
            }
            return *std::min_element(inputData.begin(), inputData.end());
        }

        double max(const std::vector<double>& inputData) {
            if (inputData.empty()) {
                return 0.0;
            }
            return *std::max_element(inputData.begin(), inputData.end());
        }

        double mean(const std::vector<double>& inputData) {
            if (inputData.empty()) {
                return 0.0;
            }
            return std::accumulate(inputData.begin(), inputData.end(), 0.0) / inputData.size();
        }

        double variance(const std::vector<double>& inputData) {
            size_t dataSize = inputData.size();
            if (dataSize < 2) {
                return 0.0;    // Variance is undefined for single value or empty
            }

            double avg   = mean(inputData);
            double sqSum = 0.0;
            for (double val : inputData) {
                sqSum += (val - avg) * (val - avg);
            }
            // Population variance (n) or Sample variance (n-1)?
            // Standard deviation used n here, so we stay consistent.
            return sqSum / dataSize;
        }

        double standardDeviation(const std::vector<double>& inputData) {
            if (inputData.size() < 2) {
                return 0.0;
            }
            double avg   = mean(inputData);
            double sqSum = 0.0;
            for (double val : inputData) {
                sqSum += (val - avg) * (val - avg);
            }
            return std::sqrt(sqSum / inputData.size());
        }

        bool isNear(double firstValue, double secondValue, double epsilon) { return std::abs(firstValue - secondValue) <= epsilon; }

    }    // namespace math
}    // namespace ctool