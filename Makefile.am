ACLOCAL_AMFLAGS = -I acinclude
SUBDIRS = @TOPLEVEL_SUBDIRS@ tests

# Alias for "check"
test: check

test-regression:
	(cd tests && $(MAKE) test-regression)

.PHONY: test
