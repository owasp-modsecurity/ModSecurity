ACLOCAL_AMFLAGS = -I build
SUBDIRS = @TOPLEVEL_SUBDIRS@ tests

CLEANFILES =
MAINTAINERCLEANFILES =

CLEANFILES += tests/regression/server_root/conf/*.t_*.conf \
   tests/regression/server_root/logs/*.log

MAINTAINERCLEANFILES += $(CLEANFILES) \
       Makefile.in \
       aclocal.m4 \
       alp2/Makefile.in \
       apache2/Makefile.in \
       build/config.guess \
       build/config.sub \
       build/depcomp \
       build/libtool.m4 \
       build/ltmain.sh \
       build/ltoptions.m4 \
       build/ltsugar.m4 \
       build/ltversion.m4 \
       build/lt~obsolete.m4 \
       build/missing \
       configure \
       ext/Makefile.in \
       mlogc/Makefile.in \
       modsecurity_config_auto.h.in~ \
       config.log \
       Makefile \
       config.status

# Alias for "check"
test: check

test-regression:
	(cd tests && $(MAKE) test-regression)

.PHONY: test
