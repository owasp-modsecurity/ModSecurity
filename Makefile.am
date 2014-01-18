ACLOCAL_AMFLAGS = -I build
SUBDIRS = @TOPLEVEL_SUBDIRS@ tests

CLEANFILES =
MAINTAINERCLEANFILES =

CLEANFILES += tests/regression/server_root/conf/*.t_*.conf \
    tests/regression/server_root/logs/*.log

MAINTAINERCLEANFILES += $(CLEANFILES) \
    aclocal.m4 \
    alp2/Makefile.in \
    apache2/Makefile.in \
    build/config.guess \
    build/config.sub \
    build/depcomp \
    build/libtool.m4 \
    build/ltmain.sh \
    build/lt~obsolete.m4 \
    build/ltoptions.m4 \
    build/ltsugar.m4 \
    build/ltversion.m4 \
    build/missing \
    config.log \
    config.status \
    configure \
    ext/Makefile.in \
    Makefile \
    Makefile.in \
    mlogc/Makefile.in \
    modsecurity_config_auto.h.in~

test: check

test-regression:
	(cd tests && $(MAKE) test-regression)

test-regression-nginx:
	(cd tests && $(MAKE) test-regression-nginx)


cppcheck:
	cppcheck . --enable=all --force 2>&1 | sed 's/^/warning: /g' 1>&2;

check-coding-style:
	for i in `(find . -iname "*.c" ; find . -iname "*.h")`; \
	    do echo $$i...; \
	    vera++ -rule L004 -param max-line-length=80 $$i 2>&1 | sed 's/^/warning: /g' 1>&2; \
	    vera++ -rule L001 $$i 2>&1 | sed 's/^/warning: /g' 1>&2; \
	done; 

.PHONY: test
