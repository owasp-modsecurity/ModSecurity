
noinst_LTLIBRARIES = libinjection.la libmbedtls.la

libinjection_la_SOURCES = \
	libinjection/src/libinjection_html5.c \
	libinjection/src/libinjection_sqli.c \
	libinjection/src/libinjection_xss.c

libinjection_la_CFLAGS = -D LIBINJECTION_VERSION=\"${LIBINJECTION_VERSION}\"
libinjection_la_LIBADD =

noinst_HEADERS = \
	libinjection/src/libinjection.h \
	libinjection/src/libinjection_html5.h \
	libinjection/src/libinjection_sqli.h \
	libinjection/src/libinjection_sqli_data.h \
	libinjection/src/libinjection_xss.h \
	mbedtls/tf-psa-crypto/include/mbedtls/base64.h \
	mbedtls/tf-psa-crypto/drivers/builtin/include/mbedtls/private/check_config.h \
	mbedtls/include/mbedtls/mbedtls_config.h \
	mbedtls/tf-psa-crypto/drivers/builtin/include/mbedtls/private/md5.h \
	mbedtls/tf-psa-crypto/include/mbedtls/platform.h \
	mbedtls/tf-psa-crypto/drivers/builtin/include/mbedtls/private/sha1.h

libmbedtls_la_SOURCES = \
	mbedtls/tf-psa-crypto/drivers/builtin/src/base64.c \
	mbedtls/tf-psa-crypto/drivers/builtin/src/md5.c \
	mbedtls/tf-psa-crypto/drivers/builtin/src/sha1.c \
	mbedtls/tf-psa-crypto/drivers/builtin/src/platform_util.c

libmbedtls_la_CFLAGS = -DMBEDTLS_CONFIG_FILE=\"mbedtls/mbedtls_config.h\" -I$(top_srcdir)/others/mbedtls/include
libmbedtls_la_CPPFLAGS =
libmbedtls_la_LIBADD =
