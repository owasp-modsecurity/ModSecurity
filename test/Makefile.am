

SUBDIRS = \
	benchmark

# make clean
CLEANFILES = 

# make maintainer-clean
MAINTAINERCLEANFILES = \
	Makefile.in



bin_PROGRAMS = unit-tests regression-tests rules-optimization


# unit_tests

unit_tests_SOURCES = \
        unit/unit.cc \
        unit/unit_test.cc

unit_tests_LDADD = \
	$(GLOBAL_LDADD) \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDFLAGS) $(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDFLAGS) $(YAJL_LDADD)

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
	$(PCRE_CFLAGS) \
	$(YAJL_CFLAGS)


# regression

regression_tests_SOURCES = \
        regression/regression.cc \
        regression/regression_test.cc \
        regression/custom_debug_log.cc

regression_tests_LDADD = \
	$(GLOBAL_LDADD) \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDFLAGS) $(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDFLAGS) $(YAJL_LDADD)

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
	$(PCRE_CFLAGS) \
	$(YAJL_CFLAGS)


# optimization

rules_optimization_SOURCES = \
        optimization/optimization.cc

rules_optimization_LDADD = \
	$(GLOBAL_LDADD) \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDFLAGS) $(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDFLAGS) $(YAJL_LDADD)

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
	$(PCRE_CFLAGS) \
	$(YAJL_CFLAGS)
