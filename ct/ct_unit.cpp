#include "ct_unit.hpp"
#include "ct_str.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <map>

namespace ct {
    namespace unit {

        struct Prefix {
            char   symbol;
            double factor;
        };

        static const Prefix prefixes[] = {{'G', 1e9}, {'M', 1e6}, {'k', 1e3}, {' ', 1.0}, {'m', 1e-3}, {'u', 1e-6}, {'n', 1e-9}, {'p', 1e-12}, {'f', 1e-15}};

        std::string format(double value, int precision, const std::string& unit) {
            if (value == 0.0) return "0 " + unit;

            double        absVal = std::abs(value);
            const Prefix* best   = &prefixes[3];    // Default to no prefix

            for (const auto& p : prefixes) {
                if (absVal >= p.factor) {
                    best = &p;
                    break;
                }
            }

            std::ostringstream oss;
            oss << std::fixed << std::setprecision(precision) << (value / best->factor);
            if (best->symbol != ' ') oss << " " << best->symbol;
            if (!unit.empty()) oss << (best->symbol == ' ' ? " " : "") << unit;

            return oss.str();
        }

        double parse(const std::string& input) {
            if (input.empty()) return 0.0;

            std::string trimmed    = ct::str::trim(input);
            char        last       = trimmed.back();
            double      multiplier = 1.0;

            for (const auto& p : prefixes) {
                if (p.symbol != ' ' && p.symbol == last) {
                    multiplier = p.factor;
                    trimmed.pop_back();
                    break;
                }
            }

            try {
                return std::stod(trimmed) * multiplier;
            } catch (...) {
                return 0.0;
            }
        }

    }    // namespace unit
}    // namespace ct