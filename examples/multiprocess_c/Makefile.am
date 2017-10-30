

noinst_PROGRAMS = multi

multi_SOURCES = \
	multi.c

multi_LDADD = \
	-L$(top_builddir)/src/.libs/ \
	-lmodsecurity \
	-lpthread \
	$(YAJL_LDFLAGS) \
	$(GEOIP_LDFLAGS) \
	$(SSDEEP_LDFLAGS) $(SSDEEP_LDADD) \
	$(LUA_LDFLAGS) $(LUA_LDADD) \
	$(GLOBAL_LDADD)

multi_CFLAGS = \
	-I$(top_builddir)/headers \
	-I$(top_builddir) \
	$(GLOBAL_CFLAGS)

MAINTAINERCLEANFILES = \
	Makefile.in


