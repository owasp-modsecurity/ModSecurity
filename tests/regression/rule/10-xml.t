### Test for XML operator rules

### Validate Scheme
# OK
{
	type => "rule",
	comment => "validateSchema (validate ok)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
        SecXmlExternalEntity On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^text/xml\$" "id:500005, \\
		        phase:1,t:none,t:lowercase,nolog,pass,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_PROCESSOR "!^XML\$" nolog,pass,skipAfter:12345,id:500006
		SecRule XML "\@validateSchema $ENV{CONF_DIR}/SoapEnvelope.xsd" "id:500007, \\
		        phase:3:,deny"
	),
	match_log => {
		debug => [ qr/XML: Initialising parser.*XML: Parsing complete \(well_formed 1\).*Target value: "\[XML document tree\]".*Successfully validated payload against Schema/s, 1 ],
		-debug => [ qr/XML parser error|validation failed|Failed to load/, 1 ],
		-error => [ qr/XML parser error|validation failed|Failed to load/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
		],
		normalize_raw_request_data(
			q(
				<?xml version="1.0" encoding="utf-8"?>
				<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"
				xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/"
				xmlns:tns="http://www.bluebank.example.com/axis/getBalance.jws"
				xmlns:types="http://www.bluebank.example.com/axis/getBalance.jws/encodedTypes"
				xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
				xmlns:xsd="http://www.w3.org/2001/XMLSchema">
						<soap:Body soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
								<q1:getInput xmlns:q1="http://DefaultNamespace">
										<id xsi:type="xsd:string">12123</id>
								</q1:getInput>
						</soap:Body>
				</soap:Envelope>
			),
		),
	),
},
# Failed attribute value
{
	type => "rule",
	comment => "validateSchema (validate attribute value failed)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
        SecXmlExternalEntity On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecRule REQUEST_HEADERS:Content-Type "^text/xml\$" "id:500008, \\
		        phase:1,t:none,t:lowercase,nolog,pass,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_PROCESSOR "!^XML\$" nolog,pass,skipAfter:12345,id:500009
		SecRule XML "\@validateSchema $ENV{CONF_DIR}/SoapEnvelope.xsd" "id:500010 \\
		        phase:2,deny,log,auditlog,id:12345"
	),
	match_log => {
		debug => [ qr/XML: Initialising parser.*XML: Parsing complete \(well_formed 1\).*Target value: "\[XML document tree\]".*'badval' is not a valid value of the local atomic type.*Schema validation failed/s, 1 ],
		-debug => [ qr/Successfully validated payload against Schema|\n\r?\n/, 1 ],
		audit => [ qr/^Message: Element.*'badval' is not a valid value of the local atomic type\.\nMessage:/m, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
		],
		normalize_raw_request_data(
			q(
				<?xml version="1.0" encoding="utf-8"?>
				<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"
				xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/"
				xmlns:tns="http://www.bluebank.example.com/axis/getBalance.jws"
				xmlns:types="http://www.bluebank.example.com/axis/getBalance.jws/encodedTypes"
				xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
				xmlns:xsd="http://www.w3.org/2001/XMLSchema">
						<soap:Body soap:mustUnderstand="badval" soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
								<q1:getInput xmlns:q1="http://DefaultNamespace">
										<id xsi:type="xsd:string">12123</id>
								</q1:getInput>
						</soap:Body>
				</soap:Envelope>
			),
		),
	),
},
# Failed validation
{
	type => "rule",
	comment => "validateSchema (validate failed)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
        SecXmlExternalEntity On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecRule REQUEST_HEADERS:Content-Type "^text/xml\$" "id:500011, \\
		        phase:1,t:none,t:lowercase,nolog,pass,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_PROCESSOR "!^XML\$" nolog,pass,skipAfter:12345,id:500012
		SecRule XML "\@validateSchema $ENV{CONF_DIR}/SoapEnvelope.xsd" "id:500013 \\
		        phase:2,deny,id:12345"
	),
	match_log => {
		debug => [ qr/XML: Initialising parser.*XML: Parsing complete \(well_formed 1\).*Target value: "\[XML document tree\]".*element is not expected/s, 1 ],
		-debug => [ qr/XML parser error|Failed to load/, 1 ],
		-error => [ qr/XML parser error|Failed to load/, 1 ],
		audit => [ qr/^Message: Element.*This element is not expected.*\nMessage:/m, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
		],
		normalize_raw_request_data(
			q(
				<?xml version="1.0" encoding="utf-8"?>
				<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"
				xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/"
				xmlns:tns="http://www.bluebank.example.com/axis/getBalance.jws"
				xmlns:types="http://www.bluebank.example.com/axis/getBalance.jws/encodedTypes"
				xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
				xmlns:xsd="http://www.w3.org/2001/XMLSchema">
						<soap:xBody soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
								<q1:getInput xmlns:q1="http://DefaultNamespace">
										<id xsi:type="xsd:string">12123</id>
								</q1:getInput>
						</soap:xBody>
				</soap:Envelope>
			),
		),
	),
},
# Bad XML
{
	type => "rule",
	comment => "validateSchema (bad XML)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
        SecXmlExternalEntity On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecRule REQUEST_HEADERS:Content-Type "^text/xml\$" "id:500014, \\
		        phase:1,t:none,t:lowercase,nolog,pass,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_PROCESSOR "!^XML\$" nolog,pass,skipAfter:12345,id:500015
		SecRule XML "\@validateSchema $ENV{CONF_DIR}/SoapEnvelope.xsd" "id:500016, \\
		        phase:2,deny,id:12345"
	),
	match_log => {
		debug => [ qr/XML: Initialising parser.*XML: Parsing complete \(well_formed 0\).*XML parser error: XML: Failed to parse document./s, 1 ],
		debug => [ qr/XML parser error: XML: Failed to parse document./, 1 ],
		error => [ qr/XML parser error: XML: Failed to parse document./, 1 ],
		audit => [ qr/XML parser error: XML: Failed to parse document./m, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
		],
		normalize_raw_request_data(
			q(
				<?xml version="1.0" encoding="utf-8"?>
				<soap:Envelop xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"
				xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/"
				xmlns:tns="http://www.bluebank.example.com/axis/getBalance.jws"
				xmlns:types="http://www.bluebank.example.com/axis/getBalance.jws/encodedTypes"
				xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
				xmlns:xsd="http://www.w3.org/2001/XMLSchema">
						<soap:Body soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
								<q1:getInput xmlns:q1="http://DefaultNamespace">
										<id xsi:type="xsd:string">12123</id>
								</q1:getInput>
						</soap:Body>
				</soap:Envelope>
			),
		),
	),
},
# Bad schema
{
	type => "rule",
	comment => "validateSchema (bad schema)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
        SecXmlExternalEntity On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecRule REQUEST_HEADERS:Content-Type "^text/xml\$" "id:500017 \\
		        phase:1,t:none,t:lowercase,nolog,pass,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_PROCESSOR "!^XML\$" nolog,pass,skipAfter:12345,id:500018
		SecRule XML "\@validateSchema $ENV{CONF_DIR}/SoapEnvelope-bad.xsd" "id:500019 \\
		        phase:2,deny,id:12345"
	),
	match_log => {
		debug => [ qr/XML: Initialising parser.*XML: Parsing complete \(well_formed 1\).*Target value: "\[XML document tree\]".*Failed to parse the XML resource.*Failed to load Schema/s, 1 ],
		audit => [ qr/^Message: .*Failed to parse the XML resource.*\nMessage: Rule processing failed/m, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
		],
		normalize_raw_request_data(
			q(
				<?xml version="1.0" encoding="utf-8"?>
				<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/"
				xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/"
				xmlns:tns="http://www.bluebank.example.com/axis/getBalance.jws"
				xmlns:types="http://www.bluebank.example.com/axis/getBalance.jws/encodedTypes"
				xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
				xmlns:xsd="http://www.w3.org/2001/XMLSchema">
						<soap:Body soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
								<q1:getInput xmlns:q1="http://DefaultNamespace">
										<id xsi:type="xsd:string">12123</id>
								</q1:getInput>
						</soap:Body>
				</soap:Envelope>
			),
		),
	),
},

