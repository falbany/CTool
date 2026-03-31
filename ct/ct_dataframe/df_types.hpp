#ifndef CT_DF_TYPES_HPP
#define CT_DF_TYPES_HPP

#include <stdexcept>
#include <string>
#include <vector>

namespace ct {
    namespace data {

        /**
         * @brief Universal data cell type.
         * Custom implementation for C++11 (replaces std::variant).
         */
        class Cell {
          public:
            enum Type { EMPTY, DOUBLE, INT, STRING };

            Cell() : m_type(EMPTY), m_d(0.0), m_i(0) {}
            Cell(double v) : m_type(DOUBLE), m_d(v), m_i(0) {}
            Cell(int v) : m_type(INT), m_d(0.0), m_i(v) {}
            Cell(const std::string& v) : m_type(STRING), m_d(0.0), m_i(0), m_s(v) {}
            Cell(const char* v) : m_type(STRING), m_d(0.0), m_i(0), m_s(v ? v : "") {}

            Type type() const { return m_type; }

            double asDouble() const {
                if (m_type == DOUBLE) return m_d;
                if (m_type == INT) return static_cast<double>(m_i);
                throw std::runtime_error("ct::data::Cell: Type mismatch. Expected numeric (DOUBLE or INT).");
            }

            int asInt() const {
                if (m_type == INT) return m_i;
                if (m_type == DOUBLE) return static_cast<int>(m_d);
                throw std::runtime_error("ct::data::Cell: Type mismatch. Expected INT (or DOUBLE).");
            }

            const std::string& asString() const {
                if (m_type == STRING) {
                    return m_s;
                }
                throw std::runtime_error("ct::data::Cell: Type mismatch. Expected STRING.");
            }

            bool operator==(const Cell& other) const {
                if (m_type != other.m_type) return false;
                switch (m_type) {
                    case DOUBLE: return m_d == other.m_d;
                    case INT:    return m_i == other.m_i;
                    case STRING: return m_s == other.m_s;
                    default:     return true;
                }
            }

            bool operator!=(const Cell& other) const { return !(*this == other); }

          private:
            Type        m_type;
            double      m_d;
            int         m_i;
            std::string m_s;
        };

        /** @brief Returns true if the cell is empty/null. */
        inline bool isNull(const Cell& val) { return val.type() == Cell::EMPTY; }

    }    // namespace data
}    // namespace ct
#endif    // CT_DF_TYPES_HPP
