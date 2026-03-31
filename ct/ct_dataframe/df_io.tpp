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
