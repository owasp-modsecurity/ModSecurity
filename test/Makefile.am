
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
	test-cases/* \
	custom-test-driver \
	test-suite.sh \
	run-json-backend-matrix.sh


# unit_tests

noinst_PROGRAMS += unit_tests
unit_tests_SOURCES = \
        unit/unit.cc \
        unit/unit_test.cc \
        common/custom_debug_log.cc

noinst_PROGRAMS += json_backend_depth_tests
json_backend_depth_tests_SOURCES = \
        unit/json_backend_depth_tests.cc


noinst_HEADERS = \
       $(srcdir)/common/colors.h \
       $(srcdir)/common/custom_debug_log.h \
       $(srcdir)/common/json.h \
       $(srcdir)/common/modsecurity_test.h \
       $(srcdir)/common/modsecurity_test_context.h \
       $(srcdir)/common/modsecurity_test_results.h \
       $(srcdir)/unit/unit_test.h \
       $(srcdir)/regression/regression_test.h


unit_tests_LDADD = \
	$(CURL_LDADD) \
	$(GEOIP_LDADD) \
	$(MAXMIND_LDADD) \
	$(GLOBAL_LDADD) \
	$(LIBXML2_LDADD) \
	$(LMDB_LDADD) \
	$(LUA_LDADD) \
	$(PCRE_LDADD) \
	$(PCRE2_LDADD) \
	$(SSDEEP_LDADD)


unit_tests_LDFLAGS = \
	-L$(top_builddir)/src/.libs/ \
	$(GEOIP_LDFLAGS) \
	-lmodsecurity \
	-lpthread \
	-lm \
	-lstdc++ \
	$(MAXMIND_LDFLAGS) \
	$(LMDB_LDFLAGS) \
	$(LUA_LDFLAGS) \
	$(SSDEEP_LDFLAGS)


unit_tests_CPPFLAGS = \
	-Icommon \
	-I$(top_srcdir)/ \
	-I$(top_srcdir)/others/jsoncons/include \
	-g \
	-I$(top_srcdir)/headers \
	$(CURL_CFLAGS) \
	$(MODSEC_NO_LOGS) \
	$(GEOIP_CFLAGS) \
	$(MAXMIND_CFLAGS) \
	$(GLOBAL_CPPFLAGS) \
	$(LMDB_CFLAGS) \
	$(PCRE_CFLAGS) \
	$(PCRE2_CFLAGS) \
	$(LUA_CFLAGS) \
	$(SSDEEP_CFLAGS) \
	$(LIBXML2_CFLAGS)

json_backend_depth_tests_LDADD = \
	$(unit_tests_LDADD)

json_backend_depth_tests_LDFLAGS = \
	$(unit_tests_LDFLAGS)

json_backend_depth_tests_CPPFLAGS = \
	$(unit_tests_CPPFLAGS)


# regression

noinst_PROGRAMS += regression_tests
regression_tests_SOURCES = \
        regression/regression.cc \
        regression/regression_test.cc \
        common/custom_debug_log.cc

regression_tests_LDADD = \
	$(CURL_LDADD) \
	$(GEOIP_LDADD) \
	$(MAXMIND_LDADD) \
	$(GLOBAL_LDADD) \
	$(LIBXML2_LDADD) \
	$(LMDB_LDADD) \
	$(LUA_LDADD) \
	$(PCRE_LDADD) \
	$(PCRE2_LDADD) \
	$(SSDEEP_LDADD)


regression_tests_LDFLAGS = \
	-L$(top_builddir)/src/.libs/ \
	$(GEOIP_LDFLAGS) \
	-lmodsecurity \
	-lpthread \
	-lm \
	-lstdc++ \
	$(MAXMIND_LDFLAGS) \
	$(LMDB_LDFLAGS) \
	$(SSDEEP_LDFLAGS) \
	$(LUA_LDFLAGS)


regression_tests_CPPFLAGS = \
	-Icommon \
	-I$(top_srcdir) \
	-I$(top_srcdir)/others/jsoncons/include \
	-g \
	-I$(top_srcdir)/headers \
	$(CURL_CFLAGS) \
	$(MODSEC_NO_LOGS) \
	$(GEOIP_CFLAGS) \
	$(MAXMIND_CFLAGS) \
	$(GLOBAL_CPPFLAGS) \
	$(LMDB_CFLAGS) \
	$(LUA_CFLAGS) \
	$(SSDEEP_CFLAGS) \
	$(PCRE_CFLAGS) \
	$(PCRE2_CFLAGS) \
	$(LIBXML2_CFLAGS)


# optimization


noinst_PROGRAMS += rules_optimization
rules_optimization_SOURCES = \
        optimization/optimization.cc

rules_optimization_LDADD = \
	$(CURL_LDADD) \
	$(GEOIP_LDADD) \
	$(MAXMIND_LDADD) \
	$(GLOBAL_LDADD) \
	$(LIBXML2_LDADD) \
	$(LMDB_LDADD) \
	$(LUA_LDADD) \
	$(PCRE_LDADD) \
	$(PCRE2_LDADD) \
	$(SSDEEP_LDADD)

rules_optimization_LDFLAGS = \
	-L$(top_builddir)/src/.libs/ \
	$(GEOIP_LDFLAGS) \
	-lmodsecurity \
	-lpthread \
	-lm \
	-lstdc++ \
	$(MAXMIND_LDFLAGS) \
	$(LMDB_LDFLAGS) \
	$(LUA_LDFLAGS) \
	$(SSDEEP_LDFLAGS)

rules_optimization_CPPFLAGS = \
	-Icommon \
	-I$(top_srcdir)/ \
	-g \
	-I$(top_srcdir)/headers \
	$(CURL_CFLAGS) \
	$(MODSEC_NO_LOGS) \
	$(GEOIP_CFLAGS) \
	$(MAXMIND_CFLAGS) \
	$(GLOBAL_CPPFLAGS) \
	$(LMDB_CFLAGS) \
	$(LUA_CFLAGS) \
	$(SSDEEP_CFLAGS) \
	$(PCRE_CFLAGS) \
	$(PCRE2_CFLAGS) \
	$(LIBXML2_CFLAGS)
