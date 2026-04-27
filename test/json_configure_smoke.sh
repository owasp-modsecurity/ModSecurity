#!/usr/bin/env bash
set -euo pipefail

# English: configure resolver smoke tests.
# Deutsch: Smoke-Tests für den Configure-Resolver.

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT_DIR"

if [[ ! -x ./configure ]]; then
  autoreconf -fi
fi

git submodule update --init --recursive >/dev/null 2>&1 || true

# 1) YAJL-only support path: no new backend selected, JSON support must stay enabled.
./configure --disable-examples --with-json-c=none --with-json-cxx=none > /tmp/config-none.log 2>&1
rg -q "JSON support \(overall\).*yes \(legacy-yajl\)" /tmp/config-none.log

# 2) Explicit missing backend must fail (no fallback).
if ./configure --disable-examples --with-json-c=auto --with-json-cxx=jsoncpp >/tmp/config-jsoncpp.log 2>&1; then
  echo "Expected configure failure for explicit missing jsoncpp backend"
  exit 1
fi
rg -q "Requested C\+\+ JSON backend 'jsoncpp'" /tmp/config-jsoncpp.log

# 3) Explicit nlohmann + bundled must succeed.
./configure --disable-examples --with-json-c=none --with-json-cxx=nlohmannjson --with-json-prefer=bundled >/tmp/config-nlohmann.log 2>&1
rg -q "JSON C\+\+ backend.*nlohmannjson \(vendored\)" /tmp/config-nlohmann.log

# 4) auto + prefer=bundled keeps bundled C++ selection deterministic.
./configure --disable-examples --with-json-c=auto --with-json-cxx=auto --with-json-prefer=bundled >/tmp/config-auto-bundled.log 2>&1
rg -q "JSON C\+\+ backend.*nlohmannjson \(vendored\)" /tmp/config-auto-bundled.log

echo "json_configure_smoke: ok"
