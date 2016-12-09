

noinst_PROGRAMS = benchmark

benchmark_SOURCES = \
        benchmark.cc

benchmark_LDADD = \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDFLAGS) $(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDFLAGS) $(YAJL_LDADD) \
	$(LMDB_LDFLAGS) $(LMDB_LDADD) \
	$(LIBXML2_LDADD) \
	$(GLOBAL_LDADD)

benchmark_CPPFLAGS = \
	-std=c++11 \
	-I$(top_builddir)/headers \
	$(GLOBAL_CPPFLAGS) \
	$(PCRE_CFLAGS) \
	$(LMDB_CFLAGS) \
	$(LIBXML2_CFLAGS)

MAINTAINERCLEANFILES = \
        Makefile.in

