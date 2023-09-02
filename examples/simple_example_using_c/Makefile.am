

noinst_PROGRAMS = test

test_SOURCES = \
	test.c

test_LDADD = \
	$(top_builddir)/src/libmodsecurity.la \
	$(GLOBAL_LDADD) \
	$(LUA_LDADD) \
	$(SSDEEP_LDADD)

test_LDFLAGS = \
	$(GEOIP_LDFLAGS) \
	-lm \
	-lstdc++ \
	$(LUA_LDFLAGS) \
	$(SSDEEP_LDFLAGS) \
	$(YAJL_LDFLAGS)

test_CFLAGS = \
	-I$(top_builddir)/headers \
	-I$(top_builddir) \
	$(GLOBAL_CFLAGS)

MAINTAINERCLEANFILES = \
	Makefile.in


