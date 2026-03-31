# DataFrame Module

The `DataFrame` module provides a lightweight, type-safe C++11 implementation of a 2D data container. It allows for managing heterogeneous tabular data (mixing strings, integers, and doubles) within a single structure, mimicking the core behavior of Python's Pandas library.

---

## Architecture Overview

The module is organized into four logical components:

| Header          | Responsibility                                  |
| --------------- | ----------------------------------------------- |
| `df_types.hpp`  | `Cell` class (type-safe variant) and helpers    |
| `df_core.hpp`   | `DataFrame` and `Column` core class definitions |
| `df_filter.hpp` | Filtering engine (`filterEq`, `filterGt`, etc.) |
| `df_io.hpp`     | **Import/Export & Conversion Logic**            |

**Design Pattern:**
- **`df_core`**: Pure data structure. Defines the API.
- **`df_io`**: Contains implementation logic for **imports** (loading from files/arrays) and **exports** (saving to CSV/converting to arrays).
- **Dual-Mode API**: 
  - **Creation**: Data can be loaded from external sources using **Free Functions** (`ct::data::fromCSV`, `fromArray2D`).
  - **Manipulation**: Data is manipulated via **Instance Methods** (`df.pushRow`, `df.toCSV`).

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

#### **Export Methods (Instance)**
*Methods to save or convert the current DataFrame instance.*

| Method                      | Description                                                            | Returns / Throws   |
| :-------------------------- | :--------------------------------------------------------------------- | :----------------- |
| `df.toCSV(filename, delim)` | Exports the DataFrame to a CSV file. Empty cells become empty strings. | `bool` (success)   |
| `df.toArray2D(colNames)`    | Extracts numeric columns into a legacy `Array2D<double>`.              | `Array2D<double>`  |
| `df.toNumArray(colNames)`   | Extracts numeric columns into a high-performance `NumArray<double>`.   | `NumArray<double>` |

#### **Import Functions (Free)**
*Static or free functions to create a DataFrame from external sources.*

| Function                           | Description                                                               | Returns / Throws |
| :--------------------------------- | :------------------------------------------------------------------------ | :--------------- |
| `fromCSV(filename, delim, header)` | Loads a CSV file. Automatically parses numeric strings as `DOUBLE` cells. | `DataFrame`      |
| `fromArray2D(array, names)`        | Converts a legacy `Array2D` into a DataFrame.                             | `DataFrame`      |
| `fromNumArray(array, names)`       | Converts a high-performance `NumArray` into a DataFrame.                  | `DataFrame`      |

> **Note:** Import functions are free functions in the `ct::data` namespace, while export functions are member methods of `DataFrame`.

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

### I/O Utilities & Integration

The module provides a complete pipeline for data ingestion, manipulation, and export, with tight integration with the `ct_num` (NumPy-like) module.

#### NumArray Integration (High Performance)
The module supports direct conversion from `DataFrame` to `ct::num::NumArray`, leveraging contiguous memory layouts for high-performance numerical pipelines.

**Why use `NumArray` over `Array2D`?**
- **Memory:** `NumArray` stores data in a single contiguous block (like NumPy), enabling faster CPU cache usage and SIMD optimizations.
- **Math:** `NumArray` supports element-wise operators (`+`, `*`, `sqrt`) natively, whereas `Array2D` requires manual loops.

#### Example: Complete Data Science Pipeline (Import → Process → Export)
```cpp
#include "ct/ct_dataframe.hpp"
#include "ct/ct_num.hpp"

int main() {
    // 1. IMPORT Data from CSV
    // Free function creates a new DataFrame from a file
    ct::data::DataFrame df = ct::data::fromCSV("sales_data.csv");

    // 2. FILTER Data
    // Get rows where 'Quantity' > 10
    auto filtered = ct::data::filterGt(df, "Quantity", 10);

    // 3. CONVERT to High-Performance Matrix
    // Extracts 'Price' and 'Discount' into a contiguous NumArray
    ct::num::NumArray<double> matrix = filtered.toNumArray({"Price", "Discount"});

    // 4. PROCESS with NumArray Ufuncs (Vectorized Math)
    // Apply 10% discount: Discount = Price * 0.1
    ct::num::NumArray<double> discounts = ct::num::mul(matrix, 0.1); // Note: using ct::num::mul if available, or manual
    
    // Example using element-wise multiplication:
    ct::num::NumArray<double> price_col = matrix.sliceRow(0); // Conceptual
    // Let's use element-wise logic:
    ct::num::NumArray<double> profit = matrix + matrix; // Just a demo of + operator

    // Better Example: Calculate Total Revenue (Price * Quantity)
    // Assuming 'Quantity' is column 0 and 'Price' is column 1
    ct::num::NumArray<double> revenue = matrix; 
    // (In a real scenario, you'd perform row-wise math or use broadcasting)
    
    // 5. EXPORT Results back to DataFrame and CSV
    auto result_df = ct::data::DataFrame::fromNumArray(revenue, {"Total_Revenue"});
    result_df.toCSV("results.csv");

    return 0;
}
```

#### Import Functions Details
The module provides three free functions to populate DataFrames from external sources:

1.  **`fromCSV(filename, delim, hasHeader)`**
    *   Reads a CSV file.
    *   **Heuristic:** Automatically attempts to parse numeric strings as `DOUBLE`. Non-numeric strings become `STRING`.
    *   **Headers:** If `hasHeader` is `true` (default), the first row defines column names.

