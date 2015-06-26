

bin_PROGRAMS = benchmark

benchmark_SOURCES = \
        benchmark.cc

benchmark_LDADD = \
        $(top_builddir)/src/.libs/libmodsecurity.a

benchmark_CPPFLAGS = \
        -std=c++11 \
		-I$(top_builddir)/headers

MAINTAINERCLEANFILES = \
        Makefile.in

