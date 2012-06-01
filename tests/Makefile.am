check_PROGRAMS = msc_test
msc_test_SOURCES = msc_test.c \
                   $(top_srcdir)/apache2/re.c \
                   $(top_srcdir)/apache2/re_operators.c \
                   $(top_srcdir)/apache2/re_actions.c \
                   $(top_srcdir)/apache2/re_tfns.c \
                   $(top_srcdir)/apache2/re_variables.c \
                   $(top_srcdir)/apache2/msc_logging.c \
                   $(top_srcdir)/apache2/msc_xml.c \
                   $(top_srcdir)/apache2/msc_multipart.c \
                   $(top_srcdir)/apache2/modsecurity.c \
                   $(top_srcdir)/apache2/msc_parsers.c \
                   $(top_srcdir)/apache2/msc_util.c \
                   $(top_srcdir)/apache2/msc_pcre.c \
                   $(top_srcdir)/apache2/msc_unicode.c \
                   $(top_srcdir)/apache2/persist_dbm.c \
                   $(top_srcdir)/apache2/msc_reqbody.c \
                   $(top_srcdir)/apache2/msc_crypt.c \
                   $(top_srcdir)/apache2/msc_tree.c \
                   $(top_srcdir)/apache2/msc_geo.c \
                   $(top_srcdir)/apache2/msc_gsb.c \
                   $(top_srcdir)/apache2/acmp.c \
                   $(top_srcdir)/apache2/msc_lua.c \
                   $(top_srcdir)/apache2/msc_release.c
msc_test_CFLAGS = @APXS_CFLAGS@ @APR_CFLAGS@ @APU_CFLAGS@ \
                  @PCRE_CFLAGS@ @LIBXML2_CFLAGS@ @MODSEC_EXTRA_CFLAGS@ @LUA_CFLAGS@
msc_test_CPPFLAGS = -I$(top_srcdir)/apache2 \
                    @APR_CPPFLAGS@ @LIBXML2_CPPFLAGS@ \
                    @PCRE_CPPFLAGS@
msc_test_LDADD = @APR_LDADD@ @APU_LDADD@ \
                 @PCRE_LDADD@ @LIBXML2_LDADD@ @LUA_LDADD@
msc_test_LDFLAGS = @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                   @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@

check_SCRIPTS = run-unit-tests.pl
TESTS = $(check_SCRIPTS)

test: check

test-regression: run-regression-tests.pl
	$(PERL) run-regression-tests.pl

.PHONY: test test-regression
