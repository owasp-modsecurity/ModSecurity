#!/bin/bash



git clone https://github.com/SpiderLabs/owasp-modsecurity-crs.git owasp-v2


echo 'Include "owasp-v2/base_rules/*.conf"' >> basic_rules.conf
echo 'Include "owasp-v2/optional_rules/*.conf"' >> basic_rules.conf

# Content injection not support in modsec v3
rm owasp-v2/optional_rules/modsecurity_crs_43_csrf_protection.conf

# WEBSERVER_ERROR_LOG is not supported in v3.
cat owasp-v2/base_rules/modsecurity_crs_20_protocol_violations.conf | sed 's/SecRule WEBSERVER_ERROR_LOG/#SecRule WEBSERVER_ERROR_LOG/g' > owasp-v2/base_rules/modsecurity_crs_20_protocol_violations.conf.tmp
mv owasp-v2/base_rules/modsecurity_crs_20_protocol_violations.conf.tmp owasp-v2/base_rules/modsecurity_crs_20_protocol_violations.conf

# Apache specific configuration.
cat owasp-v2/optional_rules/modsecurity_crs_49_header_tagging.conf | sed 's/RequestHeader/#RequestHeader/g' > owasp-v2/optional_rules/modsecurity_crs_49_header_tagging.conf.tmp
mv owasp-v2/optional_rules/modsecurity_crs_49_header_tagging.conf.tmp owasp-v2/optional_rules/modsecurity_crs_49_header_tagging.conf

cat owasp-v2/optional_rules/modsecurity_crs_55_application_defects.conf | sed 's/Header edit/#Header edit/g' > owasp-v2/optional_rules/modsecurity_crs_55_application_defects.conf.tmp
mv owasp-v2/optional_rules/modsecurity_crs_55_application_defects.conf.tmp owasp-v2/optional_rules/modsecurity_crs_55_application_defects.conf

echo "Done."

