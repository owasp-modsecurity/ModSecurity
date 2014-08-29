check_PROGRAMS = msc_test
msc_test_SOURCES = msc_test.c \
    ../apache2/acmp.c \
    ../apache2/libinjection/libinjection_html5.c \
    ../apache2/libinjection/libinjection_sqli.c \
    ../apache2/libinjection/libinjection_xss.c \
    ../apache2/modsecurity.c \
    ../apache2/msc_status_engine.c \
    ../apache2/msc_crypt.c \
    ../apache2/msc_geo.c \
    ../apache2/msc_gsb.c \
    ../apache2/msc_json.c \
    ../apache2/msc_logging.c \
    ../apache2/msc_lua.c \
    ../apache2/msc_multipart.c \
    ../apache2/msc_parsers.c \
    ../apache2/msc_pcre.c \
    ../apache2/msc_release.c \
    ../apache2/msc_reqbody.c \
    ../apache2/msc_tree.c \
    ../apache2/msc_unicode.c \
    ../apache2/msc_util.c \
    ../apache2/msc_xml.c \
    ../apache2/persist_dbm.c \
    ../apache2/re_actions.c \
    ../apache2/re.c \
    ../apache2/re_operators.c \
    ../apache2/re_tfns.c \
    ../apache2/re_variables.c \
    ../standalone/regex.c \
    ../standalone/server.c

msc_test_CFLAGS = @APR_CFLAGS@ \
    @APU_CFLAGS@ \
    @APXS_CFLAGS@ \
    @CURL_CFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LUA_CFLAGS@ \
    @MODSEC_EXTRA_CFLAGS@ \
    @PCRE_CFLAGS@ \
    @YAJL_CFLAGS@ \
    @SSDEEP_CFLAGS@
    
msc_test_CPPFLAGS = -I$(top_srcdir)/apache2 \
    @APR_CPPFLAGS@ \
    @CURL_CPPFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_CPPFLAGS@ \
    @PCRE_CPPFLAGS@
    
msc_test_LDADD = @APR_LDADD@ \
    @APU_LDADD@ \
    @CURL_LDADD@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_LDADD@ \
    @LUA_LDADD@ \
    @PCRE_LDADD@ \
    @YAJL_LDADD@ \
    @SSDEEP_CFLAGS@

msc_test_LDFLAGS = @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@

check_SCRIPTS = run-unit-tests.pl
TESTS = $(check_SCRIPTS)

test: check

test-regression: run-regression-tests.pl
	$(PERL) run-regression-tests.pl

test-regression-nginx: run-regression-tests-nginx.pl
	$(PERL) run-regression-tests-nginx.pl

.PHONY: test test-regression
