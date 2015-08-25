
ACLOCAL_AMFLAGS = -I build


SUBDIRS = \
	benchmark

# make clean
CLEANFILES = 

# make maintainer-clean
MAINTAINERCLEANFILES = \
	Makefile.in



bin_PROGRAMS = unit-tests regression-tests


# unit_tests

unit_tests_SOURCES = \
        unit/unit.cc \
        unit/unit_test.cc

unit_tests_LDADD = \
	-lrt \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDADD)

unit_tests_CPPFLAGS = \
	-std=c++11 \
	-Icommon \
	-I../ \
	-O0 \
	-g \
	-I$(top_builddir)/headers \
	$(CURL_CFLAGS) \
	$(GEOIP_CFLAGS) \
	$(PCRE_CPPFLAGS) \
	$(YAJL_CFLAGS)


# regression

regression_tests_SOURCES = \
        regression/regression.cc \
        regression/regression_test.cc \
        regression/custom_debug_log.cc

regression_tests_LDADD = \
	-lrt \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	$(CURL_LDADD) \
	$(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDADD)

regression_tests_CPPFLAGS = \
	-std=c++11 \
	-Icommon \
	-I../ \
	-O0 \
	-g \
	-I$(top_builddir)/headers \
	$(CURL_CFLAGS) \
	$(GEOIP_CFLAGS) \
	$(PCRE_CPPFLAGS) \
	$(YAJL_CFLAGS)
