# /unit: Engineering Notation & Unit Utilities

## 📖 Overview
The `/unit` module provides utilities for handling engineering notation (SI prefixes), robust string parsing with units, and common engineering conversions (like temperature). It is designed to simplify the representation and input of physical quantities in scientific and engineering applications.

## ✨ Key Features
- **Engineering Formatting:** Automatically scales values to use appropriate SI prefixes (G, M, k, m, u, n, p, f).
- **Robust Parsing:** Parses strings containing numeric values, SI prefixes, and optional unit suffixes (e.g., "1.5mV", "2.2 kOhm").
- **Precision Control:** Custom precision for formatted output.
- **Unit Support:** Seamlessly adds unit symbols to formatted strings.
- **Temperature Conversions:** Easy conversion between Celsius and Kelvin.

## 🚀 Quick Start

### 1. Engineering Formatting
Convert raw numbers into human-readable strings with SI prefixes.

```cpp
#include "ctool/unit.hpp"

// Standard formatting
std::string s1 = ctool::unit::format(1200.0, 1, "Hz");   // "1.2 kHz"
std::string s2 = ctool::unit::format(0.0000015, 2, "A"); // "1.50 uA"

// Without units
std::string s3 = ctool::unit::format(1500000.0, 0);      // "2 M" (rounded)
```

### 2. Robust Parsing
Extract numeric values from strings containing suffixes. The parser is robust to spaces and unit symbols.

```cpp
double v1 = ctool::unit::parse("1.5kV");      // 1500.0
double v2 = ctool::unit::parse("100 mA");     // 0.1
double v3 = ctool::unit::parse("2.2uF");      // 0.0000022
double v4 = ctool::unit::parse("470");        // 470.0 (no prefix)
```

### 3. Temperature Conversions
```cpp
double kelvin  = ctool::unit::cToK(25.0);     // 298.15
double celsius = ctool::unit::kToC(0.0);      // -273.15
```

## 📊 Supported Prefixes

| Prefix | Symbol | Factor | Example |
| :--- | :--- | :--- | :--- |
| **Giga** | `G` | $10^9$ | `1.5 G` |
| **Mega** | `M` | $10^6$ | `100 M` |
| **Kilo** | `k` | $10^3$ | `2.2 k` |
| **(none)** | | $10^0$ | `1.0` |
| **milli** | `m` | $10^{-3}$ | `500 m` |
| **micro** | `u` | $10^{-6}$ | `10 u` |
| **nano** | `n` | $10^{-9}$ | `1 n` |
| **pico** | `p` | $10^{-12}$ | `1.5 p` |
| **femto** | `f` | $10^{-15}$ | `800 f` |

## ⚠️ Notes
- **Rounding:** The `format` function uses `std::fixed` with the specified precision, which includes standard rounding logic.
- **Parsing Strategy:** The `parse` function extracts the numeric part first, then looks at the *first* character of the remaining suffix for a matching SI prefix.
- **Limits:** Values larger than Giga or smaller than Femto will retain the Giga/Femto prefix but with larger/smaller numeric parts (e.g., `1500.0 GW`).

## 📝 License
Part of the CTool project.
