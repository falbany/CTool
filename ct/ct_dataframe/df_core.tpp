/**
 * @file df_core.tpp
 * @author Florent ALBANY (FAL)
 * @brief Implementation of DataFrame core methods.
 */

#include <stdexcept>

namespace ct {
    namespace data {

        inline void DataFrame::addColumn(const std::string& name) { m_columns.push_back({name, {}}); }

        inline void DataFrame::pushRow(const std::vector<Cell>& row) {
            if (row.size() != m_columns.size()) {
                throw std::invalid_argument("DataFrame::pushRow - row size does not match column count.");
            }
            for (size_t i = 0; i < row.size(); ++i) {
                m_columns[i].push(row[i]);
            }
        }

        inline void DataFrame::clear() { m_columns.clear(); }

        inline size_t DataFrame::rows() const { return m_columns.empty() ? 0 : m_columns[0].size(); }

        inline std::vector<std::string> DataFrame::columnNames() const {
            std::vector<std::string> names;
            for (const auto& col : m_columns) names.push_back(col.name);
            return names;
        }

        inline const Cell& DataFrame::at(size_t row, const std::string& colName) const {
            int idx = findIndex(colName);
            if (idx == -1) throw std::out_of_range("Column not found: " + colName);
            return m_columns[idx].at(row);
        }

        inline std::vector<Cell> DataFrame::getColumn(const std::string& name) const {
            int idx = findIndex(name);
            if (idx == -1) throw std::out_of_range("Column not found: " + name);
            return m_columns[idx].data;
        }

        inline std::vector<Cell> DataFrame::getRow(size_t index) const {
            if (index >= rows()) throw std::out_of_range("DataFrame::getRow: Index out of bounds.");
            std::vector<Cell> row;
            row.reserve(m_columns.size());
            for (const auto& col : m_columns) {
                row.push_back(col.data[index]);
            }
            return row;
        }

        inline DataFrame DataFrame::head(size_t n) const {
            DataFrame res;
            auto names = columnNames();
            for (const auto& name : names) res.addColumn(name);
            size_t limit = (n < rows()) ? n : rows();
            for (size_t i = 0; i < limit; ++i) res.pushRow(getRow(i));
            return res;
        }

        inline int DataFrame::findIndex(const std::string& name) const {
            for (size_t i = 0; i < m_columns.size(); ++i) {
                if (m_columns[i].name == name) return static_cast<int>(i);
            }
            return -1;
        }

    }    // namespace data
}    // namespace ct