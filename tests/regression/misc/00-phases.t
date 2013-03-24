### Test the phases

# Phase 1 (request headers)
{
	type => "misc",
	comment => "phase 1",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType text/plain null
		SecRule REQUEST_LINE "^POST" "phase:1,pass,log,auditlog,id:500169"
		SecRule ARGS "val1" "phase:1,pass,log,auditlog,id:500170"
		SecRule RESPONSE_HEADERS:Last-Modified "." "phase:1,pass,log,auditlog,id:500171"
		SecRule RESPONSE_BODY "TEST" "phase:1,pass,log,auditlog,id:500172"
	),
	match_log => {
		error => [ qr/Pattern match "\^POST" at REQUEST_LINE/, 1 ],
		-error => [ qr/Pattern match .* (ARGS|RESPONSE)/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

# Phase 2 (request body)
{
	type => "misc",
	comment => "phase 2",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType text/plain null
		SecRule REQUEST_LINE "^POST" "phase:2,pass,log,auditlog,id:500173"
		SecRule ARGS "val1" "phase:2,pass,log,auditlog,id:500174"
		SecRule RESPONSE_HEADERS:Last-Modified "." "phase:2,pass,log,auditlog,id:500175"
		SecRule RESPONSE_BODY "TEST" "phase:2,pass,log,auditlog,id:500176"
	),
	match_log => {
		error => [ qr/Pattern match "\^POST" at REQUEST_LINE.*Pattern match "val1" at ARGS/s, 1 ],
		-error => [ qr/Pattern match .* RESPONSE/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

# Phase 3 (response headers)
{
	type => "misc",
	comment => "phase 3",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType text/plain null
		SecRule REQUEST_LINE "^POST" "phase:3,pass,log,auditlog,id:500177"
		SecRule ARGS "val1" "phase:3,pass,log,auditlog,id:500178"
		SecRule RESPONSE_HEADERS:Last-Modified "." "phase:3,pass,log,auditlog,id:500179"
		SecRule RESPONSE_BODY "TEST" "phase:3,pass,log,auditlog,id:500180"
	),
	match_log => {
		error => [ qr/Pattern match "\^POST" at REQUEST_LINE.*Pattern match "val1" at ARGS.*Pattern match "\." at RESPONSE_HEADERS/s, 1 ],
		-error => [ qr/Pattern match .* RESPONSE_BODY/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

# Phase 4 (response body)
{
	type => "misc",
	comment => "phase 4",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType text/plain null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
		SecRule REQUEST_LINE "^POST" "phase:4,pass,log,auditlog,id:500181"
		SecRule ARGS "val1" "phase:4,pass,log,auditlog,id:500182"
		SecRule RESPONSE_HEADERS:Last-Modified "." "phase:4,pass,log,auditlog,id:500183"
		SecRule RESPONSE_BODY "TEST" "phase:4,pass,log,auditlog,id:500184"
	),
	match_log => {
		error => [ qr/Pattern match "\^POST" at REQUEST_LINE.*Pattern match "val1" at ARGS.*Pattern match "\." at RESPONSE_HEADERS.*Pattern match "TEST" at RESPONSE_BODY/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

# Phase 5 (logging)
{
	type => "misc",
	comment => "phase 5",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType text/plain null
		SecRule REQUEST_LINE "^POST" "phase:5,pass,log,auditlog,id:500185"
		SecRule ARGS "val1" "phase:5,pass,log,auditlog,id:500186"
		SecRule RESPONSE_HEADERS:Last-Modified "." "phase:5,pass,log,auditlog,id:500187"
		SecRule RESPONSE_BODY "TEST" "phase:5,pass,log,auditlog,id:500188"
	),
	match_log => {
		error => [ qr/Pattern match "\^POST" at REQUEST_LINE.*Pattern match "val1" at ARGS.*Pattern match "\." at RESPONSE_HEADERS.*Pattern match "TEST" at RESPONSE_BODY/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},
