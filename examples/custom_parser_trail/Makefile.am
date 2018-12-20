

noinst_PROGRAMS = parser

parser_SOURCES = \
	parser.cc

parser_LDADD = \
	$(SSDEEP_LDADD) \
	$(LUA_LDADD) \
	$(MAXMIND_LDADD) \
	$(GLOBAL_LDADD)

parser_LDFLAGS = \
	-L$(top_builddir)/src/.libs/ \
	$(GEOIP_LDFLAGS) \
	-lmodsecurity \
	-lpthread \
	-lm \
	-lstdc++ \
	$(LUA_LDFLAGS) \
	$(SSDEEP_LDFLAGS) \
	$(MAXMIND_LDFLAGS) \
	$(YAJL_LDFLAGS)

parser_CPPFLAGS = \
	-I$(top_builddir)/headers \
	-I$(top_builddir) \
	$(GLOBAL_CFLAGS)

MAINTAINERCLEANFILES = \
	Makefile.in


