/**
 * @file df_types.hpp
 * @author Florent ALBANY (FAL)
 * @brief Core types for the DataFrame module.
 * @version 1.0
 * @date 2026-03-24
 */

#ifndef CT_DF_TYPES_HPP
#define CT_DF_TYPES_HPP

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

    double asDouble() const { return m_type == DOUBLE ? m_d : (m_type == INT ? static_cast<double>(m_i) : 0.0); }
    int asInt() const { return m_type == INT ? m_i : (m_type == DOUBLE ? static_cast<int>(m_d) : 0); }
    const std::string& asString() const { return m_s; }

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
    Type m_type;
    double m_d;
    int m_i;
    std::string m_s;
};

/** @brief Returns true if the cell is empty/null. */
inline bool isNull(const Cell& val) { return val.type() == Cell::EMPTY; }

} // namespace data
} // namespace ct
#endif // CT_DF_TYPES_HPP