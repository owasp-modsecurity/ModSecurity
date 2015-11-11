

bin_PROGRAMS = test

test_SOURCES = \
	test.c

test_LDADD = \
	-L$(top_builddir)/src/.libs/ -lmodsecurity $(YAJL_LDFLAGS) $(GEOIP_LDFLAGS)

test_CFLAGS = \
	-I$(top_builddir)/headers \
	-I$(top_builddir)

MAINTAINERCLEANFILES = \
	Makefile.in


