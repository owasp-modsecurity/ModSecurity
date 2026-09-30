#!/bin/bash

# Helper script to build benchmark_rules with Google Benchmark
# This is a temporary solution until configure.ac is updated

set -e

echo "Building ModSecurity benchmark_rules with Google Benchmark"
echo "==========================================================="

# Check if Google Benchmark is installed
if ! pkg-config --exists benchmark 2>/dev/null; then
    if ! ldconfig -p 2>/dev/null | grep -q libbenchmark; then
        echo "ERROR: Google Benchmark not found!"
        echo ""
        echo "Please install it first:"
        echo "  Ubuntu/Debian: sudo apt-get install libbenchmark-dev"
        echo "  macOS:         brew install google-benchmark"
        echo "  From source:   See README.md for instructions"
        exit 1
    fi
fi

echo "✓ Google Benchmark found"

# Navigate to repository root
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
REPO_ROOT="${SCRIPT_DIR}/../.."
cd "${REPO_ROOT}"

echo "✓ Working directory: ${REPO_ROOT}"

# Check if libmodsecurity is built
if [ ! -f "src/.libs/libmodsecurity.so" ] && [ ! -f "src/.libs/libmodsecurity.dylib" ]; then
    echo ""
    echo "ERROR: libmodsecurity not built!"
    echo "Please build ModSecurity first:"
    echo "  ./build.sh"
    echo "  ./configure"
    echo "  make"
    exit 1
fi

echo "✓ libmodsecurity found"

# Get compiler flags
echo ""
echo "Detecting compiler flags..."

CXX="${CXX:-g++}"
CXXFLAGS="${CXXFLAGS:--std=c++17 -O3 -DNDEBUG}"

# Get Google Benchmark flags
if pkg-config --exists benchmark; then
    BENCHMARK_CFLAGS="$(pkg-config --cflags benchmark)"
    BENCHMARK_LIBS="$(pkg-config --libs benchmark)"
    echo "✓ Found Google Benchmark via pkg-config"
else
    # Try common locations
    for prefix in /usr/local /opt/homebrew /usr; do
        if [ -f "${prefix}/include/benchmark/benchmark.h" ]; then
            BENCHMARK_CFLAGS="-I${prefix}/include"
            BENCHMARK_LIBS="-L${prefix}/lib -lbenchmark"
            echo "✓ Found Google Benchmark at ${prefix}"
            break
        fi
    done

    if [ -z "${BENCHMARK_CFLAGS}" ]; then
        echo "✗ Could not find Google Benchmark headers!"
        exit 1
    fi
fi

# Get PCRE flags
if pkg-config --exists libpcre2-8; then
    PCRE_CFLAGS="$(pkg-config --cflags libpcre2-8)"
    PCRE_LIBS="$(pkg-config --libs libpcre2-8)"
elif pkg-config --exists libpcre; then
    PCRE_CFLAGS="$(pkg-config --cflags libpcre)"
    PCRE_LIBS="$(pkg-config --libs libpcre)"
else
    PCRE_CFLAGS=""
    PCRE_LIBS="-lpcre"
fi

# Get YAJL flags if available
if pkg-config --exists yajl; then
    YAJL_LIBS="$(pkg-config --libs yajl)"
else
    YAJL_LIBS="-lyajl"
fi

# Build command
echo ""
echo "Building benchmark_rules..."
echo "Compiler: ${CXX}"
echo "Flags: ${CXXFLAGS}"

BUILD_CMD="${CXX} ${CXXFLAGS} \
    -I${REPO_ROOT}/headers \
    ${BENCHMARK_CFLAGS} \
    ${PCRE_CFLAGS} \
    ${REPO_ROOT}/test/benchmark/benchmark_rules.cc \
    -L${REPO_ROOT}/src/.libs \
    -lmodsecurity \
    ${PCRE_LIBS} \
    ${YAJL_LIBS} \
    ${BENCHMARK_LIBS} \
    -lpthread \
    -o ${REPO_ROOT}/test/benchmark/benchmark_rules"

echo ""
echo "Executing: ${BUILD_CMD}"
echo ""

eval ${BUILD_CMD}

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ Build successful!"
    echo ""
    echo "Run the benchmarks:"
    echo "  cd test/benchmark"
    echo "  ./benchmark_rules"
    echo ""
    echo "Or with options:"
    echo "  ./benchmark_rules --benchmark_filter=SingleHeader"
    echo "  ./benchmark_rules --benchmark_format=json"
    echo ""
else
    echo ""
    echo "✗ Build failed!"
    exit 1
fi
