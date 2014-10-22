pkglibdir = $(prefix)/lib

pkglib_LTLIBRARIES = standalone.la

standalone_la_SOURCES = ../apache2/acmp.c \
    ../apache2/apache2_config.c \
    ../apache2/apache2_io.c \
    ../apache2/apache2_util.c \
    ../apache2/libinjection/libinjection_html5.c \
    ../apache2/libinjection/libinjection_sqli.c \
    ../apache2/libinjection/libinjection_xss.c \
    ../apache2/mod_security2.c \
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
    ../apache2/msc_remote_rules.c \
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
    api.c \
    buckets.c \
    config.c \
    filters.c \
    hooks.c \
    regex.c \
    server.c

    # FIXME: Standalone does not mean that it will be a nginx build.
standalone_la_CFLAGS = -DVERSION_NGINX \
    @APR_CFLAGS@ \
    @APU_CFLAGS@ \
    @APXS_CFLAGS@ \
    @CURL_CFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LUA_CFLAGS@ \
    @MODSEC_EXTRA_CFLAGS@ \
    @PCRE_CFLAGS@ \
    @YAJL_CFLAGS@ \
    @SSDEEP_CFLAGS@

standalone_la_CPPFLAGS = @APR_CPPFLAGS@ \
    @LIBXML2_CPPFLAGS@ \
    @PCRE_CPPFLAGS@
    
standalone_la_LIBADD = @APR_LDADD@ \
    @APU_LDADD@ \
    @LIBXML2_LDADD@ \
    @LUA_LDADD@ \
    @PCRE_LDADD@ \
    @YAJL_LDADD@ \
    @SSDEEP_CFLAGS@

if AIX
standalone_la_LDFLAGS = -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if HPUX
standalone_la_LDFLAGS = -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if MACOSX
standalone_la_LDFLAGS = -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if SOLARIS
standalone_la_LDFLAGS = -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if LINUX
standalone_la_LDFLAGS = -no-undefined -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if FREEBSD
standalone_la_LDFLAGS = -no-undefined -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if OPENBSD
standalone_la_LDFLAGS = -no-undefined -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if NETBSD
standalone_la_LDFLAGS = -no-undefined -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

