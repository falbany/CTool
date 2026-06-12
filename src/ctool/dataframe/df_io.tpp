/**
 * @file df_io.tpp
 * @brief Implementation of DataFrame I/O operations.
 */

#include <iomanip>

namespace ctool {
    namespace data {

        // --- Factory Functions (Imports) ---

        inline DataFrame fromCSV(const std::string& filename, char delim, bool hasHeader) {
            DataFrame df;
            // Read raw strings from CSV utility (don't skip header yet so we can parse it)
            std::vector<std::vector<std::string>> rawData = ctool::csv::read(filename, delim, false);

            if (rawData.empty()) return df;

            size_t startRow = 0;
            size_t numCols  = rawData[0].size();

            if (hasHeader) {
                for (const auto& name : rawData[0]) df.addColumn(name);
                startRow = 1;
            } else {
                for (size_t i = 0; i < numCols; ++i) df.addColumn("col" + std::to_string(i));
            }

            for (size_t i = startRow; i < rawData.size(); ++i) {
                std::vector<Cell> row;
                for (const auto& cellStr : rawData[i]) {
                    // Basic heuristic: try to treat as double, otherwise string
                    try {
                        row.push_back(std::stod(cellStr));
                    } catch (...) {
                        row.push_back(cellStr);
                    }
                }
                df.pushRow(row);
            }
            return df;
        }

        inline DataFrame fromArray2D(const ctool::array::Array2D<double>& array, const std::vector<std::string>& columnNames) {
            DataFrame df;
            if (array.empty()) return df;

            if (!columnNames.empty() && columnNames.size() != array.cols()) {
                throw std::invalid_argument("fromArray2D: columnNames size mismatch.");
            }

            for (size_t j = 0; j < array.cols(); ++j) {
                df.addColumn(columnNames.empty() ? "col" + std::to_string(j) : columnNames[j]);
            }

            for (size_t i = 0; i < array.rows(); ++i) {
                std::vector<double> rowData = array.sliceRow(i);
                df.pushRow(std::vector<Cell>(rowData.begin(), rowData.end()));
            }
            return df;
        }

        inline DataFrame fromNumArray(const ctool::array::ArrayND<double>& array, const std::vector<std::string>& columnNames) {
            DataFrame df;
            if (array.size() == 0) return df;    // Empty check

            const size_t rows = array.rows();
            const size_t cols = array.cols();

            if (!columnNames.empty() && columnNames.size() != cols) {
                throw std::invalid_argument("fromNumArray: columnNames size mismatch with array width.");
            }

            // Add columns first
            for (size_t j = 0; j < cols; ++j) {
                std::string name = columnNames.empty() ? "col" + std::to_string(j) : columnNames[j];
                df.addColumn(name);
            }

            // Access contiguous data directly for performance
            const double* rawData = array.data();

            // Fill rows
            for (size_t i = 0; i < rows; ++i) {
                std::vector<Cell> row;
                row.reserve(cols);
                for (size_t j = 0; j < cols; ++j) {
                    // Calculate 1D index manually or use operator()
                    // Since NumArray uses contiguous storage: index = i * cols + j
                    double val = rawData[i * cols + j];
                    row.push_back(Cell(val));
                }
                df.pushRow(row);
            }
            return df;
        }

        // --- Member Method Implementations (Exports) ---
        // These are defined here in df_io.tpp but called as df.toCSV(), etc.

        inline bool DataFrame::toCSV(const std::string& filename, char delim) {
            try {
                ctool::csv::Writer writer(filename, delim, false);
                writer.setHeader(columnNames());

                for (size_t r = 0; r < rows(); ++r) {
                    std::vector<std::string> rowStrings;
                    rowStrings.reserve(cols());

                    auto names = columnNames();
                    for (const auto& name : names) {
                        const Cell& cell = at(r, name);
                        if (cell.type() == Cell::STRING) {
                            rowStrings.push_back(cell.asString());
                        } else if (cell.type() == Cell::EMPTY) {
                            rowStrings.push_back("");
                        } else {
                            rowStrings.push_back(std::to_string(cell.asDouble()));
                        }
                    }
                    writer.writeRow(rowStrings);
                }
                return true;
            } catch (...) {
                return false;
            }
        }

        inline ctool::array::Array2D<double> DataFrame::toArray2D(const std::vector<std::string>& columnNames) const {
            if (rows() == 0 || columnNames.empty()) {
                return ctool::array::Array2D<double>();
            }

            ctool::array::Array2D<double> matrix(rows(), columnNames.size());

            for (size_t j = 0; j < columnNames.size(); ++j) {
                std::vector<Cell> colData = getColumn(columnNames[j]);
                for (size_t i = 0; i < colData.size(); ++i) {
                    matrix(i, j) = colData[i].asDouble();
                }
            }
            return matrix;
        }

        inline ctool::array::ArrayND<double> DataFrame::toNumArray(const std::vector<std::string>& columnNames) const {
            if (rows() == 0 || columnNames.empty()) {
                return ctool::array::ArrayND<double>(0, 0);
            }

            const size_t rows_count = rows();
            const size_t cols_count = columnNames.size();

            ctool::array::ArrayND<double> matrix(rows_count, cols_count);
            double*                   dataPtr = matrix.data();

            for (size_t j = 0; j < cols_count; ++j) {
                const std::string&       colName = columnNames[j];
                const std::vector<Cell>& colData = getColumn(colName);

                for (size_t i = 0; i < rows_count; ++i) {
                    if (colData[i].type() != Cell::DOUBLE && colData[i].type() != Cell::INT) {
                        throw std::runtime_error("toNumArray: Column '" + colName + "' contains non-numeric data at row " + std::to_string(i));
                    }

                    double val                   = colData[i].asDouble();
                    dataPtr[i * cols_count + j] = val;
                }
            }
            return matrix;
        }

    }    // namespace data
}    // namespace ctool

inline std::ostream& operator<<(std::ostream& os, const ctool::data::DataFrame& df) {
    auto names = df.columnNames();

    // Print Header
    for (const auto& name : names) {
        os << std::left << std::setw(15) << name;
    }
    os << "\n" << std::string(names.size() * 15, '-') << "\n";

    // Print Rows
    for (size_t r = 0; r < df.rows(); ++r) {
        for (const auto& name : names) {
            const auto& cell = df.at(r, name);
            std::string val;
            switch (cell.type()) {
                case ctool::data::Cell::INT:    val = std::to_string(cell.asInt()); break;
                case ctool::data::Cell::DOUBLE: {
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(2) << cell.asDouble();
                    val = ss.str();
                    break;
                }
                case ctool::data::Cell::STRING: val = cell.asString(); break;
                default:                     val = "NaN"; break;
            }
            if (val.length() > 13) val.replace(10, 3, "...");
            os << std::left << std::setw(15) << val;
        }
        os << "\n";
    }
    return os;
}
