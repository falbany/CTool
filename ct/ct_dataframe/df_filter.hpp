/**
 * @file df_filter.hpp
 * @author Florent ALBANY (FAL)
 * @brief Filtering utilities for DataFrame.
 * @version 1.0
 * @date 2026-03-24
 */

#ifndef CT_DF_FILTER_HPP
#define CT_DF_FILTER_HPP

#include "df_core.hpp"

namespace ct {
    namespace data {

        enum class CompareOp { EQ, NE, LT, LE, GT, GE };

        /** @brief Returns a new DataFrame containing rows where the column matches val. */
        DataFrame filterEq(const DataFrame& df, const std::string& col, const Cell& val);

        /** @brief Returns a new DataFrame containing rows where the column does NOT match val. */
        DataFrame filterNe(const DataFrame& df, const std::string& col, const Cell& val);

        /** @brief Returns a new DataFrame where the column value > val (numeric only). */
        DataFrame filterGt(const DataFrame& df, const std::string& col, double val);

        /** @brief Returns a new DataFrame where the column value < val (numeric only). */
        DataFrame filterLt(const DataFrame& df, const std::string& col, double val);

        /** @brief Returns a new DataFrame where the column value >= val (numeric only). */
        DataFrame filterGe(const DataFrame& df, const std::string& col, double val);

        /** @brief Returns a new DataFrame where the column value <= val (numeric only). */
        DataFrame filterLe(const DataFrame& df, const std::string& col, double val);

        /** @brief Generic filter using CompareOp enum. */
        DataFrame filter(const DataFrame& df, const std::string& col, const Cell& val, CompareOp op);

    }    // namespace data
}    // namespace ct

#include "df_filter.tpp"
#endif    // CT_DF_FILTER_HPP