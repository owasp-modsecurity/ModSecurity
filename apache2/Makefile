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
top_dir      = /apps/apache22

top_srcdir   = ${top_dir}
top_builddir = ${top_dir}

include ${top_builddir}/build/special.mk

APXS      = apxs
APACHECTL = apachectl

INCLUDES = -I /usr/include/libxml2
DEFS = -DWITH_LIBXML2
#LIBS = -Lmy/lib/dir -lmylib

CFLAGS = -O2 -g -Wuninitialized -Wall -Wmissing-prototypes -Wshadow -Wunused-variable -Wunused-value -Wchar-subscripts -Wsign-compare

all: local-shared-build

clean:
	-rm -f *.o *.lo *.slo *.la *~ .libs
    
