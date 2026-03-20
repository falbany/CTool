# ct_ini Module

The `ct_ini` module provides a robust interface for managing standard INI configuration files. It supports sections, keys, values, and comments.

## Features
- **Load/Save**: Full persistence support.
- **Section Handling**: Organizes data into logical blocks.
- **Default Values**: Provides fallbacks if a key is missing.
- **Comment Stripping**: Automatically ignores lines starting with `;` or `#` and handles inline comments.

---

## API Reference

### Classes
- **`ct::ini::Profile`**: The main container for INI data.

### Methods

| Method | Description |
| :--- | :--- |
| `load(filepath)` | Reads an INI file from disk. |
| `save(filepath)` | Writes current data to an INI file. |
| `get(section, key, def)` | Retrieves a value (with optional default). |
| `set(section, key, val)` | Updates or creates a new entry. |
| `hasSection(section)` | Checks for existence of a category. |
| `getKeys(section)` | Lists all keys in a specific section. |

---

## Usage Example

### Reading a Configuration
```cpp
#include "ct_ini.hpp"

ct::ini::Profile config;
if (config.load("settings.ini")) {
    std::string ip = config.get("Network", "ServerIP", "127.0.0.1");
    int port = std::stoi(config.get("Network", "Port", "8080"));
}
```

### Modifying and Saving
```cpp
ct::ini::Profile profile;
profile.set("User", "Name", "Florent");
profile.set("User", "Level", "Admin");

// Creates or overwrites the file
profile.save("user_pref.ini");
```

### Typical .ini Format Supported
```ini
; Global settings
[System]
Version = 1.1.0
DEBUG = true # Inline comments are supported
```

---

## 🛠 Best Practices
- **Sections**: Always use sections to avoid key name collisions.
- **Types**: Values are stored as strings. Use `std::stoi`, `std::stod`, or `ct::str::to_bool` for conversion.