# Validate DTD
# OK
{
	type => "rule",
	comment => "validateDTD (validate ok)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
        SecXmlExternalEntity On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^text/xml\$" "id:500020, \\
		        phase:1,t:none,t:lowercase,nolog,pass,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_PROCESSOR "!^XML\$" nolog,pass,skipAfter:12345,id:500021
		SecRule XML "\@validateDTD $ENV{CONF_DIR}/SoapEnvelope.dtd" "id:500022, \\
		        phase:2,deny,id:12345"
	),
	match_log => {
		debug => [ qr/XML: Initialising parser.*XML: Parsing complete \(well_formed 1\).*Target value: "\[XML document tree\]".*Successfully validated payload against DTD/s, 1 ],
		-debug => [ qr/XML parser error|validation failed|Failed to load/, 1 ],
		-error => [ qr/XML parser error|validation failed|Failed to load/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
		],
		normalize_raw_request_data(
			q(
				<?xml version="1.0" encoding="utf-8"?>
				<!DOCTYPE Envelope SYSTEM "SoapEnvelope.dtd">
				<Envelope>
						<Body>
								<getInput>
										<id type="string">12123</id>
								</getInput>
						</Body>
				</Envelope>
			),
		),
	),
},
# Failed validation
{
	type => "rule",
	comment => "validateDTD (validate failed)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
        SecXmlExternalEntity On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^text/xml\$" "id:500023, \\
		        phase:1,t:none,t:lowercase,nolog,pass,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_PROCESSOR "!^XML\$" nolog,pass,skipAfter:12345,id:500024
		SecRule XML "\@validateDTD $ENV{CONF_DIR}/SoapEnvelope.dtd" "id:500025, \\
		        phase:2,deny,id:12345"
	),
	match_log => {
		debug => [ qr/XML: Initialising parser.*XML: Parsing complete \(well_formed 1\).*Target value: "\[XML document tree\]".*content does not follow the DTD/s, 1 ],
		-debug => [ qr/XML parser error|Failed to load/, 1 ],
		-error => [ qr/XML parser error|Failed to load/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
		],
		normalize_raw_request_data(
			q(
				<?xml version="1.0" encoding="utf-8"?>
				<!DOCTYPE Envelope SYSTEM "SoapEnvelope.dtd">
				<Envelope>
						<xBody>
								<getInput>
										<id type="string">12123</id>
								</getInput>
						</xBody>
				</Envelope>
			),
		),
	),
},
# Bad XML
{
	type => "rule",
	comment => "validateDTD (bad XML)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
        SecXmlExternalEntity On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecRule REQUEST_HEADERS:Content-Type "^text/xml\$" "id:500026, \\
		        phase:1,t:none,t:lowercase,nolog,pass,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_PROCESSOR "!^XML\$" nolog,pass,skipAfter:12345,id:500027
		SecRule XML "\@validateDTD $ENV{CONF_DIR}/SoapEnvelope.dtd" "id:500028, \\
		        phase:2,deny,id:12345"
	),
	match_log => {
		debug => [ qr/XML: Initialising parser.*XML: Parsing complete \(well_formed 0\).*XML parser error: XML: Failed to parse document./s, 1 ],
		debug => [ qr/XML parser error: XML: Failed to parse document./, 1 ],
		audit => [ qr/^Message: .*Failed to parse document.*\nMessage:/m, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
		],
		normalize_raw_request_data(
			q(
				<?xml version="1.0" encoding="utf-8"?>
				<!DOCTYPE Envelope SYSTEM "SoapEnvelope.dtd">
				<Envelop>
						<Body>
								<getInput>
										<id type="string">12123</id>
								</getInput>
						</Body>
				</Envelope>
			),
		),
	),
},
# Bad DTD
{
	type => "rule",
	comment => "validateDTD (bad DTD)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
        SecXmlExternalEntity On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^(?:application(?:/soap\+|/)|text/)xml" "id:500029, \\
		        phase:1,t:none,t:lowercase,nolog,pass,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_PROCESSOR "!^XML\$" nolog,pass,skipAfter:12345,id:500030
		SecRule XML "\@validateDTD $ENV{CONF_DIR}/SoapEnvelope-bad.dtd" "id:500031 \\
		        phase:2,deny,id:12345"
	),
	match_log => {
		debug => [ qr/XML: Initialising parser.*XML: Parsing complete \(well_formed 1\).*Target value: "\[XML document tree\]".*Failed to load DTD/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
		],
		normalize_raw_request_data(
			q(
				<?xml version="1.0" encoding="utf-8"?>
				<!DOCTYPE Envelope SYSTEM "SoapEnvelope.dtd">
				<Envelope>
						<Body>
								<getInput>
										<id type="string">12123</id>
								</getInput>
						</Body>
				</Envelope>
			),
		),
	),
},

