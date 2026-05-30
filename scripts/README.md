# 🛠️ Scripts Directory

This folder contains utility scripts for the CTool project.

## 📄 Scripts

### `format.sh`
This script uses `clang-format` to ensure code consistency across the project.
It automatically finds and formats source files in `src/` and `tests/`.

**Usage:**
- `./scripts/format.sh`: Formats all C, C++, and header files.
- `./scripts/format.sh --check`: Checks for formatting violations without applying changes (used in CI).
