

bin_PROGRAMS = benchmark

benchmark_SOURCES = \
        benchmark.cc

benchmark_LDADD = \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDADD) \
	$(YAJL_LDADD)

benchmark_CPPFLAGS = \
	-std=c++11 \
	-I$(top_builddir)/headers

MAINTAINERCLEANFILES = \
        Makefile.in

