

noinst_PROGRAMS = multi

multi_SOURCES = \
	multi.c

multi_LDADD = \
	-L$(top_builddir)/src/.libs/ \
	-lmodsecurity \
	$(YAJL_LDFLAGS) \
	$(GEOIP_LDFLAGS) \
	$(GLOBAL_LDADD)

multi_CFLAGS = \
	-I$(top_builddir)/headers \
	-I$(top_builddir) \
	$(GLOBAL_CFLAGS)

MAINTAINERCLEANFILES = \
	Makefile.in


