/**
 * @file df_dataframe.hpp
 * @author Florent ALBANY (FAL)
 * @brief Centralized entry point for the DataFrame module.
 * @version 1.0
 * @date 2026-03-24
 */

#pragma once

#ifndef CT_DF_DATAFRAME_HPP
    #define CT_DF_DATAFRAME_HPP

    /**
    * This header groups all components of the DataFrame feature:
    * - Core structures (DataFrame, Column, Cell)
    * - Filtering engine (filterEq, filterGt, etc.)
    * - I/O utilities (CSV export, Array2D integration, Pretty Print)
    */

    #include "ct_dataframe/df_types.hpp"
    #include "ct_dataframe/df_core.hpp"
    #include "ct_dataframe/df_filter.hpp"
    #include "ct_dataframe/df_io.hpp"

#endif    // CT_DF_DATAFRAME_HPP