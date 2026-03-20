# ct_json Module

The `ct_json` module provides a lightweight, dependency-free way to handle JSON strings in C++11. It is optimized for flat configuration objects and simple data exchange.

## Features
- **Zero Dependencies**: No external libraries required.
- **Serialization**: Convert `std::map` or `std::vector` to JSON strings.
- **Extraction**: Retrieve specific values without parsing the entire tree.
- **Minification**: Compact JSON strings for network transmission.

---

## API Reference

### Namespaces
- `ct::json`

### Functions

| Function              | Return Type   | Description                                |
| :-------------------- | :------------ | :----------------------------------------- |
| `stringify(map)`      | `std::string` | Converts a map to a JSON object.           |
| `stringifyArray(vec)` | `std::string` | Converts a vector to a JSON array.         |
| `getValue(json, key)` | `std::string` | Finds the string value for a specific key. |
| `isValid(json)`       | `bool`        | Basic syntax validation.                   |
| `minify(json)`        | `std::string` | Removes whitespace outside of quotes.      |

---

## Usage Examples

### Creating JSON Data
```cpp
#include "ct_json.hpp"
#include <map>

std::map<std::string, std::string> config;
config["version"] = "1.2.0";
config["author"] = "Florent";

std::string rawJson = ct::json::stringify(config);
// Output: {"author":"Florent","version":"1.2.0"}
```

### Parsing a Value
```cpp
std::string response = "{\"status\": \"success\", \"code\": \"200\"}";
std::string status = ct::json::getValue(response, "status");

if (status == "success") {
    // Process data...
}
```

### Minifying for Network Transmission
```cpp
std::string pretty = R"({
    "id": 42,
    "msg": "Hello World"
})";

std::string compact = ct::json::minify(pretty);
```

---

## ⚠️ Limitations
- **Flat Objects**: `getValue` is designed for flat JSON structures. It may return incorrect results for nested keys with the same name.
- **Types**: All values are handled as `std::string`. Numeric conversion should be performed after extraction using `std::stod` or `std::stoi`.
```
