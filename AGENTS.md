# 🤖 Agent Contribution Protocol (AGENTS.md)

This document defines the mandatory steps for adding new features or modules to the CTool/CBridge project.

## 1. Directory Standards
- **Pure C (CBridge):** Source in `cb/`, headers in `cb/`. Use `cb_` prefix.
- **C++ (CTool):** Source in `ct/`, headers in `ct/`, templates in `ct/*.tpp`. Use `ct_` prefix.
- **Documentation:** Every new module **must** have a corresponding `.md` file in `doc/`.

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
- Create `doc/module_name.md`.
- Update `doc/README.md` and the root `README.md` tables with the new module.
- Update `CHANGELOG.md` following the [Keep a Changelog](https://keepachangelog.com/) format.

## 5. Implementation Workflow
1. Create implementation (`.c`/`.cpp`) and headers.
2. Link in `CBridge.h/c` or `CTool.hpp/cpp`.
3. Draft the Markdown guide in `doc/`.
4. Update the Documentation Hub and the Changelog.