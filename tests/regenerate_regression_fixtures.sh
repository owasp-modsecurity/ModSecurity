#!/usr/bin/env bash
# Regenerates tests/regression/fixtures/ from tests/regression/*/*.t via
# dump_regression_fixtures.pl. Run this after editing/adding a .t file.
#
# Usage: tests/regenerate_regression_fixtures.sh [output_dir]
#   output_dir defaults to tests/regression/fixtures (in place).
#   Pass a scratch dir (e.g. in CI) to check for staleness with:
#     diff -r tests/regression/fixtures "$scratch_dir"
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUT_DIR="${1:-$SCRIPT_DIR/regression/fixtures}"

command -v perl >/dev/null || { echo "perl is required" >&2; exit 1; }
perl -MLWP::UserAgent -e 1 2>/dev/null || {
    echo "Perl module LWP::UserAgent is required (apt: libwww-perl)" >&2
    exit 1
}

rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR"

status=0
for t_file in "$SCRIPT_DIR"/regression/*/*.t; do
    type_dir="$(basename "$(dirname "$t_file")")"
    name="$(basename "$t_file" .t)"
    mkdir -p "$OUT_DIR/$type_dir"
    if ! perl "$SCRIPT_DIR/dump_regression_fixtures.pl" "$t_file" > "$OUT_DIR/$type_dir/$name.json"; then
        echo "FAILED to dump $t_file" >&2
        status=1
    fi
done

exit "$status"
