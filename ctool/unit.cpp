#include "unit.hpp"
#include "string.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <map>

namespace ctool {
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

            std::string trimmed = ctool::str::trim(input);
            if (trimmed.empty()) return 0.0;

            try {
                size_t pos   = 0;
                double value = std::stod(trimmed, &pos);

                // Extract the part after the numeric value (e.g., "mV", " kOhm", "uA")
                std::string suffix = ctool::str::trim(trimmed.substr(pos));

                if (!suffix.empty()) {
                    char pChar = suffix[0];
                    for (const auto& p : prefixes) {
                        if (p.symbol != ' ' && p.symbol == pChar) {
                            return value * p.factor;
                        }
                    }
                }

                return value;
            } catch (...) {
                return 0.0;
            }
        }

    }    // namespace unit
}    // namespace ctool