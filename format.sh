#!/usr/bin/env bash

# Exit on error, undefined variables, and pipe failures
set -euo pipefail

# Set the target directory to the first argument, or current directory by default
TARGET_DIR="${1:-.}"

# Check if clang-format is installed
if ! command -v clang-format &> /dev/null; then
    echo "Error: clang-format is not installed or not in PATH." >&2
    exit 1
fi

echo "Formatting C/C++ files in: $TARGET_DIR"

# Find and format files recursively, skipping .git and build directories
find "$TARGET_DIR" -type d \( -name ".git" -o -name "build" \) -prune -o \
     -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.cc" -o -name "*.cxx" -o \
                -name "*.h" -o -name "*.hpp" -o -name "*.hh" -o -name "*.hxx" \) \
     -print0 | xargs -0 clang-format -i

echo "Formatting complete!"
