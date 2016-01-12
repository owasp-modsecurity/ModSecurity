#!/bin/bash



git clone https://github.com/SpiderLabs/owasp-modsecurity-crs.git owasp-v2


echo 'Include "owasp-v2/base_rules/*.conf"' >> basic_rules.conf


cat owasp-v2/base_rules/modsecurity_crs_20_protocol_violations.conf | sed 's/SecRule WEBSERVER_ERROR_LOG/#SecRule WEBSERVER_ERROR_LOG/g' > owasp-v2/base_rules/modsecurity_crs_20_protocol_violations.conf.tmp
mv owasp-v2/base_rules/modsecurity_crs_20_protocol_violations.conf.tmp owasp-v2/base_rules/modsecurity_crs_20_protocol_violations.conf


echo "Done."

