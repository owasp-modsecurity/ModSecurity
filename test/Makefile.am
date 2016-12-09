
if AFL_FUZZER
export MAYBE_AFL_FUZZER = fuzzer
endif


SUBDIRS = \
	benchmark \
	$(MAYBE_AFL_FUZZER)


# make clean
CLEANFILES = 

# make maintainer-clean
MAINTAINERCLEANFILES = \
	Makefile.in


bin_PROGRAMS =
noinst_PROGRAMS =

EXTRA_DIST = \
	test-cases/*


# unit_tests

noinst_PROGRAMS += unit_tests
unit_tests_SOURCES = \
        unit/unit.cc \
        unit/unit_test.cc

noinst_HEADERS = \
       common/modsecurity_test.cc \
       common/*.h \
       unit/*.h \
       regression/*.h

unit_tests_LDADD = \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDFLAGS) $(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDFLAGS) $(YAJL_LDADD) \
	$(LMDB_LDFLAGS) $(LMDB_LDADD) \
	$(LIBXML2_LDADD) \
	$(GLOBAL_LDADD)


unit_tests_CPPFLAGS = \
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
	$(LMDB_CFLAGS) \
	$(PCRE_CFLAGS) \
	$(YAJL_CFLAGS) \
	$(LIBXML2_CFLAGS)


# regression

noinst_PROGRAMS += regression_tests
regression_tests_SOURCES = \
        regression/regression.cc \
        regression/regression_test.cc \
        regression/custom_debug_log.cc

regression_tests_LDADD = \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDFLAGS) $(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDFLAGS) $(YAJL_LDADD) \
	$(LMDB_LDFLAGS) $(LMDB_LDADD) \
	$(LIBXML2_LDADD) \
	$(GLOBAL_LDADD)


regression_tests_CPPFLAGS = \
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
	$(LMDB_CFLAGS) \
	$(PCRE_CFLAGS) \
	$(YAJL_CFLAGS) \
	$(LIBXML2_CFLAGS)


# optimization


noinst_PROGRAMS += rules_optimization
rules_optimization_SOURCES = \
        optimization/optimization.cc

rules_optimization_LDADD = \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDFLAGS) $(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDFLAGS) $(YAJL_LDADD) \
	$(LMDB_LDFLAGS) $(LMDB_LDADD) \
	$(LIBXML2_LDADD) \
	$(GLOBAL_LDADD)


rules_optimization_CPPFLAGS = \
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
	$(LMDB_CFLAGS) \
	$(PCRE_CFLAGS) \
	$(YAJL_CFLAGS) \
	$(LIBXML2_CFLAGS)

