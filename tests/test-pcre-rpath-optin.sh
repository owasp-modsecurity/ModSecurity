#!/bin/sh
# Test PCRE/PCRE2 configure flags without installing or starting Apache.
# Automake supplies this script from srcdir for in-tree and VPATH builds.
set -eu
for cmd in autoconf grep sed wc; do
    if ! command -v "$cmd" >/dev/null 2>&1; then
        printf 'SKIP: missing %s\n' "$cmd"
        exit 77
    fi
done
src=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
tmp=$(mktemp -d "${TMPDIR:-/tmp}/modsec-pcre-rpath-test.XXXXXX")
trap 'rm -rf "$tmp"' EXIT
trap 'exit 1' HUP INT TERM
cp "$src/build/find_pcre.m4" "$src/build/find_pcre2.m4" "$tmp/"
cat > "$tmp/configure.ac" <<'AC'
AC_INIT([modsecurity-pcre-rpath-test], [1.0])
AC_CONFIG_SRCDIR([configure.ac])
m4_include([find_pcre.m4])
m4_include([find_pcre2.m4])
verbose_output=0
CHECK_PCRE
CHECK_PCRE2
AC_CONFIG_FILES([flags make-line])
AC_OUTPUT
AC
cat > "$tmp/flags.in" <<'IN'
PCRE_RPATH_FLAGS=@PCRE_RPATH_FLAGS@
PCRE_LDADD=@PCRE_LDADD@
PCRE2_LDADD=@PCRE2_LDADD@
IN
sed -n '/^if LINUX$/,/^endif$/p' "$src/apache2/Makefile.am" | \
    grep '^mod_security2_la_LDFLAGS = ' > "$tmp/make-line.in"
if test "$(wc -l < "$tmp/make-line.in")" -ne 1; then
    echo 'FAIL: expected exactly one Linux module linker assignment' >&2
    exit 1
fi
cat > "$tmp/fake-pcre-config" <<'CONFIG'
#!/bin/sh
case "$1" in
    --version) echo 8.45 ;;
    --cflags) : ;;
    --libs) printf '%s\n' "$TEST_PCRE_LIBS" ;;
    *) exit 1 ;;
esac
CONFIG
cat > "$tmp/fake-pcre2-config" <<'CONFIG'
#!/bin/sh
case "$1" in
    --version) echo 10.46 ;;
    --cflags) : ;;
    --libs8) printf '%s\n' "$TEST_PCRE2_LIBS" ;;
    *) exit 1 ;;
esac
CONFIG
chmod +x "$tmp/fake-pcre-config" "$tmp/fake-pcre2-config"
(cd "$tmp" && autoconf)

try_ok() {
    label=$1 mode=$2 libs=$3 want=$4
    shift 4
    mkdir "$tmp/$label"
    if test "$mode" = pcre1; then
        config_arg="--with-pcre=$tmp/fake-pcre-config"
    else
        config_arg="--with-pcre2=$tmp/fake-pcre2-config"
    fi
    (cd "$tmp/$label" && TEST_PCRE_LIBS="$libs" TEST_PCRE2_LIBS="$libs" \
        ../configure "$config_arg" "$@" >configure.stdout 2>configure.stderr)
    actual=$(sed -n 's/^PCRE_RPATH_FLAGS=//p' "$tmp/$label/flags")
    test "$actual" = "$want" || {
        echo "FAIL: $label expected [$want] got [$actual]" >&2
        exit 1
    }
    if test "$mode" = pcre1; then
        linked=$(sed -n 's/^PCRE_LDADD=//p' "$tmp/$label/flags")
    else
        linked=$(sed -n 's/^PCRE2_LDADD=//p' "$tmp/$label/flags")
    fi
    if test "$linked" != "$libs"; then
        echo "FAIL: $label altered PCRE linker arguments" >&2
        exit 1
    fi
    if test -z "$want"; then
        if grep -E '(^|[[:space:]])-R([[:space:]]|$)' "$tmp/$label/make-line" >/dev/null; then
            echo "FAIL: $label has unexpected explicit -R" >&2
            exit 1
        fi
    elif ! grep -F -- "$want" "$tmp/$label/make-line" >/dev/null; then
        echo "FAIL: $label did not pass the explicit -R flag" >&2
        exit 1
    fi
    echo "PASS $label"
}
try_bad() {
    label=$1 option=$2
    mkdir "$tmp/$label"
    if (cd "$tmp/$label" && TEST_PCRE2_LIBS='-lpcre2-8' \
        ../configure "--with-pcre2=$tmp/fake-pcre2-config" "$option" \
        >configure.stdout 2>configure.stderr); then
        echo "FAIL: $label incorrectly accepted $option" >&2
        exit 1
    fi
    if ! grep -F -- '--with-pcre-rpath' "$tmp/$label/configure.stderr" >/dev/null; then
        echo "FAIL: $label did not fail on --with-pcre-rpath validation" >&2
        exit 1
    fi
    echo "PASS $label (invalid option rejected)"
}

# Default PCRE and PCRE2 flags must never invent a Libtool -R.
try_ok pcre2_system pcre2 '-lpcre2-8' ''
try_ok pcre2_long_path pcre2 '-L/opt/a/b/c/d/lib -lpcre2-8' ''
try_ok pcre2_repeated_slashes pcre2 '-L/tmp/////protected/lib -lpcre2-8' ''
try_ok pcre2_multiple_L pcre2 '-L/tmp/first -L/opt/second -lpcre2-8' ''
try_ok pcre2_split_L pcre2 '-L /opt/private/lib -lpcre2-8' ''
try_ok pcre1_system pcre1 '-lpcre' ''
try_ok pcre1_long_path pcre1 '-L/opt/a/b/c/d/lib -lpcre' ''
try_ok pcre1_repeated_slashes pcre1 '-L/tmp/////protected/lib -lpcre' ''

# No option and explicit opt-out have the same safe behavior.
try_ok pcre2_disabled pcre2 '-lpcre2-8' '' '--without-pcre-rpath'
try_ok pcre1_disabled pcre1 '-lpcre' '' '--with-pcre-rpath=no'

# Opt-in is explicit for both PCRE APIs, including staged target directories.
try_ok pcre2_explicit pcre2 '-lpcre2-8' '-R /opt/nonexistent-target-pcre/lib' \
    '--with-pcre-rpath=/opt/nonexistent-target-pcre/lib'
try_ok pcre1_explicit pcre1 '-lpcre' '-R /opt/nonexistent-target-pcre/lib' \
    '--with-pcre-rpath=/opt/nonexistent-target-pcre/lib'
try_bad relative_path '--with-pcre-rpath=relative/lib'
try_bad unsafe_semicolon '--with-pcre-rpath=/tmp;extra'
try_bad multiple_paths '--with-pcre-rpath=/opt/one:/opt/two'
try_bad missing_value '--with-pcre-rpath=yes'
try_bad empty_value '--with-pcre-rpath='
try_bad unsafe_space '--with-pcre-rpath=/opt/unsafe directory'
echo 'PASS all PCRE RPATH configure regression cases'
