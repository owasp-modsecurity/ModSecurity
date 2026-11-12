#!/bin/sh
# YAJL-enabled original library; output directory must remain outside sources.
set -eu
repo=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
out=${1:?usage: build-extended-audit.sh /absolute/output/dir [original/build/root]}
build=${2:-$repo}
mkdir -p "$out"
${CXX:-c++} -std=c++17 -DWITH_YAJL ${CPPFLAGS:-} \
  $(pkg-config --cflags yajl) -I"$(dirname "$(pkg-config --variable=includedir yajl)")" \
  -I"$repo/headers" "$repo/test/json-baseline/extended_audit_probe.cc" \
  -L"$build/src/.libs" ${LDFLAGS:-} -Wl,-rpath,"$build/src/.libs" \
  -lmodsecurity -o "$out/extended-audit-probe"
printf '%s\n' "Set LD_LIBRARY_PATH=$build/src/.libs:\$LD_LIBRARY_PATH for execution."
