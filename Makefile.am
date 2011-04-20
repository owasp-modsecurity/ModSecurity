ACLOCAL_AMFLAGS = -I build
SUBDIRS = @TOPLEVEL_SUBDIRS@ tests

# Alias for "check"
test: check

test-regression:
	(cd tests && $(MAKE) test-regression)

.PHONY: test