### ctl:ruleRemoveTargetByTag with an XPath target (XML://@*)
# Baseline: without any target removal, the XML://@* target lets the rule
# match "attack" found in an XML attribute value.
{
	type => "rule",
	comment => "ruleRemoveTargetByTag baseline: XML://\@* matches attribute value",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^(?:application(?:/soap\+|/)|text/)xml" "id:500040, \\
		        phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/*|XML://\@* "\@rx attack" "id:500041, \\
		        phase:2,deny,status:403,log,tag:'xml-attr-remove-test',msg:'XML attribute matched'"
	),
	match_log => {
		error => [ qr/Pattern match "attack" at XML\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
		],
		normalize_raw_request_data(
			q(
				<?xml version="1.0"?><root><a probe="attack"></a></root>
			),
		),
	),
},
# Regression for issue #3591: ctl:ruleRemoveTargetByTag must strip the
# XML://@* target from the tagged rule, so it stops inspecting attribute
# values. Previously this had no effect because target-exception matching
# only ever compared the variable's embedded ":param" suffix in its name,
# which XML targets never populate (they keep the XPath expression in a
# separate field instead).
{
	type => "rule",
	comment => "ruleRemoveTargetByTag removes an XML://\@* target from a tagged rule",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^(?:application(?:/soap\+|/)|text/)xml" "id:500042, \\
		        phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/*|XML://\@* "\@rx attack" "id:500043, \\
		        phase:2,deny,status:403,log,tag:'xml-attr-remove-test-2',msg:'XML attribute matched'"
		SecAction "id:500044,phase:1,pass,nolog,ctl:ruleRemoveTargetByTag=xml-attr-remove-test-2;XML://\@*"
	),
	match_log => {
		debug => [ qr/fetch_target_exception: Target XML:\/\/\@\* will not be processed\./, 1 ],
		-error => [ qr/Pattern match "attack" at XML\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
		],
		normalize_raw_request_data(
			q(
				<?xml version="1.0"?><root><a probe="attack"></a></root>
			),
		),
	),
},
