
BUILT_SOURCES= \
	parser/seclang-parser.cc

libmodsecuritydir = $(prefix)/lib
libmodsecurity_LTLIBRARIES = libmodsecurity.la
libmodsecurity_ladir = $(prefix)/include

CLEANFILES = \
	location.hh \
	position.hh \
	parser/seclang-parser.cc \
	parser/seclang-parser.hh \
	parser/seclang-scanner.cc \
	stack.hh


MAINTAINERCLEANFILES = \
	Makefile.in \
	config.h.in \
	config.h.in~


pkginclude_HEADERS = \
	../headers/modsecurity/modsecurity.h \
	../headers/modsecurity/assay.h \
	../headers/modsecurity/rules.h \
	../headers/modsecurity/debug_log.h \
	../headers/modsecurity/intervention.h


ACTIONS = \
	actions/action.cc \
	actions/block.cc \
	actions/phase.cc \
	actions/redirect.cc \
	actions/rule_id.cc \
	actions/status.cc \
	actions/transformations/base64_decode.cc \
	actions/transformations/base64_decode_ext.cc \
	actions/transformations/cmd_line.cc \
	actions/transformations/compress_whitespace.cc \
	actions/transformations/css_decode.cc \
	actions/transformations/escape_seq_decode.cc \
	actions/transformations/hex_decode.cc \
	actions/transformations/hex_encode.cc \
	actions/transformations/html_entity_decode.cc \
	actions/transformations/js_decode.cc \
	actions/transformations/length.cc \
	actions/transformations/lowercase.cc \
	actions/transformations/md5.cc \
	actions/transformations/none.cc \
	actions/transformations/normalise_path.cc \
	actions/transformations/normalise_path_win.cc \
	actions/transformations/parity_even_7bit.cc \
	actions/transformations/parity_odd_7bit.cc \
	actions/transformations/parity_zero_7bit.cc \
	actions/transformations/remove_comments.cc \
	actions/transformations/remove_comments_char.cc \
	actions/transformations/remove_nulls.cc \
	actions/transformations/remove_whitespace.cc \
	actions/transformations/replace_comments.cc \
	actions/transformations/replace_nulls.cc \
	actions/transformations/sha1.cc \
	actions/transformations/sql_hex_decode.cc \
	actions/transformations/transformation.cc \
	actions/transformations/trim.cc \
	actions/transformations/trim_left.cc \
	actions/transformations/trim_right.cc \
	actions/transformations/url_decode.cc \
	actions/transformations/url_decode_uni.cc \
	actions/transformations/url_encode.cc \
	actions/transformations/utf8_to_unicode.cc


libmodsecurity_la_SOURCES = \
	parser/seclang-parser.yy \
	parser/seclang-scanner.ll \
	parser/driver.cc \
	assay.cc \
	modsecurity.cc \
	rules.cc \
	utils.cc \
	debug_log.cc \
	rule.cc \
	variable.cc \
	operators/operator.cc \
	operators/detect_sqli.cc \
	operators/detect_xss.cc \
	operators/inspect_file.cc \
	operators/fuzzy_hash.cc \
	operators/validate_byte_range.cc \
	operators/validate_dtd.cc \
	operators/validate_hash.cc \
	operators/validate_schema.cc \
	operators/validate_url_encoding.cc \
	operators/validate_utf8_encoding.cc \
	operators/verify_cc.cc \
	operators/verify_cpf.cc \
	operators/verify_ssn.cc \
	operators/geolookup.cc \
	operators/gsblookup.cc \
	operators/rsub.cc \
	operators/within.cc \
	operators/contains_word.cc \
	operators/contains.cc \
	operators/ends_with.cc \
	operators/eq.cc \
	operators/ge.cc \
	operators/gt.cc \
	operators/ip_match_f.cc \
	operators/ip_match.cc \
	operators/ip_match_from_file.cc \
	operators/le.cc \
	operators/lt.cc \
	operators/pm_f.cc \
	operators/pm.cc \
	operators/pm_from_file.cc \
	operators/rbl.cc \
	operators/rx.cc \
	operators/str_eq.cc \
	operators/str_match.cc \
	operators/begins_with.cc \
	${ACTIONS}




libmodsecurity_la_CFLAGS = 


libmodsecurity_la_CPPFLAGS = \
	-std=c++11 \
	-I.. \
	-g \
	-O0 \
	-I ../headers


libmodsecurity_la_LIBADD = \
	@LEXLIB@


libmodsecurity_la_LDFLAGS = \
	-version-info @MSC_VERSION_INFO@


