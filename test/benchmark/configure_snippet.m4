# Google Benchmark Detection Snippet
# Add this to configure.ac to enable automatic detection of Google Benchmark
#
# Usage: Copy the content below to configure.ac, ideally after the LUA check
# and before the "Files to be generated via autotools" section.

# ==============================================================================
# Check for Google Benchmark
# ==============================================================================

AC_ARG_WITH([benchmark],
    [AS_HELP_STRING([--with-benchmark],
        [enable Google Benchmark support @<:@default=check@:>@])],
    [],
    [with_benchmark=check])

HAVE_GOOGLE_BENCHMARK=0
BENCHMARK_LDADD=""
BENCHMARK_CFLAGS=""

AS_IF([test "x$with_benchmark" != "xno"],
    [
        # Try pkg-config first
        PKG_CHECK_MODULES([BENCHMARK], [benchmark >= 1.5.0],
            [
                HAVE_GOOGLE_BENCHMARK=1
                AC_DEFINE([HAVE_GOOGLE_BENCHMARK], [1],
                          [Define if you have Google Benchmark])
                BENCHMARK_LDADD="${BENCHMARK_LIBS}"
                BENCHMARK_CFLAGS="${BENCHMARK_CFLAGS}"
            ],
            [
                # Fallback to library check
                AC_CHECK_LIB([benchmark], [main],
                    [
                        AC_CHECK_HEADER([benchmark/benchmark.h],
                            [
                                HAVE_GOOGLE_BENCHMARK=1
                                BENCHMARK_LDADD="-lbenchmark -lpthread"
                                AC_DEFINE([HAVE_GOOGLE_BENCHMARK], [1],
                                          [Define if you have Google Benchmark])
                            ],
                            [
                                if test "x$with_benchmark" != "xcheck"; then
                                    AC_MSG_FAILURE([--with-benchmark was given, but benchmark headers not found])
                                fi
                            ])
                    ],
                    [
                        if test "x$with_benchmark" != "xcheck"; then
                            AC_MSG_FAILURE([--with-benchmark was given, but benchmark library not found])
                        fi
                    ])
            ])
    ])

AC_SUBST([BENCHMARK_LDADD])
AC_SUBST([BENCHMARK_CFLAGS])
AM_CONDITIONAL([HAVE_GOOGLE_BENCHMARK], [test "$HAVE_GOOGLE_BENCHMARK" = "1"])

# ==============================================================================
# End of Google Benchmark check
# ==============================================================================
