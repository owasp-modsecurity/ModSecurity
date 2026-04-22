

noinst_PROGRAMS = \
	benchmark \
	json_benchmark

EXTRA_DIST = \
	basic_rules.conf \
	json_benchmark_rules.conf \
	run-json-benchmarks.sh \
	download-owasp-v3-rules.sh \
	download-owasp-v4-rules.sh

benchmark_SOURCES = \
        benchmark.cc

json_benchmark_SOURCES = \
	json_benchmark.cc

benchmark_LDADD = \
	$(CURL_LDADD) \
	$(GEOIP_LDADD) \
	$(MAXMIND_LDADD) \
	$(PCRE_LDADD) \
	$(PCRE2_LDADD) \
	$(LMDB_LDADD) \
	$(SSDEEP_LDADD) \
	$(LUA_LDADD) \
	$(LIBXML2_LDADD) \
	$(GLOBAL_LDADD)

benchmark_LDFLAGS = \
	-L$(top_builddir)/src/.libs/ \
	$(GEOIP_LDFLAGS) \
	-lmodsecurity \
	-lpthread \
	-lm \
	-lstdc++ \
	$(GEOIP_LDFLAGS) \
	$(MAXMIND_LDFLAGS) \
	$(LMDB_LDFLAGS) \
	$(SSDEEP_LDFLAGS) \
	$(LUA_LDFLAGS)

benchmark_CPPFLAGS = \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/headers \
	$(GLOBAL_CPPFLAGS) \
	$(PCRE_CFLAGS) \
	$(PCRE2_CFLAGS) \
	$(LMDB_CFLAGS) \
	$(LIBXML2_CFLAGS)

json_benchmark_LDADD = \
	$(benchmark_LDADD)

json_benchmark_LDFLAGS = \
	$(benchmark_LDFLAGS)

json_benchmark_CPPFLAGS = \
	$(benchmark_CPPFLAGS) \
	-DMSC_JSON_BENCHMARK_RULES_DIR=\"$(srcdir)\"

MAINTAINERCLEANFILES = \
        Makefile.in
