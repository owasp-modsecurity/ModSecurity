pkglibdir = $(prefix)/lib
pkglib_LTLIBRARIES = standalone.la
#include_HEADERS = re.h modsecurity.h msc_logging.h msc_multipart.h \
#                  msc_parsers.h msc_pcre.h msc_util.h msc_xml.h \
#                  persist_dbm.h apache2.h msc_geo.h acmp.h utf8tables.h \
#                  msc_lua.h msc_release.h

standalone_la_SOURCES = ../apache2/mod_security2.c \
                           ../apache2/apache2_config.c ../apache2/apache2_io.c ../apache2/apache2_util.c \
                           ../apache2/re.c ../apache2/re_operators.c ../apache2/re_actions.c ../apache2/re_tfns.c \
                           ../apache2/re_variables.c ../apache2/msc_logging.c ../apache2/msc_xml.c \
                           ../apache2/msc_multipart.c ../apache2/modsecurity.c ../apache2/msc_parsers.c \
                           ../apache2/msc_util.c ../apache2/msc_pcre.c ../apache2/persist_dbm.c ../apache2/msc_reqbody.c \
     			   ../apache2/msc_geo.c ../apache2/msc_gsb.c ../apache2/msc_unicode.c \
     			   ../apache2/acmp.c ../apache2/msc_lua.c ../apache2/msc_release.c \
     			   ../apache2/msc_crypt.c ../apache2/msc_tree.c \
                           api.c buckets.c \
                           config.c filters.c \
                           hooks.c \
                           regex.c server.c

standalone_la_CFLAGS = @APXS_CFLAGS@ @APR_CFLAGS@ @APU_CFLAGS@ \
                          @PCRE_CFLAGS@ @LIBXML2_CFLAGS@ @LUA_CFLAGS@ @MODSEC_EXTRA_CFLAGS@ @CURL_CFLAGS@ -DVERSION_NGINX
standalone_la_CPPFLAGS = @APR_CPPFLAGS@ @PCRE_CPPFLAGS@ @LIBXML2_CPPFLAGS@
standalone_la_LIBADD = @APR_LDADD@ @APU_LDADD@ @PCRE_LDADD@ @LIBXML2_LDADD@ @LUA_LDADD@

if AIX
standalone_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if HPUX
standalone_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if MACOSX
standalone_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if SOLARIS
standalone_la_LDFLAGS = -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if LINUX
standalone_la_LDFLAGS = -no-undefined -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if FREEBSD
standalone_la_LDFLAGS = -no-undefined -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if OPENBSD
standalone_la_LDFLAGS = -no-undefined -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

if NETBSD
standalone_la_LDFLAGS = -no-undefined -module -avoid-version \
                           @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
                           @PCRE_LDFLAGS@ @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@
endif

install-exec-hook: $(pkglib_LTLIBRARIES)
	@echo "Creating Nginx config file..."; \
	rm -f ../nginx/modsecurity/config; \
	echo "ngx_addon_name=ngx_http_modsecurity" >> ../nginx/modsecurity/config; \
	echo "CORE_MODULES=\"\$$CORE_MODULES ngx_pool_context_module\"" >> ../nginx/modsecurity/config; \
	echo "HTTP_AUX_FILTER_MODULES=\"ngx_http_modsecurity \$$HTTP_AUX_FILTER_MODULES\"" >> ../nginx/modsecurity/config; \
	echo "NGX_ADDON_SRCS=\"\$$NGX_ADDON_SRCS \$$ngx_addon_dir/ngx_http_modsecurity.c \$$ngx_addon_dir/apr_bucket_nginx.c \$$ngx_addon_dir/ngx_pool_context.c\"" >> ../nginx/modsecurity/config;\
	echo "NGX_ADDON_DEPS=\"\$$NGX_ADDON_DEPS \$$ngx_addon_dir/apr_bucket_nginx.h \$$ngx_addon_dir/ngx_pool_context.h\"" >> ../nginx/modsecurity/config; \
	echo "CORE_LIBS=\"\$$CORE_LIBS \$$ngx_addon_dir/../../standalone/.libs/standalone.a -lapr-1 -laprutil-1 -lxml2 -lm @LUA_LDADD@\"" >> ../nginx/modsecurity/config; \
	echo "CORE_INCS=\"\$$CORE_INCS /usr/include/apache2 /usr/include/apr-1.0 /usr/include/httpd /usr/include/apr-1 \$$ngx_addon_dir \$$ngx_addon_dir/../../standalone \$$ngx_addon_dir/../../apache2 /usr/include/libxml2 `echo @LUA_CFLAGS@ | cut -d "I" -f3`\"" >> ../nginx/modsecurity/config; \
	echo "Removing unused static libraries..."; \
	for m in $(pkglib_LTLIBRARIES); do \
	  base=`echo $$m | sed 's/\..*//'`; \
	  rm -f $(DESTDIR)$(pkglibdir)/$$base.*a; \
	  cp -p $(DESTDIR)$(pkglibdir)/$$base.so $(APXS_MODULES); \
	done
