/**
 * @file df_column.hpp
 * @author Florent ALBANY (FAL)
 * @brief Column container for DataFrame.
 * @version 1.0
 * @date 2026-03-24
 */

#ifndef CT_DF_COLUMN_HPP
#define CT_DF_COLUMN_HPP

#include "df_types.hpp"

namespace ct {
    namespace data {

        /**
         * @brief Represents a single named column of data.
         */
        class Column {
          public:
            std::string       name;
            std::vector<Cell> data;

            void        push(const Cell& val) { data.push_back(val); }
            size_t      size() const { return data.size(); }
            Cell&       at(size_t idx) { return data.at(idx); }
            const Cell& at(size_t idx) const { return data.at(idx); }
        };

    }    // namespace data
}    // namespace ct
#endif    // CT_DF_COLUMN_HPP