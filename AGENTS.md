# 🤖 Agent Contribution Protocol (AGENTS.md)

This document defines the mandatory steps for adding new features or modules to the CTool/CBridge project.

## 1. Directory Standards
- **Pure C (CBridge):** Source in `src/cbridge/`, headers in `src/cbridge/`.
- **C++ (CTool):** Source in `src/ctool/`, headers in `src/ctool/`, templates in `src/ctool/*.tpp`.
- **Internal:** Private headers and core utilities in `src/internal/`. These are not part of the public API.
- **Documentation:** Every new module **must** have a corresponding `.md` file in `doc/src/cbridge/` or `doc/src/ctool/`.

## 2. Namespace Integration
Every new module must be "registered" in the unified entry points:
- **CBridge.h:** Add the module's namespace struct to the `CBridge` global struct.
- **CTool.hpp:** Include the module header within the `ct` namespace.

## 3. Doxygen Requirements
All headers must include:
- `@file`, `@author`, `@brief`, and `@version` tags.
- Detailed `@param` and `@return` descriptions for every function.
- A `@note` regarding memory ownership (especially for CBridge).

## 4. Documentation Standards
- Create `doc/src/cbridge/module_name.md` or `doc/src/ctool/module_name.md`.
- Update `doc/README.md` and the root `README.md` tables with the new module.
- Update `CHANGELOG.md` following the [Keep a Changelog](https://keepachangelog.com/) format.

## 5. Formatting Standards
All code **must** be formatted using `clang-format`.
-   Run `./scripts/format.sh` to apply formatting.
-   The CI pipeline will enforce this with `./scripts/format.sh --check`.

## 6. Build and Test Standards
All modules are built and tested using CMake.
-   **Environment-Specific Toolchains:** Use the appropriate CMake toolchain file for your operating system. Refer to [cmake/README.md](cmake/README.md) for detailed instructions on how to select and use toolchains like `cmake/macOS-Clang.cmake`, `cmake/MSYS2-UCRT64.cmake`, or `cmake/MinGW.cmake`.
-   **Building:** `cmake -B build && cmake --build build`
-   **Testing:** `ctest --test-dir build`

## 7. Implementation Workflow
1. Create implementation (`.c`/`.cpp`) and headers.
2. Link in `src/CBridge.h/c` or `src/CTool.hpp/cpp`.
3. Draft the Markdown guide in `doc/`.
4. Update the Documentation Hub and the Changelog.