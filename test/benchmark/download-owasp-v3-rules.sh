#!/bin/bash


git clone https://github.com/SpiderLabs/owasp-modsecurity-crs.git owasp-v3
cd owasp-v3
git checkout v3.0.0-rc1
cd -

echo 'Include "owasp-v3/modsecurity_crs_10_setup.conf.example"' >> basic_rules.conf
echo 'Include "owasp-v3/rules/*.conf"' >> basic_rules.conf


# removes WEBSERVER_ERROR_LOG
cat owasp-v3/rules/REQUEST-20-PROTOCOL-ENFORCEMENT.conf | sed 's/SecRule WEBSERVER_ERROR_LOG/#SecRule WEBSERVER_ERROR_LOG/g' > owasp-v3/rules/REQUEST-20-PROTOCOL-ENFORCEMENT.conf.tmp
mv owasp-v3/rules/REQUEST-20-PROTOCOL-ENFORCEMENT.conf.tmp owasp-v3/rules/REQUEST-20-PROTOCOL-ENFORCEMENT.conf

echo "Done."

