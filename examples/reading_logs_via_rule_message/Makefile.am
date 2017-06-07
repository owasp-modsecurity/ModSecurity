

noinst_PROGRAMS = simple_request

simple_request_SOURCES = \
	simple_request.cc

simple_request_LDADD = \
	$(top_builddir)/src/.libs/libmodsecurity.a \
	-lpthread \
	$(CURL_LDADD) \
	$(GEOIP_LDFLAGS) $(GEOIP_LDADD) \
	$(PCRE_LDADD) \
	$(YAJL_LDFLAGS) $(YAJL_LDADD) \
	$(LMDB_LDFLAGS) $(LMDB_LDADD) \
	$(LIBXML2_LDADD) \
	$(GLOBAL_LDADD)


simple_request_CPPFLAGS = \
	$(GLOBAL_CFLAGS) \
	-std=c++11 \
	-I$(top_builddir)/headers \
	-I$(top_builddir) \
	-g \
	-I../others \
	-fPIC \
	-O3 \
	$(GEOIP_CFLAGS) \
	$(GLOBAL_CPPFLAGS) \
	$(MODSEC_NO_LOGS) \
	$(YAJL_CFLAGS) \
	$(LMDB_CFLAGS) \
	$(PCRE_CFLAGS) \
	$(LIBXML2_CFLAGS)


MAINTAINERCLEANFILES = \
	Makefile.in


