#!/usr/bin/env bash
set -euo pipefail
VERSION="${1:-v3.11.3}"
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
OUT_DIR="$ROOT_DIR/third_party/nlohmann_json/include/nlohmann"
mkdir -p "$OUT_DIR"
URL="https://raw.githubusercontent.com/nlohmann/json/${VERSION}/single_include/nlohmann/json.hpp"
curl -L --fail "$URL" -o "$OUT_DIR/json.hpp"
echo "Imported nlohmann/json ${VERSION} into $OUT_DIR/json.hpp"
