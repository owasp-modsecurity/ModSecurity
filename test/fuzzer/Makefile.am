

# make clean
CLEANFILES = 

# make maintainer-clean
MAINTAINERCLEANFILES = \
	Makefile.in


bin_PROGRAMS = afl_fuzzer

afl_fuzzer_SOURCES = \
	afl_fuzzer.cc

afl_fuzzer_LDADD = \
	$(GLOBAL_LDADD) \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDFLAGS) $(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDFLAGS) $(YAJL_LDADD)


afl_fuzzer_CPPFLAGS = \
	-std=c++11 \
	-Icommon \
	-I../ \
	-O0 \
	-g \
	-I$(top_builddir)/headers \
	$(CURL_CFLAGS) \
	$(MODSEC_NO_LOGS) \
	$(GEOIP_CFLAGS) \
	$(GLOBAL_CPPFLAGS) \
	$(PCRE_CFLAGS) \
	$(YAJL_CFLAGS)
