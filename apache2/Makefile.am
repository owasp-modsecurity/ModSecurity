pkglibdir = $(prefix)/lib
pkglib_LTLIBRARIES = mod_security2.la

mod_security2_la_SOURCES = acmp.c \
    apache2_config.c \
    apache2_io.c \
    apache2_util.c \
    libinjection/libinjection_html5.c \
    libinjection/libinjection_sqli.c \
    libinjection/libinjection_xss.c \
    mod_security2.c \
    modsecurity.c \
    msc_status_engine.c \
    msc_crypt.c \
    msc_geo.c \
    msc_gsb.c \
    msc_json.c \
    msc_logging.c \
    msc_lua.c \
    msc_multipart.c \
    msc_parsers.c \
    msc_pcre.c \
    msc_release.c \
    msc_reqbody.c \
    msc_tree.c \
    msc_unicode.c \
    msc_util.c \
    msc_xml.c \
    persist_dbm.c \
    re_actions.c \
    re.c \
    re_operators.c \
    re_tfns.c \
    re_variables.c

mod_security2_la_CFLAGS = @APR_CFLAGS@ \
    @APU_CFLAGS@ \
    @APXS_CFLAGS@ \
    @CURL_CFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LUA_CFLAGS@ \
    @MODSEC_EXTRA_CFLAGS@ \
    @PCRE_CFLAGS@ \
    @YAJL_CFLAGS@ \
    @SSDEEP_CFLAGS@


mod_security2_la_CPPFLAGS = @APR_CPPFLAGS@ \
    @CURL_CPPFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_CPPFLAGS@ \
    @PCRE_CPPFLAGS@

mod_security2_la_LIBADD = @APR_LDADD@ \
    @APU_LDADD@ \
    @CURL_LDADD@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_LDADD@ \
    @LUA_LDADD@ \
    @PCRE_LDADD@ \
    @YAJL_LDADD@

if AIX
mod_security2_la_LDFLAGS = -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if HPUX
mod_security2_la_LDFLAGS = -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if MACOSX
mod_security2_la_LDFLAGS = -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if SOLARIS
mod_security2_la_LDFLAGS = -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if LINUX
mod_security2_la_LDFLAGS = -no-undefined -module -avoid-version -R @PCRE_LD_PATH@ \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if FREEBSD
mod_security2_la_LDFLAGS = -no-undefined -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if OPENBSD
mod_security2_la_LDFLAGS = -no-undefined -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if NETBSD
mod_security2_la_LDFLAGS = -no-undefined -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @LIBXML2_CFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@
endif

if LINUX
install-exec-hook: $(pkglib_LTLIBRARIES)
	@echo "Removing unused static libraries..."; \
	for m in $(pkglib_LTLIBRARIES); do \
	  base=`echo $$m | sed 's/\..*//'`; \
	  rm -f $(DESTDIR)$(pkglibdir)/$$base.*a; \
	  install -D -m444 $(DESTDIR)$(pkglibdir)/$$base.so $(DESTDIR)$(APXS_MODULES)/$$base.so; \
	done
else
install-exec-hook: $(pkglib_LTLIBRARIES)
	@echo "Removing unused static libraries..."; \
	for m in $(pkglib_LTLIBRARIES); do \
	  base=`echo $$m | sed 's/\..*//'`; \
	  rm -f $(DESTDIR)$(pkglibdir)/$$base.*a; \
	  cp -p $(DESTDIR)$(pkglibdir)/$$base.so $(DESTDIR)$(APXS_MODULES); \
	done
endif
