/**
 * @file df_filter.tpp
 * @author Florent ALBANY (FAL)
 * @brief Implementation of DataFrame filtering operations.
 */

namespace ct {
    namespace data {

        inline DataFrame filterEq(const DataFrame& df, const std::string& col, const Cell& val) {
            DataFrame result;
            auto      names = df.columnNames();
            for (const auto& n : names) result.addColumn(n);

            // Optimization: Find column indices once
            std::vector<std::string> colNames = df.columnNames();

            for (size_t r = 0; r < df.rows(); ++r) {
                if (df.at(r, col) == val) {
                    std::vector<Cell> row;
                    for (const auto& n : names) {
                        row.push_back(df.at(r, n));
                    }
                    result.pushRow(row);
                }
            }
            return result;
        }

        inline DataFrame filterNe(const DataFrame& df, const std::string& col, const Cell& val) {
            DataFrame result;
            auto      names = df.columnNames();
            for (const auto& n : names) result.addColumn(n);

            for (size_t r = 0; r < df.rows(); ++r) {
                const Cell& current = df.at(r, col);
                if (current != val) {
                    std::vector<Cell> row;
                    for (const auto& n : names) row.push_back(df.at(r, n));
                    result.pushRow(row);
                }
            }
            return result;
        }

        inline DataFrame filterGt(const DataFrame& df, const std::string& col, double val) {
            DataFrame result;
            auto      names = df.columnNames();
            for (const auto& n : names) result.addColumn(n);

            for (size_t r = 0; r < df.rows(); ++r) {
                const auto& cell  = df.at(r, col);
                bool        match = false;

                if (cell.type() == Cell::DOUBLE || cell.type() == Cell::INT) {
                    if (cell.asDouble() > val) match = true;
                }

                if (match) {
                    std::vector<Cell> rowData;
                    for (const auto& n : names) {
                        rowData.push_back(df.at(r, n));
                    }
                    result.pushRow(rowData);
                }
            }
            return result;
        }

        inline DataFrame filterLt(const DataFrame& df, const std::string& col, double val) {
            DataFrame result;
            auto      names = df.columnNames();
            for (const auto& n : names) result.addColumn(n);

            for (size_t r = 0; r < df.rows(); ++r) {
                const auto& cell  = df.at(r, col);
                bool        match = false;
                if (cell.type() == Cell::DOUBLE || cell.type() == Cell::INT) {
                    if (cell.asDouble() < val) match = true;
                }
                if (match) {
                    std::vector<Cell> rowData;
                    for (const auto& n : names) rowData.push_back(df.at(r, n));
                    result.pushRow(rowData);
                }
            }
            return result;
        }

        inline DataFrame filterGe(const DataFrame& df, const std::string& col, double val) {
            DataFrame result;
            auto      names = df.columnNames();
            for (const auto& n : names) result.addColumn(n);

            for (size_t r = 0; r < df.rows(); ++r) {
                const auto& cell  = df.at(r, col);
                bool        match = false;
                if (cell.type() == Cell::DOUBLE || cell.type() == Cell::INT) {
                    if (cell.asDouble() >= val) match = true;
                }
                if (match) {
                    std::vector<Cell> rowData;
                    for (const auto& n : names) rowData.push_back(df.at(r, n));
                    result.pushRow(rowData);
                }
            }
            return result;
        }

        inline DataFrame filterLe(const DataFrame& df, const std::string& col, double val) {
            DataFrame result;
            auto      names = df.columnNames();
            for (const auto& n : names) result.addColumn(n);

            for (size_t r = 0; r < df.rows(); ++r) {
                const auto& cell  = df.at(r, col);
                bool        match = false;
                if (cell.type() == Cell::DOUBLE || cell.type() == Cell::INT) {
                    if (cell.asDouble() <= val) match = true;
                }
                if (match) {
                    std::vector<Cell> rowData;
                    for (const auto& n : names) rowData.push_back(df.at(r, n));
                    result.pushRow(rowData);
                }
            }
            return result;
        }

        inline DataFrame filter(const DataFrame& df, const std::string& col, const Cell& val, CompareOp op) {
            DataFrame result;
            auto      names = df.columnNames();
            for (const auto& n : names) result.addColumn(n);

            // Optimization: Retrieve names once to avoid repeated findIndex overhead
            // In a production environment, we'd access m_columns directly if possible
            for (size_t r = 0; r < df.rows(); ++r) {
                const Cell& current = df.at(r, col);
                bool        match   = false;

                switch (op) {
                    case CompareOp::EQ: match = (current == val); break;
                    case CompareOp::NE: match = (current != val); break;
                    case CompareOp::GT:
                        if (current.type() != Cell::STRING && val.type() != Cell::STRING) match = current.asDouble() > val.asDouble();
                        break;
                    case CompareOp::LT:
                        if (current.type() != Cell::STRING && val.type() != Cell::STRING) match = current.asDouble() < val.asDouble();
                        break;
                    case CompareOp::GE:
                        if (current.type() != Cell::STRING && val.type() != Cell::STRING) match = current.asDouble() >= val.asDouble();
                        break;
                    case CompareOp::LE:
                        if (current.type() != Cell::STRING && val.type() != Cell::STRING) match = current.asDouble() <= val.asDouble();
                        break;
                }

                if (match) {
                    result.pushRow(df.getRow(r));
                }
            }
            return result;
        }

    }    // namespace data
}    // namespace ct