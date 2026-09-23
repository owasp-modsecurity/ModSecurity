#!/bin/bash

# Wrapper script to run benchmark_rules with proper library paths

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Set library path for macOS
export DYLD_LIBRARY_PATH="${SCRIPT_DIR}/../../src/.libs:/opt/homebrew/lib:${DYLD_LIBRARY_PATH}"

# Run the benchmark with all arguments passed through
"${SCRIPT_DIR}/benchmark_rules" "$@"
