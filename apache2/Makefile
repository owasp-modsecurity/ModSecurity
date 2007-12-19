builddir     = .

# To successfully build ModSecurity for Apache you will
# need to configure the "top_dir" variable properly. The
# correct value will differ from system to system.
#
# If you've installed Apache manually simply point to the
# installation directory. Most pre-packaged installations
# consist of two parts. One contains the binaries, and the
# other contains the "development" files. You will typically
# need both.
#
# The list below may help:
#
#     Fedora Core - /usr/lib/httpd (the httpd-devel package must be installed)
#
#     Debian - /usr/share/apache2  (apache2-prefork-dev or apache2-threaded-dev
#                                   needed, depending on your installation type)
#
# NOTE: On Solaris the top_builddir is /var/apache2.  This version of Apache
# httpd uses the buildin PCRE, but you must still install the PCRE headers
# to compile ModSecurity as they are not installed with Apache httpd. Because
# of this, it is recommended to install your own version of Apache httpd.
#
# Additionally, if you do not have PCRE installed, you can use the
# headers included with Apache httpd.  To do this, add the PCRE
# include directory to the INCLUDES variable below
# Something like: -I /path/to/httpd-x.y/srclib/pcre
#
top_dir      = /usr/local/apache2

top_srcdir   = ${top_dir}
top_builddir = ${top_dir}

include ${top_builddir}/build/special.mk

APXS      = apxs
APACHECTL = apachectl

INCLUDES = -I /usr/include/libxml2
#INCLUDES = -I /usr/include/libxml2 -I /path/to/httpd-x.y/srclib/pcre
DEFS = -DWITH_LIBXML2
# DEFS = -DWITH_ICONV
#DEFS = -DWITH_LIBXML2 -DPERFORMANCE_MEASUREMENT
#DEFS = -DWITH_LIBXML2 -DNO_MODSEC_API
#DEFS = -DWITH_LIBXML2 -DDEBUG_CONF
#DEFS = -DWITH_LIBXML2 -DCACHE_DEBUG
#LIBS = -Lmy/lib/dir -lmylib

CFLAGS = -O2 -g -Wuninitialized -Wall -Wmissing-prototypes -Wshadow -Wunused-variable -Wunused-value -Wchar-subscripts -Wsign-compare

all: local-shared-build

### Experimental Test Framework (UNIX only right now)
TESTOBJS = \
    re.o re_operators.o re_actions.o re_tfns.o re_variables.o \
    msc_logging.o msc_xml.o msc_multipart.o modsecurity.o msc_parsers.o msc_util.o msc_pcre.o \
    persist_dbm.o msc_reqbody.o msc_geo.o acmp.o

msc-test: msc-test.c ${TESTOBJS}
	@${CC} -I .. -I `${top_dir}/bin/${APXS} -q INCLUDEDIR` ${INCLUDES} ${DEFS} -L ${top_dir}/lib -o msc-test ${TESTOBJS} msc-test.c -lapr-1 -laprutil-1 -lpcre -lxml2

test: msc-test
	t/run-tests.pl

clean:
	-rm -f *.o *.lo *.slo *.la *~ .libs msc-test

.PHONY: all clean test
