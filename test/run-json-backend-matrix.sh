#!/usr/bin/env bash

set -u

usage() {
    cat <<'EOF'
Usage: test/run-json-backend-matrix.sh [--build-root DIR] [--jobs N] [--configure-extra "ARGS"] [--keep-build-dirs]

Exit codes:
  0  success
 10  configure failure
 11  build failure
 12  test failure
 13  backend result difference
 64  invalid usage
EOF
}

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_root="${repo_root}/build-json-backend-matrix"
jobs="$(getconf _NPROCESSORS_ONLN 2>/dev/null || printf '1')"
configure_extra=""
keep_build_dirs=0

while [ "$#" -gt 0 ]; do
    case "$1" in
        --build-root)
            shift
            [ "$#" -gt 0 ] || { usage; exit 64; }
            build_root="$1"
            ;;
        --jobs)
            shift
            [ "$#" -gt 0 ] || { usage; exit 64; }
            jobs="$1"
            ;;
        --configure-extra)
            shift
            [ "$#" -gt 0 ] || { usage; exit 64; }
            configure_extra="$1"
            ;;
        --keep-build-dirs)
            keep_build_dirs=1
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

declare -a configure_extra_args=()
if [ -n "${configure_extra}" ]; then
    read -r -a configure_extra_args <<< "${configure_extra}"
fi

readonly test_files=(
    "test/test-cases/regression/request-body-parser-json.json"
    "test/test-cases/regression/request-body-parser-json-backend-edgecases.json"
)

mkdir -p "${build_root}/logs"

extract_summary() {
    local backend="$1"
    local input_log="$2"
    local output_tsv="$3"

    awk -v backend="${backend}" '
        match($0, /^:test-result:[[:space:]]+([^[:space:]]+)[[:space:]]+([^:]+):(.*)$/, m) {
            print backend "\t" m[2] "\t" m[3] "\t" m[1];
        }
    ' "${input_log}" > "${output_tsv}"
}

run_backend() {
    local backend="$1"
    local build_dir="${build_root}/${backend}"
    local raw_log="${build_root}/logs/${backend}.log"
    local summary_file="${build_root}/logs/${backend}.summary.tsv"
    local configure_status=0
    local build_status=0
    local test_status=0

    if [ "${keep_build_dirs}" -eq 0 ]; then
        rm -rf "${build_dir}"
    fi
    mkdir -p "${build_dir}"
    : > "${raw_log}"

    (
        cd "${build_dir}" && \
        "${repo_root}/configure" \
            --with-json-backend="${backend}" \
            "${configure_extra_args[@]}"
    ) >> "${raw_log}" 2>&1
    configure_status=$?
    if [ "${configure_status}" -ne 0 ]; then
        return 10
    fi

    (
        cd "${build_dir}" && \
        make -j "${jobs}" -C others
    ) >> "${raw_log}" 2>&1
    build_status=$?
    if [ "${build_status}" -ne 0 ]; then
        return 11
    fi

    (
        cd "${build_dir}" && \
        make -j "${jobs}" -C src libmodsecurity.la
    ) >> "${raw_log}" 2>&1
    build_status=$?
    if [ "${build_status}" -ne 0 ]; then
        return 11
    fi

    (
        cd "${build_dir}" && \
        make -j "${jobs}" -C test regression_tests
    ) >> "${raw_log}" 2>&1
    build_status=$?
    if [ "${build_status}" -ne 0 ]; then
        return 11
    fi

    for test_file in "${test_files[@]}"; do
        (
            cd "${build_dir}/test" && \
            ./regression_tests automake "${repo_root}/${test_file}"
        ) >> "${raw_log}" 2>&1
        if [ "$?" -ne 0 ]; then
            test_status=1
        fi
    done

    extract_summary "${backend}" "${raw_log}" "${summary_file}"
    if [ ! -s "${summary_file}" ]; then
        test_status=1
    fi
    if awk -F '\t' '$4 != "PASS" {exit 1}' "${summary_file}"; then
        :
    else
        test_status=1
    fi

    if [ "${test_status}" -ne 0 ]; then
        return 12
    fi
    return 0
}

backend_test_failure=0
for backend in simdjson jsoncons; do
    run_backend "${backend}"
    status=$?
    case "${status}" in
        0)
            ;;
        10|11)
            exit "${status}"
            ;;
        12)
            backend_test_failure=1
            ;;
        *)
            exit "${status}"
            ;;
    esac
done

cat "${build_root}/logs/simdjson.summary.tsv" \
    "${build_root}/logs/jsoncons.summary.tsv" \
    > "${build_root}/summary.tsv"

if diff -u \
    <(cut -f 2- "${build_root}/logs/simdjson.summary.tsv" | sort) \
    <(cut -f 2- "${build_root}/logs/jsoncons.summary.tsv" | sort) \
    > "${build_root}/logs/backend-diff.log"; then
    :
else
    exit 13
fi

if [ "${backend_test_failure}" -ne 0 ]; then
    exit 12
fi

exit 0
