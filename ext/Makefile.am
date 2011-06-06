EXT_CFLAGS = -I../apache2 \
			 @APXS_CFLAGS@ @APR_CFLAGS@ @APU_CFLAGS@ \
			 @LIBXML2_CFLAGS@ @LUA_CFLAGS@
EXT_CPPFLAGS = @APR_CPPFLAGS@ @LIBXML2_CPPFLAGS@
EXT_LIBADD = @APR_LDADD@ @APU_LDADD@ @LIBXML2_LDADD@ @LUA_LDADD@
EXT_LDFLAGS = -no-undefined -module -avoid-version \
			  @APR_LDFLAGS@ @APU_LDFLAGS@ @APXS_LDFLAGS@ \
			  @LIBXML2_LDFLAGS@ @LUA_LDFLAGS@

pkglibdir = $(prefix)/lib
pkglib_LTLIBRARIES = mod_op_strstr.la \
					 mod_tfn_reverse.la \
					 mod_var_remote_addr_port.la \
					 mod_reqbody_example.la

mod_op_strstr_la_SOURCES = mod_op_strstr.c
mod_op_strstr_la_CFLAGS = $(EXT_CFLAGS)
mod_op_strstr_la_CPPFLAGS = $(EXT_CPPFLAGS)
mod_op_strstr_la_LIBADD = $(EXT_LIBADD)
mod_op_strstr_la_LDFLAGS = $(EXT_LDFLAGS)

mod_tfn_reverse_la_SOURCES = mod_tfn_reverse.c
mod_tfn_reverse_la_CFLAGS = $(EXT_CFLAGS)
mod_tfn_reverse_la_CPPFLAGS = $(EXT_CPPFLAGS)
mod_tfn_reverse_la_LIBADD = $(EXT_LIBADD)
mod_tfn_reverse_la_LDFLAGS = $(EXT_LDFLAGS)

mod_var_remote_addr_port_la_SOURCES = mod_var_remote_addr_port.c
mod_var_remote_addr_port_la_CFLAGS = $(EXT_CFLAGS)
mod_var_remote_addr_port_la_CPPFLAGS = $(EXT_CPPFLAGS)
mod_var_remote_addr_port_la_LIBADD = $(EXT_LIBADD)
mod_var_remote_addr_port_la_LDFLAGS = $(EXT_LDFLAGS)

mod_reqbody_example_la_SOURCES = mod_reqbody_example.c
mod_reqbody_example_la_CFLAGS = $(EXT_CFLAGS)
mod_reqbody_example_la_CPPFLAGS = $(EXT_CPPFLAGS)
mod_reqbody_example_la_LIBADD = $(EXT_LIBADD)
mod_reqbody_example_la_LDFLAGS = $(EXT_LDFLAGS)

install-exec-hook: $(pkglib_LTLIBRARIES)
	@echo "Removing unused static libraries..."; \
	for m in $(pkglib_LTLIBRARIES); do \
	  base=`echo $$m | sed 's/\..*//'`; \
	  rm -f $(DESTDIR)$(pkglibdir)/$$base.*a; \
	  cp -p $(DESTDIR)$(pkglibdir)/$$base.so $(APXS_MODULES); \
	done
