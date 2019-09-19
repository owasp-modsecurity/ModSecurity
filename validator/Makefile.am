bin_PROGRAMS = validator

validator_SOURCES = main.c

    # FIXME: Standalone does not mean that it will be a nginx build.
validator_CFLAGS = -DVERSION_NGINX \
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

validator_CPPFLAGS = @APR_CPPFLAGS@ \
    @LIBXML2_CPPFLAGS@ \
    @PCRE_CPPFLAGS@

validator_LDADD = ../standalone/.libs/standalone.a \
    @APR_LDADD@ \
    @APU_LDADD@ \
    @CURL_LDADD@ \
    @LIBXML2_LDADD@ \
    @LUA_LDADD@ \
    @PCRE_LDADD@ \
    @YAJL_LDADD@ \
    -lstdc++ \
    -lm
	
validator_LDFLAGS = -no-undefined -module -avoid-version \
    @APR_LDFLAGS@ \
    @APU_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @APXS_LDFLAGS@ \
    @LIBXML2_LDFLAGS@ \
    @LUA_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @YAJL_LDFLAGS@ \
    @SSDEEP_LDFLAGS@

