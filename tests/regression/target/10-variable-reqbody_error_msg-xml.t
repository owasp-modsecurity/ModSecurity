### REQBODY_ERROR_MSG for XML parse failures (#3680)
#
# libxml2's own reason (with line and column) must reach the error message.
# The parser contexts also carry XML_PARSE_NOERROR, so libxml2's own
# "body.xml:N: parser error" lines must never show up in the error log
# (Apache's stderr); the -error checks below guard that.

# Tag mismatch
{
	type => "target",
	comment => "REQBODY_ERROR_MSG carries the libxml2 reason (tag mismatch)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^text/xml\$" "id:500005, \\
		        phase:1,t:none,t:lowercase,nolog,pass,ctl:requestBodyProcessor=XML"
	),
	match_log => {
		error => [ qr/XML parser error: XML: Failed to parse document: Opening and ending tag mismatch: book line \d+ and bookasdf \(line \d+, column \d+\)/, 1 ],
		-error => [ qr/body\.xml:\d+: parser error/, 1 ],
		debug => [ qr/XML: Parsing complete \(well_formed 0\)/, 1 ],
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
				<?xml version="1.0" encoding="UTF-8"?>
				<bookstore>
				<book category="WEB">
				<title lang="en">Learning XML</title>
				<author>Erik T. Ray</author>
				</bookasdf>
				</bookstore>
			),
		),
	),
},

# Extra content after the root element (the #3563 reproducer)
{
	type => "target",
	comment => "REQBODY_ERROR_MSG carries the libxml2 reason (extra content)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^text/xml\$" "id:500005, \\
		        phase:1,t:none,t:lowercase,nolog,pass,ctl:requestBodyProcessor=XML"
	),
	match_log => {
		error => [ qr/XML parser error: XML: Failed to parse document: Extra content at the end of the document \(line \d+, column \d+\)/, 1 ],
		-error => [ qr/body\.xml:\d+: parser error/, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
		],
		"<xml></xml> Testing modsec",
	),
},
