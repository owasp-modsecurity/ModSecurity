#!/bin/sh
# Run after the ordinary YAJL-enabled project build. Outputs stay outside sources.
set -eu
repo=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
out=${1:?usage: build-probes.sh /absolute/output/directory}
mkdir -p "$out"
# Honor normal caller CPPFLAGS/LDFLAGS and pkg-config relocation.
# Intentional word splitting for compiler flag variables follows configure/make.
${CXX:-c++} -std=c++17 -DWITH_YAJL ${CPPFLAGS:-} \
  $(pkg-config --cflags yajl) -I"$(dirname "$(pkg-config --variable=includedir yajl)")" -I"$repo/headers" -I"$repo" \
  "$repo/test/json-baseline/probe.cc" -L"$repo/src/.libs" \
  ${LDFLAGS:-} -Wl,-rpath,"$repo/src/.libs" -lmodsecurity -o "$out/probe"
${CXX:-c++} -std=c++17 -DWITH_YAJL ${CPPFLAGS:-} \
  $(pkg-config --cflags yajl) -I"$(dirname "$(pkg-config --variable=includedir yajl)")" -I"$repo/headers" \
  "$repo/test/json-baseline/audit_probe.cc" -L"$repo/src/.libs" \
  ${LDFLAGS:-} -Wl,-rpath,"$repo/src/.libs" -lmodsecurity -o "$out/audit-probe"
printf '%s\n' "Run with LD_LIBRARY_PATH=$repo/src/.libs:\$LD_LIBRARY_PATH (source build first)."
