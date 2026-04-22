#!/usr/bin/env bash

set -u

usage() {
    cat <<'EOF'
Usage: test/benchmark/run-json-benchmarks.sh --simdjson-build DIR --jsoncons-build DIR [--include-invalid]
EOF
    return
}

simdjson_build=""
jsoncons_build=""
include_invalid=0
output_file="${PWD}/json-benchmark-results.jsonl"

while [[ "$#" -gt 0 ]]; do
    case "$1" in
        --simdjson-build)
            shift
            [[ "$#" -gt 0 ]] || { usage; exit 64; }
            simdjson_build="$1"
            ;;
        --jsoncons-build)
            shift
            [[ "$#" -gt 0 ]] || { usage; exit 64; }
            jsoncons_build="$1"
            ;;
        --include-invalid)
            include_invalid=1
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            usage
            exit 64
            ;;
    esac
    shift
done

[[ -n "${simdjson_build}" ]] || { usage; exit 64; }
[[ -n "${jsoncons_build}" ]] || { usage; exit 64; }

readonly base_scenarios=(
    "large-object"
    "deep-nesting"
    "numbers"
    "utf8"
)

readonly invalid_scenarios=(
    "truncated"
    "malformed"
)

run_scenarios() {
    local build_dir="$1"
    local binary="${build_dir}/test/benchmark/json_benchmark"
    local scenario

    if [[ ! -x "${binary}" ]]; then
        echo "missing benchmark binary: ${binary}" >&2
        return 1
    fi

    for scenario in "${base_scenarios[@]}"; do
        "${binary}" --scenario "${scenario}" --output json >> "${output_file}"
    done

    if [[ "${include_invalid}" -ne 0 ]]; then
        for scenario in "${invalid_scenarios[@]}"; do
            "${binary}" --scenario "${scenario}" --include-invalid --output json >> "${output_file}"
        done
    fi

    return 0
}

: > "${output_file}"
run_scenarios "${simdjson_build}" || exit 1
run_scenarios "${jsoncons_build}" || exit 1

printf 'Wrote %s\n' "${output_file}"
