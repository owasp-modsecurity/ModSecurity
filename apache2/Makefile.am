pkglibdir = $(prefix)/lib
pkglib_LTLIBRARIES = mod_security2.la
#include_HEADERS = re.h modsecurity.h msc_logging.h msc_multipart.h \
#                  msc_parsers.h msc_pcre.h msc_util.h msc_xml.h \
#                  persist_dbm.h apache2.h msc_geo.h acmp.h utf8tables.h \
#                  msc_lua.h msc_release.h

mod_security2_la_SOURCES = mod_security2.c \
                           apache2_config.c apache2_io.c apache2_util.c \
                           re.c re_operators.c re_actions.c re_tfns.c \
                           re_variables.c msc_logging.c msc_xml.c \
                           msc_multipart.c modsecurity.c msc_parsers.c \
                           msc_util.c msc_pcre.c persist_dbm.c msc_reqbody.c \
                           msc_geo.c msc_gsb.c msc_crypt.c msc_tree.c msc_unicode.c acmp.c msc_lua.c msc_release.c

mod_security2_la_CFLAGS = @APXS_CFLAGS@ @APR_CFLAGS@ @APU_CFLAGS@ \
                          @PCRE_CFLAGS@ @LIBXML2_CFLAGS@ @LUA_CFLAGS@ @MODSEC_EXTRA_CFLAGS@ @CURL_CFLAGS@
mod_security2_la_CPPFLAGS = @APR_CPPFLAGS@ @PCRE_CPPFLAGS@ @LIBXML2_CPPFLAGS@
mod_security2_la_LIBADD = @APR_LDADD@ @APU_LDADD@ @PCRE_LDADD@ @LIBXML2_LDADD@ @LUA_LDADD@

if AIX
mod_security2_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if HPUX
mod_security2_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if MACOSX
mod_security2_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if SOLARIS
mod_security2_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if LINUX
mod_security2_la_LDFLAGS = -no-undefined -module -avoid-version -R @PCRE_LD_PATH@ \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if FREEBSD
mod_security2_la_LDFLAGS = -no-undefined -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if OPENBSD
mod_security2_la_LDFLAGS = -no-undefined -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if NETBSD
mod_security2_la_LDFLAGS = -no-undefined -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if LINUX
install-exec-hook: $(pkglib_LTLIBRARIES)
	@echo "Removing unused static libraries..."; \
	for m in $(pkglib_LTLIBRARIES); do \
	  base=`echo $$m | sed 's/\..*//'`; \
	  rm -f $(DESTDIR)$(pkglibdir)/$$base.*a; \
	  install -D -m444 $(DESTDIR)$(pkglibdir)/$$base.so $(DESTDIR)$(APXS_MODULES); \
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
