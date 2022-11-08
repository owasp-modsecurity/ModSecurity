
bin_SCRIPTS = mlogc-batch-load.pl

bin_PROGRAMS = mlogc

mlogc_SOURCES = mlogc.c

mlogc_CPPFLAGS = @APR_CPPFLAGS@ \
    @PCRE_CPPFLAGS@ \
    @PCRE2_CPPFLAGS@ \
    @CURL_CPPFLAGS@ \
    -I$(top_srcdir)/apache2

mlogc_CFLAGS = @APR_CFLAGS@ \
    @CURL_CFLAGS@ \
    @PCRE_CFLAGS@ \
    @PCRE2_CFLAGS@
    
mlogc_LDFLAGS = @APR_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @PCRE_LDFLAGS@ \
    @PCRE2_LDFLAGS@
    
mlogc_LDADD = @APR_LDADD@ \
    @CURL_LDADD@ \
    @PCRE_LDADD@ \
    @PCRE2_LDADD@
