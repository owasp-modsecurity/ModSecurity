MOD_SECURITY2 = mod_security2 apache2_config apache2_io apache2_util \
    re re_operators re_actions re_tfns re_variables \
    msc_logging msc_xml msc_multipart modsecurity msc_parsers msc_util msc_pcre \
    persist_dbm msc_reqbody pdf_protect msc_geo msc_gsb msc_crypt msc_tree msc_unicode acmp msc_lua

H = re.h modsecurity.h msc_logging.h msc_multipart.h msc_parsers.h \
    msc_pcre.h msc_util.h msc_xml.h persist_dbm.h apache2.h pdf_protect.h \
    msc_geo.h msc_gsb.h msc_crypt.h msc_tree.h msc_unicode.h acmp.h utf8tables.h msc_lua.h

${MOD_SECURITY2:=.slo}: ${H}
${MOD_SECURITY2:=.lo}: ${H}
${MOD_SECURITY2:=.o}: ${H}

mod_security2.la: ${MOD_SECURITY2:=.slo}
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version ${MOD_SECURITY2:=.lo}

DISTCLEAN_TARGETS = modules.mk

shared =  mod_security2.la
