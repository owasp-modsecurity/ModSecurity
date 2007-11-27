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
# On Solaris the top_builddir is /var/apache2.  This version of Apache
# httpd uses the buildin PCRE, but you must still install the PCRE headers
# to compile ModSecurity as they are not installed with Apache httpd. Because
# of this, it is recommended to install your own version of Apache httpd.
#
# Additionally, if you do not have PCRE installed, you can use the
# headers included with Apache httpd.  To do this, add the PCRE
# include directory to the INCLUDES variable below
# Something like: -I /path/to/httpd-x.y/srclib/pcre
#
# XML Support:  Please note that the included Core Ruleset may assume XML
# support is available (-DWITH_LIBXML2).  You may have to manually remove any
# XML references in the Core Ruleset if you choose not to include XML support.
# In future versions of ModSecurity XML support will be required.
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
#DEFS = -DWITH_LIBXML2 -DNO_MODSEC_API
#LIBS = -Lmy/lib/dir -lmylib

CFLAGS = -O2 -g -Wuninitialized -Wall -Wmissing-prototypes -Wshadow -Wunused-variable -Wunused-value -Wchar-subscripts -Wsign-compare

all: local-shared-build

clean:
	-rm -f *.o *.lo *.slo *.la *~ .libs
    
