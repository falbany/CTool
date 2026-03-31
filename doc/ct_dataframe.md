# ct_dataframe Module

The `ct::data` module provides a lightweight, type-safe C++11 implementation of a DataFrame. It allows for managing heterogeneous tabular data (mixing strings, integers, and doubles) within a single container, mimicking the basic behavior of Python's Pandas library.

---

## Architecture Overview

The module is organized into four logical components:

| Header          | Responsibility                                    |
| --------------- | ------------------------------------------------- |
| `df_types.hpp`  | `Cell` class (type-safe variant) and helpers      |
| `df_core.hpp`   | `DataFrame` and `Column` core class definitions   |
| `df_filter.hpp` | Filtering engine (`filterEq`, `filterGt`, etc.)   |
| `df_io.hpp`     | I/O utilities (CSV export, Array2D, Pretty Print) |

All implementations are provided as inline template code via corresponding `.tpp` files to enable header-only usage.

---

## API Reference

### `ct::data::Cell`

A custom type-safe variant for C++11 that stores values of type `double`, `int`, `std::string`, or `EMPTY`. It replaces `std::variant` for projects limited to C++11.

#### Constructors

| Constructor                  | Description                            |
| ---------------------------- | -------------------------------------- |
| `Cell()`                     | Creates an empty cell (`Type::EMPTY`). |
| `Cell(double v)`             | Creates a cell holding a `double`.     |
| `Cell(int v)`                | Creates a cell holding an `int`.       |
| `Cell(const std::string& v)` | Creates a cell holding a string.       |
| `Cell(const char* v)`        | Creates a cell holding a C-string.     |

#### Methods

| Method       | Description                                                           | Throws                                       |
| ------------ | --------------------------------------------------------------------- | -------------------------------------------- |
| `type()`     | Returns the enum `Type` (EMPTY, DOUBLE, INT, STRING).                 | —                                            |
| `asDouble()` | Returns value as `double`. Converts `INT` automatically if necessary. | `std::runtime_error` if type is STRING/EMPTY |
| `asInt()`    | Returns value as `int`. Truncates `DOUBLE` if necessary.              | `std::runtime_error` if type is STRING/EMPTY |
| `asString()` | Returns a const reference to the internal string.                     | `std::runtime_error` if type is not STRING   |

#### Operators

- `operator==` / `operator!=` — Equality comparison. Returns `false` for mismatched types (except EMPTY).

#### Free Functions

| Function                  | Description                                  |
| ------------------------- | -------------------------------------------- |
| `isNull(const Cell& val)` | Returns `true` if the cell is `Type::EMPTY`. |

---

### `ct::data::Column` *(Internal)*

A named container for a vector of `Cell` values. Columns are managed internally by `DataFrame` and are not typically used directly.

| Member | Type                | Description                |
| ------ | ------------------- | -------------------------- |
| `name` | `std::string`       | Column identifier          |
| `data` | `std::vector<Cell>` | Cell values for the column |

---

### `ct::data::DataFrame`

The main class storing data in **column-major** format for better cache locality during column-wise operations.

#### Construction & Modifications

| Method                                  | Description                                                |
| --------------------------------------- | ---------------------------------------------------------- |
| `DataFrame()`                           | Constructs an empty DataFrame.                             |
| `addColumn(const std::string& name)`    | Adds a new column header. Case-sensitive, must be unique.  |
| `pushRow(const std::vector<Cell>& row)` | Appends a row. Size must match column count.               |
| `clear()`                               | Removes all columns and data, resetting to an empty state. |

#### Dimensions & Inspection

| Method               | Description                                         |
| -------------------- | --------------------------------------------------- |
| `rows()`             | Returns the total number of rows (0 if empty).      |
| `cols()`             | Returns the total number of columns.                |
| `columnNames()`      | Returns `std::vector<std::string>` of column names. |
| `head(size_t n = 5)` | Returns a new DataFrame with the first `n` rows.    |

#### Data Access

| Method                                       | Description                                                     |
| -------------------------------------------- | --------------------------------------------------------------- |
| `at(size_t row, const std::string& colName)` | Returns `const Cell&`. Throws if row or column is out of range. |
| `getRow(size_t index)`                       | Returns a copy of the row as `std::vector<Cell>`.               |
| `getColumn(const std::string& name)`         | Returns a copy of the column as `std::vector<Cell>`.            |

#### Exceptions

| Method        | Exception               | Condition                           |
| ------------- | ----------------------- | ----------------------------------- |
| `pushRow()`   | `std::invalid_argument` | Row size doesn't match column count |
| `at()`        | `std::out_of_range`     | Invalid row index or column name    |
| `getRow()`    | `std::out_of_range`     | Invalid row index                   |
| `getColumn()` | `std::out_of_range`     | Column name not found               |

---

### Filtering Engine

Filtering functions are **immutable**: they return a new `DataFrame` and leave the original unchanged.

#### Quick Filters

