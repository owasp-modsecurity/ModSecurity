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
                           msc_geo.c msc_gsb.c acmp.c msc_lua.c msc_release.c

mod_security2_la_CFLAGS = @APXS_CFLAGS@ @APR_CFLAGS@ @APU_CFLAGS@ \
                          @LIBXML2_CFLAGS@ @LUA_CFLAGS@
mod_security2_la_CPPFLAGS = @APR_CPPFLAGS@ @LIBXML2_CPPFLAGS@ @LUA_CPPFLAGS@
mod_security2_la_LIBADD = @APR_LDADD@ @APU_LDADD@ @LIBXML2_LDADD@ @LUA_LDADD@

if MACOSX
mod_security2_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
else
mod_security2_la_LDFLAGS = -no-undefined -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

install-exec-hook: $(pkglib_LTLIBRARIES)
	@echo "Removing unused static libraries..."; \
	for m in $(pkglib_LTLIBRARIES); do \
	  base=`echo $$m | sed 's/\..*//'`; \
	  rm -f $(DESTDIR)$(pkglibdir)/$$base.*a; \
	  cp -p $(DESTDIR)$(pkglibdir)/$$base.so $(APXS_MODULES); \
	done
