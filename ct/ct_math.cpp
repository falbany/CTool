#include "ct_math.hpp"
#include <numeric>
#include <algorithm>

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

        double mean(const std::vector<double>& data) {
            if (data.empty()) return 0.0;
            return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
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