| Function                                                                 | Description                                  |
| ------------------------------------------------------------------------ | -------------------------------------------- |
| `filterEq(const DataFrame& df, const std::string& col, const Cell& val)` | Rows where column equals `val`.              |
| `filterNe(const DataFrame& df, const std::string& col, const Cell& val)` | Rows where column **not** equal `val`.       |
| `filterGt(const DataFrame& df, const std::string& col, double val)`      | Rows where column `>` `val` (numeric only).  |
| `filterLt(const DataFrame& df, const std::string& col, double val)`      | Rows where column `<` `val` (numeric only).  |
| `filterGe(const DataFrame& df, const std::string& col, double val)`      | Rows where column `>=` `val` (numeric only). |
| `filterLe(const DataFrame& df, const std::string& col, double val)`      | Rows where column `<=` `val` (numeric only). |

> **Note:** Numeric filters (`Gt`, `Lt`, `Ge`, `Le`) automatically skip non-numeric cells (STRING/EMPTY) without throwing.

#### Generic Filter

```cpp
DataFrame filter(const DataFrame& df, const std::string& col, const Cell& val, CompareOp op);
```

| `CompareOp` | Meaning          |
| ----------- | ---------------- |
| `EQ`        | Equal to         |
| `NE`        | Not equal to     |
| `GT`        | Greater than     |
| `LT`        | Less than        |
| `GE`        | Greater or equal |
| `LE`        | Less or equal    |

---

### I/O Utilities

#### CSV Export

```cpp
bool toCSV(const DataFrame& df, const std::string& filename, char delim = ',');
```

Exports the DataFrame to a CSV file using the `ct::csv::Writer` backend. Returns `true` on success, `false` on failure.

- `delim`: Column separator character (default `,`).
- Empty cells are written as empty strings.
- Double/Int cells are formatted using `std::to_string`.

#### Array2D Conversion

```cpp
ct::array::Array2D<double> toArray2D(const DataFrame& df, const std::vector<std::string>& columnNames);
```

Extracts specified numeric columns into a dense `Array2D<double>` matrix for numerical processing.

- **Throws** `std::runtime_error` if any listed column contains non-numeric data.
- Returns an empty `Array2D` if the DataFrame has no rows or the column list is empty.

#### Pretty Printing

```cpp
std::ostream& operator<<(std::ostream& os, const ct::data::DataFrame& df);
```

Prints a formatted table to any `std::ostream` (e.g., `std::cout`).

- Columns are left-aligned with a width of 15 characters.
- Doubles are formatted to 2 decimal places.
- Long strings (> 13 chars) are truncated with `...`.
- Empty cells display as `NaN`.

---

## Usage Examples

### Creating and Populating a DataFrame

```cpp
#include "ct/ct_dataframe/df_core.hpp"

ct::data::DataFrame df;
df.addColumn("ID");
df.addColumn("Name");
df.addColumn("Score");

df.pushRow({101, "Alice", 95.5});
df.pushRow({102, "Bob", 88.0});
df.pushRow({103, "Charlie", 92.3});
```

### Filtering Data

```cpp
#include "ct/ct_dataframe/df_filter.hpp"

// Get only rows where Score > 90.0
auto highPerformers = ct::data::filterGt(df, "Score", 90.0); 

// Using the generic filter with CompareOp
auto midRange = ct::data::filter(df, "Score", 85.0, ct::data::CompareOp::GE);
```

### 3. Math and I/O Integration
```cpp
#include "ct/data/ct_dataframe.hpp"
#include "ct/ct_math.hpp"

// Export to CSV for external reporting
ct::data::toCSV(df, "output.csv");          // Comma-separated
ct::data::toCSV(df, "output.tsv", '\t');    // Tab-separated

// Convert numeric columns to a matrix for linear regression
auto matrix = ct::data::toArray2D(df, {"ID", "Value"});

// Perform math on the exported matrix
matrix.scale(2.0); 
```

### Pretty Printing

```cpp
#include <iostream>
#include "ct/ct_dataframe/df_io.hpp"

std::cout << df << std::endl;
// Output:
// ID             Name           Score          
// ---------------------------------------------
// 101            Alice          95.50          
// 102            Bob            88.00          
// 103            Charlie        92.30          
```

### Inspecting Data

```cpp
// Get first rows (for quick inspection)
auto preview = df.head(2);

// Access individual cells
const auto& name = df.at(0, "Name");
std::cout << name.asString() << std::endl;  // "Alice"

// Check if cell is empty
if (ct::data::isNull(df.at(0, "ID"))) {
    // handle missing value
}
```

---

## Design Notes

- **Column-Major Storage**: Data is stored column-by-column rather than row-by-row. This optimizes column access and filtering operations, which are the most common DataFrame workloads.
- **No External Dependencies**: The `Cell` class replaces `std::variant` for C++11 compatibility. It uses a simple tagged union approach with explicit type checking.
- **Immutable Filtering**: All filter functions return a new DataFrame rather than modifying the original, enabling functional-style data pipelines.
- **Header-Only Implementation**: All code is inlined via `.tpp` files, allowing the module to be used without separate compilation units.

---

**Author:** Florent ALBANY (FAL)  
**Last Updated:** 2026-03-24