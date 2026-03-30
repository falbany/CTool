#include "ct_math.hpp"
#include <numeric>
#include <algorithm>
#include <stdexcept>

namespace ct {
    namespace math {

        RegResult linearFit(const std::vector<double>& x, const std::vector<double>& y) {
            size_t n = x.size();
            if (n != y.size() || n < 2) return {0.0, 0.0, 0.0};

            double sX = 0, sY = 0, sXX = 0, sYY = 0, sXY = 0;
            for (size_t i = 0; i < n; ++i) {
                sX += x[i];
                sY += y[i];
                sXX += x[i] * x[i];
                sYY += y[i] * y[i];
                sXY += x[i] * y[i];
            }

            double denom = (n * sXX - sX * sX);
            if (std::abs(denom) < 1e-20) return {0.0, 0.0, 0.0};

            RegResult res;
            res.slope     = (n * sXY - sX * sY) / denom;
            res.intercept = (sY - res.slope * sX) / n;

            // Calculate R-Squared
            double ssTot = sYY - (sY * sY) / n;
            double ssRes = 0;
            for (size_t i = 0; i < n; ++i) {
                double fit = res.slope * x[i] + res.intercept;
                ssRes += (y[i] - fit) * (y[i] - fit);
            }
            res.rSquared = (ssTot > 0) ? 1.0 - (ssRes / ssTot) : 1.0;

            return res;
        }

        RegResult logLinearFit(const std::vector<double>& x, const std::vector<double>& y) {
            size_t n = x.size();
            if (n != y.size() || n < 2) return {0.0, 0.0, 0.0};

            // Check for non-positive y values
            for (size_t i = 0; i < n; ++i) {
                if (y[i] <= 0.0) {
                    throw std::invalid_argument("logLinearFit requires all y values to be strictly positive.");
                }
            }

            double sX = 0, sYlog = 0, sXX = 0, sYYlog = 0, sXYlog = 0;
            for (size_t i = 0; i < n; ++i) {
                double yLog = std::log(y[i]);
                sX += x[i];
                sYlog += yLog;
                sXX += x[i] * x[i];
                sYYlog += yLog * yLog;
                sXYlog += x[i] * yLog;
            }

            double denom = (n * sXX - sX * sX);
            if (std::abs(denom) < 1e-20) return {0.0, 0.0, 0.0};

            RegResult res;
            res.slope     = (n * sXYlog - sX * sYlog) / denom;    // This is B
            res.intercept = (sYlog - res.slope * sX) / n;         // This is ln(A)

            // Calculate R-Squared on the log-transformed data
            double ssTot = sYYlog - (sYlog * sYlog) / n;
            double ssRes = 0;
            for (size_t i = 0; i < n; ++i) {
                double fit = res.slope * x[i] + res.intercept;
                ssRes += (std::log(y[i]) - fit) * (std::log(y[i]) - fit);
            }
            res.rSquared = (ssTot > 0) ? 1.0 - (ssRes / ssTot) : 1.0;

            return res;
        }

        /**
         * @brief Calculates residuals for a linear model.
         */
        std::vector<double> residuals(const std::vector<double>& x, const std::vector<double>& y, double slope, double intercept) {
            std::vector<double> resVec;
            size_t              n = x.size();
            if (n != y.size()) {
                throw std::invalid_argument("Residuals: x and y vectors must be of equal size.");
            }

            resVec.reserve(n);
            for (size_t i = 0; i < n; ++i) {
                double predicted = slope * x[i] + intercept;
                resVec.push_back(y[i] - predicted);
            }
            return resVec;
        }

        /**
         * @brief Helper: Gaussian elimination for small matrices (degree <= 4).
         */
        static std::vector<double> solveLinearSystem(std::vector<std::vector<double>> A, std::vector<double> b) {
            size_t n = A.size();
            // Forward elimination
            for (size_t i = 0; i < n; ++i) {
                // Pivot
                size_t maxRow = i;
                for (size_t k = i + 1; k < n; ++k) {
                    if (std::abs(A[k][i]) > std::abs(A[maxRow][i])) {
                        maxRow = k;
                    }
                }
                std::swap(A[i], A[maxRow]);
                std::swap(b[i], b[maxRow]);

                if (std::abs(A[i][i]) < 1e-20) continue;    // Singular matrix

                for (size_t k = i + 1; k < n; ++k) {
                    double factor = A[k][i] / A[i][i];
                    for (size_t j = i; j < n; ++j) {
                        A[k][j] -= factor * A[i][j];
                    }
                    b[k] -= factor * b[i];
                }
            }

            // Back substitution
            std::vector<double> x(n, 0.0);
            for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
                double sum = 0.0;
                for (size_t j = i + 1; j < n; ++j) {
                    sum += A[i][j] * x[j];
                }
                if (std::abs(A[i][i]) > 1e-20) {
                    x[i] = (b[i] - sum) / A[i][i];
                }
            }
            return x;
        }

        /**
         * @brief Fits a polynomial of degree n.
         */
        std::vector<double> polynomialFit(const std::vector<double>& x, const std::vector<double>& y, int degree) {
            if (degree < 1 || degree > 4) {
                throw std::invalid_argument("polynomialFit: Degree must be between 1 and 4.");
            }
            size_t n = x.size();
            if (n != y.size() || n < static_cast<size_t>(degree) + 1) {
                return {};    // Not enough data points
            }

            // Build the Normal Equation system: (X^T X) * beta = X^T y
            // Matrix size: (degree+1) x (degree+1)
            size_t                           m = static_cast<size_t>(degree) + 1;
            std::vector<std::vector<double>> A(m, std::vector<double>(m, 0.0));
            std::vector<double>              b(m, 0.0);

            // Compute sums of powers
            for (size_t k = 0; k < n; ++k) {
                double              xVal = x[k];
                double              yVal = y[k];
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


        double min(const std::vector<double>& data) {
            if (data.empty()) return 0.0;
            return *std::min_element(data.begin(), data.end());
        }

        double max(const std::vector<double>& data) {
            if (data.empty()) return 0.0;
            return *std::max_element(data.begin(), data.end());
        }

        double mean(const std::vector<double>& data) {
            if (data.empty()) return 0.0;
            return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
        }

        double variance(const std::vector<double>& data) {
            size_t n = data.size();
            if (n < 2) return 0.0;    // Variance is undefined for single value or empty

            double avg    = mean(data);
            double sq_sum = 0.0;
            for (double v : data) {
                sq_sum += (v - avg) * (v - avg);
            }
            // Population variance (n) or Sample variance (n-1)?
            // Standard deviation used n here, so we stay consistent.
            return sq_sum / n;
        }

        double standardDeviation(const std::vector<double>& data) {
            if (data.size() < 2) return 0.0;
            double avg    = mean(data);
            double sq_sum = 0.0;
            for (double v : data) sq_sum += (v - avg) * (v - avg);
            return std::sqrt(sq_sum / data.size());
        }

        bool isNear(double a, double b, double epsilon) { return std::abs(a - b) <= epsilon; }

    }    // namespace math
}    // namespace ct