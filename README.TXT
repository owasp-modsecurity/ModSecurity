ModSecurity for Apache 2.x, http://www.modsecurity.org/
Copyright (c) 2004-2013 Trustwave Holdings, Inc. (http://www.trustwave.com/)

You may not use this file except in compliance with
the License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

If any of the files related to licensing are missing or if you have any
other questions related to licensing please contact Trustwave Holdings, Inc.
directly using the email address security@modsecurity.org.


DOCUMENTATION

Please refer to the documentation folder (/doc) for
the reference manual.


##############################################
----------------------------------
OWASP ModSecurity Core Rule Set (CRS)


Project Site:
https://www.owasp.org/index.php/Category:OWASP_ModSecurity_Core_Rule_Set_Project


Download:
https://github.com/SpiderLabs/owasp-modsecurity-crs

----------------------------------

ModSecurity™ is a web application firewall engine that provides very
little protection on its own. In order to become useful, ModSecurity™ must
be configured with rules. In order to enable users to take full advantage
of ModSecurity™ out of the box, Trustwave's SpiderLabs is providing a free
certified rule set for ModSecurity™ 2.x. Unlike intrusion detection and
prevention systems, which rely on signatures specific to known
vulnerabilities, the Core Rules provide generic protection from unknown
vulnerabilities often found in web applications, which are in most cases
custom coded. The Core Rules are heavily commented to allow it to be used
as a step-by-step deployment guide for ModSecurity™.
Core Rules Content

In order to provide generic web applications protection, the Core Rules
use the following techniques:

* HTTP Protection - detecting violations of the HTTP protocol and a
locally defined usage policy.
* Real-time Blacklist Lookups - utilizes 3rd Party IP Reputation
* Web-based Malware Detection - identifies malicious web content by check
against the Google Safe Browsing API.
* HTTP Denial of Service Protections - defense against HTTP Flooding and
Slow HTTP DoS Attacks.
* Common Web Attacks Protection - detecting common web application
security attack.
* Automation Detection - Detecting bots, crawlers, scanners and other
surface malicious activity.
* Integration with AV Scanning for File Uploads - detects malicious files
uploaded through the web application.
* Tracking Sensitive Data - Tracks Credit Card usage and blocks leakages.
* Trojan Protection - Detecting access to Trojans horses.
* Identification of Application Defects - alerts on application
misconfigurations.
* Error Detection and Hiding - Disguising error messages sent by the
server.


----------------------------------
ModSecurity Rules from Trustwave SpiderLabs

Project Site:
https://www.trustwave.com/modsecurity-rules-support.php

Download:
https://ssl.trustwave.com/web-application-firewall

----------------------------------



Trustwave now provides a commercial certified rule set for ModSecurity 2.x
that protects against known attacks that target vulnerabilities in public
software and are based on intelligence gathered from real-world
investigations, honeypot data and research.

1. More than 16,000 specific rules, broken out into the following attack
categories:
 * SQL injection
 * Cross-site Scripting (XSS)
 * Local File Include
 * Remote File Include

2. User option for application specific rules, covering the same
vulnerability classes for applications such as:
 * WordPress
 * cPanel
 * osCommerce
 * Joomla
 * For a complete listing of application coverage, please refer to this
link (which is updated daily).
https://modsecurity.org/projects/commercial/rules/application_coverage.html

3. Complements and integrates with the OWASP Core Rule Set
4. IP Reputation capabilities which provide protection against malicious
clients identified by the Trustwave SpiderLabs Distributed Web Honeypots
5. Malware Detection capabilities which prevent your web site from
distributing malicious code to clients.
##############################################