2.  **`fromArray2D(array, names)`**
    *   Converts a legacy `ct::array::Array2D` into a DataFrame.
    *   Useful for migrating data from older grid-based systems.

3.  **`fromNumArray(array, names)`**
    *   Converts a high-performance `ct::num::NumArray` into a DataFrame.
    *   Optimized for performance by reading contiguous memory directly.
    *   **Throws** `std::invalid_argument` if column names count mismatches array width.

#### CSV Export Details
When using `df.toCSV`:
- `delim`: Defaults to `,`. Use `'\t'` for TSV.
- `DOUBLE`/`INT` cells are converted via `std::to_string`.
- `EMPTY` cells are written as an empty field (,,).
- Returns `true` on success, `false` on I/O error.

#### Array2D vs. NumArray Conversion
- **`toArray2D`**: Returns a legacy `Array2D<double>` (Vector of Vectors). Suitable for compatibility.
- **`toNumArray`**: Returns a `NumArray<double>` (Contiguous memory). **Recommended** for numerical computations.
  - **Throws** `std::runtime_error` if any listed column contains non-numeric data.
  - Returns an empty `NumArray` if the DataFrame has no rows or the column list is empty.

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

### 1. Creating and Populating a DataFrame

```cpp
#include "ct/ct_dataframe.hpp"

ct::data::DataFrame df;
df.addColumn("ID");
df.addColumn("Name");
df.addColumn("Score");

df.pushRow({101, "Alice", 95.5});
df.pushRow({102, "Bob", 88.0});
df.pushRow({103, "Charlie", 92.3});
```

### 2. Filtering Data

```cpp
#include "ct/ct_dataframe.hpp"

// Get only rows where Score > 90.0
auto highPerformers = ct::data::filterGt(df, "Score", 90.0); 

// Using the generic filter with CompareOp
auto midRange = ct::data::filter(df, "Score", 85.0, ct::data::CompareOp::GE);
```

### 3. Importing Data

```cpp
#include "ct/ct_dataframe.hpp"

// Load from CSV
auto df1 = ct::data::fromCSV("input_data.csv");

// Load from legacy Array2D
ct::array::Array2D<double> legacy_data = {{1.1, 2.2}, {3.3, 4.4}};
auto df2 = ct::data::fromArray2D(legacy_data, {"ColA", "ColB"});

// Load from high-performance NumArray
ct::num::NumArray<double> num_data = {{5.0, 6.0}};
auto df3 = ct::data::fromNumArray(num_data, {"X", "Y"});
```

### 4. Export and Conversion

```cpp
#include "ct/ct_dataframe.hpp"
#include "ct/ct_num.hpp"

ct::data::DataFrame df;
df.addColumn("X");
df.addColumn("Y");
df.pushRow({1.1, 2.2});
df.pushRow({3.3, 4.4});

// Export to CSV
df.toCSV("data.csv");
df.toCSV("data.tsv", '\t');

// Convert to legacy Array2D
ct::array::Array2D<double> arr = df.toArray2D({"X", "Y"});

// Perform math on Array2D
arr.scale(2.0);

// Convert to high-performance NumArray
ct::num::NumArray<double> num_arr = df.toNumArray({"X", "Y"});

// Perform math on NumArray
auto sum_arr = num_arr + num_arr;
auto scaled = num_arr * 2.0;
auto roots = ct::num::sqrt(num_arr);
```

### 5. NumArray Integration with Ufuncs

```cpp
#include "ct/ct_dataframe.hpp"
#include "ct/ct_num.hpp"

// 1. Load Data
auto df = ct::data::fromCSV("sensor_data.csv");

// 2. Extract Numeric Columns
auto matrix = df.toNumArray({"Temp", "Pressure"});

// 3. Apply Universal Functions (Ufuncs)
// Calculate square root of values
auto roots = ct::num::sqrt(matrix);

// Calculate sine of values
auto sine_vals = ct::num::sin(matrix);

// 4. Element-wise Arithmetic
auto sum_matrix = matrix + matrix; // Element-wise addition
auto scaled = matrix * 1.5;        // Element-wise scaling

// 5. Reductions
double total_temp = ct::num::sum(matrix); // Sum of all elements
double avg_pressure = ct::num::mean(matrix); // Average
```

### 6. Pretty Printing

```cpp
#include "ct/ct_dataframe.hpp"
#include <iostream>

ct::data::DataFrame df;
df.addColumn("ID");
df.addColumn("Name");
df.pushRow({1, "Alice"});
df.pushRow({2, "Bob"});

std::cout << df << std::endl;
// Output:
// ID             Name              
// --------------------------------
// 1              Alice             
// 2              Bob               
```

### 7. Inspecting Data

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

- **Column-Major Storage**: Data is stored column-by-column rather than row-by-row. This optimizes column access and filtering operations.
- **Hybrid API**: Imports are free functions (`fromCSV`), while exports are member methods (`toCSV`). This separation keeps the `DataFrame` class focused on data management while allowing flexible data ingestion.
- **No External Dependencies**: The `Cell` class replaces `std::variant` for C++11 compatibility.
- **Immutable Filtering**: All filter functions return a new `DataFrame` rather than modifying the original, enabling functional-style data pipelines.
- **Header-Only Implementation**: All code is inlined via `.tpp` files, allowing the module to be used without separate compilation units.

---

**Author:** Florent ALBANY (FAL)  
**Last Updated:** 2026-03-24