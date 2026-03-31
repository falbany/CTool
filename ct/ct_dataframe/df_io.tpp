/**
 * @file df_io.tpp
 * @brief Implementation of DataFrame I/O operations.
 */

#include <iomanip>

namespace ct {
    namespace data {

        inline bool toCSV(const DataFrame& df, const std::string& filename, char delim) {
            try {
                ct::csv::Writer writer(filename, delim, false);
                writer.setHeader(df.columnNames());

                for (size_t r = 0; r < df.rows(); ++r) {
                    std::vector<Cell>        rowCells = df.getRow(r);
                    std::vector<std::string> rowStrings;
                    for (const auto& cell : rowCells) {
                        if (cell.type() == Cell::STRING)
                            rowStrings.push_back(cell.asString());
                        else if (cell.type() == Cell::EMPTY)
                            rowStrings.push_back("");
                        else
                            rowStrings.push_back(std::to_string(cell.asDouble()));
                    }
                    writer.writeRow(rowStrings);
                }
                return true;
            } catch (...) {
                return false;
            }
        }

        inline DataFrame fromCSV(const std::string& filename, char delim, bool hasHeader) {
            DataFrame df;
            // Read raw strings from CSV utility (don't skip header yet so we can parse it)
            std::vector<std::vector<std::string>> rawData = ct::csv::read(filename, delim, false);

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

        inline DataFrame fromArray2D(const ct::array::Array2D<double>& array, const std::vector<std::string>& columnNames) {
            DataFrame df;
            if (array.empty()) return df;

            if (!columnNames.empty() && columnNames.size() != array.cols()) {
                throw std::invalid_argument("fromArray2D: columnNames size mismatch.");
            }

            for (size_t j = 0; j < array.cols(); ++j) {
                df.addColumn(columnNames.empty() ? "col" + std::to_string(j) : columnNames[j]);
            }

            for (size_t i = 0; i < array.rows(); ++i) {
                df.pushRow(std::vector<Cell>(array.toVector()[i].begin(), array.toVector()[i].end()));
            }
            return df;
        }

        inline ct::array::Array2D<double> toArray2D(const DataFrame& df, const std::vector<std::string>& columnNames) {
            if (df.rows() == 0 || columnNames.empty()) return ct::array::Array2D<double>();

            ct::array::Array2D<double> matrix(df.rows(), columnNames.size());

            for (size_t j = 0; j < columnNames.size(); ++j) {
                std::vector<Cell> colData = df.getColumn(columnNames[j]);
                for (size_t i = 0; i < colData.size(); ++i) {
                    matrix(i, j) = colData[i].asDouble();    // asDouble throws if STRING
                }
            }
            return matrix;
        }

        inline DataFrame fromNumArray(const ct::num::NumArray<double>& array, const std::vector<std::string>& columnNames) {
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
            const double* raw_data = array.data();

            // Fill rows
            for (size_t i = 0; i < rows; ++i) {
                std::vector<Cell> row;
                row.reserve(cols);
                for (size_t j = 0; j < cols; ++j) {
                    // Calculate 1D index manually or use operator()
                    // Since NumArray uses contiguous storage: index = i * cols + j
                    double val = raw_data[i * cols + j];
                    row.push_back(Cell(val));
                }
                df.pushRow(row);
            }
            return df;
        }

        inline ct::num::NumArray<double> toNumArray(const DataFrame& df, const std::vector<std::string>& columnNames) {
            if (df.rows() == 0 || columnNames.empty()) {
                return ct::num::NumArray<double>(0, 0);
            }

            const size_t rows = df.rows();
            const size_t cols = columnNames.size();

            // Pre-allocate contiguous buffer (NumPy style)
            ct::num::NumArray<double> matrix(rows, cols);
            double*                   data_ptr = matrix.data();    // Get raw pointer for fast writing

            for (size_t j = 0; j < cols; ++j) {
                const std::string&       colName = columnNames[j];
                const std::vector<Cell>& colData = df.getColumn(colName);

                for (size_t i = 0; i < rows; ++i) {
                    // Check type safety
                    if (colData[i].type() != Cell::DOUBLE && colData[i].type() != Cell::INT) {
                        throw std::runtime_error("toNumArray: Column '" + colName + "' contains non-numeric data at row " + std::to_string(i));
                    }

                    double val = colData[i].asDouble();
                    // Write to contiguous 1D buffer (row-major order: i * cols + j)
                    data_ptr[i * cols + j] = val;
                }
            }
            return matrix;
        }

    }    // namespace data
}    // namespace ct

inline std::ostream& operator<<(std::ostream& os, const ct::data::DataFrame& df) {
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
                case ct::data::Cell::INT:    val = std::to_string(cell.asInt()); break;
                case ct::data::Cell::DOUBLE: {
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(2) << cell.asDouble();
                    val = ss.str();
                    break;
                }
                case ct::data::Cell::STRING: val = cell.asString(); break;
                default:                     val = "NaN"; break;
            }
            if (val.length() > 13) val = val.substr(0, 10) + "...";
            os << std::left << std::setw(15) << val;
        }
        os << "\n";
    }
    return os;
}
