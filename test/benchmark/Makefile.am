

bin_PROGRAMS = benchmark

benchmark_SOURCES = \
        benchmark.cc

benchmark_LDADD = \
	-lrt \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDADD)

benchmark_CPPFLAGS = \
	-std=c++11 \
	-I$(top_builddir)/headers \
	$(PCRE_CPPFLAGS)


MAINTAINERCLEANFILES = \
        Makefile.in

