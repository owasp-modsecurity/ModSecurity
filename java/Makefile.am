pkglibdir = $(prefix)/lib
pkglib_LTLIBRARIES = libModSecurityJNI.la

libModSecurityJNI_la_SOURCES = ../apache2/mod_security2.c \
                           ../apache2/apache2_config.c ../apache2/apache2_io.c ../apache2/apache2_util.c \
                           ../apache2/re.c ../apache2/re_operators.c ../apache2/re_actions.c ../apache2/re_tfns.c \
                           ../apache2/re_variables.c ../apache2/msc_logging.c ../apache2/msc_xml.c \
                           ../apache2/msc_multipart.c ../apache2/modsecurity.c ../apache2/msc_parsers.c \
                           ../apache2/msc_util.c ../apache2/msc_pcre.c ../apache2/persist_dbm.c ../apache2/msc_reqbody.c \
     			   ../apache2/msc_geo.c ../apache2/msc_gsb.c ../apache2/msc_unicode.c \
     			   ../apache2/acmp.c ../apache2/msc_lua.c ../apache2/msc_release.c \
     			   ../apache2/msc_crypt.c ../apache2/msc_tree.c
                           ../standalone/api.c ../standalone/buckets.c \
                           ../standalone/config.c ../standalone/filters.c \
                           ../standalone/hooks.c ../standalone/regex.c ../standalone/server.c \
                           org_modsecurity_ModSecurity.c

libModSecurityJNI_la_CFLAGS = @APXS_CFLAGS@ @APR_CFLAGS@ @APU_CFLAGS@ \
                          @PCRE_CFLAGS@ @LIBXML2_CFLAGS@ @LUA_CFLAGS@ @MODSEC_EXTRA_CFLAGS@  @CURL_CFLAGS@
#libModSecurityJNI_la_CXXFLAGS = @APXS_CFLAGS@ @APR_CFLAGS@ @APU_CFLAGS@ \
#                          @PCRE_CFLAGS@ @LIBXML2_CFLAGS@ @LUA_CFLAGS@ @MODSEC_EXTRA_CFLAGS@  @CURL_CFLAGS@
libModSecurityJNI_la_CPPFLAGS = @APR_CPPFLAGS@ @PCRE_CPPFLAGS@ @LIBXML2_CPPFLAGS@ @JNI_CPPFLAGS@
libModSecurityJNI_la_LIBADD = @APR_LDADD@ @APU_LDADD@ @PCRE_LDADD@ @LIBXML2_LDADD@ @LUA_LDADD@

if AIX
libModSecurityJNI_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if HPUX
libModSecurityJNI_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if MACOSX
libModSecurityJNI_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if SOLARIS
libModSecurityJNI_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if LINUX
libModSecurityJNI_la_LDFLAGS = -no-undefined -module -avoid-version -R @PCRE_LD_PATH@ \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if FREEBSD
libModSecurityJNI_la_LDFLAGS = -no-undefined -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if OPENBSD
libModSecurityJNI_la_LDFLAGS = -no-undefined -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if NETBSD
libModSecurityJNI_la_LDFLAGS = -no-undefined -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
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
