
bin_SCRIPTS = mlogc-batch-load.pl

bin_PROGRAMS = mlogc

mlogc_SOURCES = mlogc.c

mlogc_CPPFLAGS = @APR_CPPFLAGS@ \
    @PCRE_CPPFLAGS@ \
    @CURL_CPPFLAGS@ \
    -I$(top_srcdir)/apache2

mlogc_CFLAGS = @APR_CFLAGS@ \
    @CURL_CFLAGS@ \
    @PCRE_CFLAGS@
    
mlogc_LDFLAGS = @APR_LDFLAGS@ \
    @CURL_LDFLAGS@ \
    @PCRE_LDFLAGS@
    
mlogc_LDADD = @APR_LDADD@ \
    @CURL_LDADD@ \
    @PCRE_LDADD@
