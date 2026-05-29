#!/usr/bin/env bash

# Exit on error, undefined variables, and pipe failures
set -eo pipefail

# Set the target directory to the first argument, or current directory by default
TARGET_DIR="${1:-.}"
CHECK_MODE=false

# Check if clang-format is installed
if ! command -v clang-format &> /dev/null; then
    echo "Error: clang-format is not installed or not in PATH." >&2
    exit 1
fi

# Parse arguments
if [[ "$TARGET_DIR" == "--check" ]] || [[ "$TARGET_DIR" == "-c" ]]; then
    CHECK_MODE=true
    TARGET_DIR="${2:-.}"
elif [[ "$2" == "--check" ]] || [[ "$2" == "-c" ]]; then
    CHECK_MODE=true
    TARGET_DIR="$1"
fi

# Check files for formatting issues
check_formatting() {
    # Use a null-separated list to safely handle filenames with spaces and run
    # clang-format only when files are found.
    local not_compliant
    not_compliant=$(find "$TARGET_DIR" -type d \( -name ".git" -o -name "build" \) -prune -o \
            -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.cc" -o -name "*.cxx" -o \
                       -name "*.h" -o -name "*.hpp" -o -name "*.hh" -o -name "*.hxx" \) \
            -print0 | xargs -0 --no-run-if-empty clang-format --dry-run --Werror 2>&1) || true

    if [[ -n "$not_compliant" ]]; then
        echo "$not_compliant"
        return 1
    fi
    return 0
}

if [[ "$CHECK_MODE" == "true" ]]; then
    echo "Checking C/C++ file formatting in: $TARGET_DIR"
    if check_formatting; then
        echo "✅ All files are properly formatted."
        exit 0
    else
        echo "❌ Code formatting issues found. Run './format.sh' to fix."
        exit 1
    fi
else
    echo "Formatting C/C++ files in: $TARGET_DIR"
    
    # Find and format files recursively, skipping .git and build directories
    find "$TARGET_DIR" -type d \( -name ".git" -o -name "build" \) -prune -o \
         -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.cc" -o -name "*.cxx" -o \
                    -name "*.h" -o -name "*.hpp" -o -name "*.hh" -o -name "*.hxx" \) \
         -print0 | xargs -0 clang-format -i
    
    echo "Formatting complete!"
fi
