### Tests for directives altering how a response is handled

# SecResponseBodyMimeTypesClear
{
	type => "config",
	comment => "SecResponseBodyMimeTypesClear",
	conf => qq(
		SecRuleEngine On
		SecResponseBodyAccess On
		SecResponseBodyMimeTypesClear
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule RESPONSE_BODY "TEST" "phase:4,deny,id:500237"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
		debug => [ qr/Not buffering response body for unconfigured MIME type/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# SecResponseBodyAccess & SecResponseBodyMimeType
{
	type => "config",
	comment => "SecResponseBodyAccess On",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecResponseBodyAccess On
		SecResponseBodyMimeType text/plain null
		SecRule RESPONSE_BODY "TEST" "phase:4,deny,id:500238"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 4\)\. Pattern match "TEST" at RESPONSE_BODY\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "config",
	comment => "SecResponseBodyAccess Off",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecResponseBodyAccess Off
		SecResponseBodyMimeType text/plain null
		SecRule RESPONSE_BODY "TEST" "phase:4,deny,id:500239"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
		debug => [ qr/Response body buffering is not enabled\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# SecResponseBodyLimit
{
	type => "config",
	comment => "SecResponseBodyLimit (equal)",
	conf => qq(
		SecRuleEngine On
		SecResponseBodyAccess On
		SecResponseBodyMimeType text/plain null
		SecResponseBodyLimit 8192
	),
	match_log => {
		-error => [ qr/Content-Length \(\d+\) over the limit/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/8k.txt",
	),
},
{
	type => "config",
	comment => "SecResponseBodyLimit (less)",
	conf => qq(
		SecRuleEngine On
		SecResponseBodyAccess On
		SecResponseBodyMimeType text/plain null
		SecResponseBodyLimit 9000
	),
	match_log => {
		-error => [ qr/Content-Length \(\d+\) over the limit/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/8k.txt",
	),
},
{
	type => "config",
	comment => "SecResponseBodyLimit (greater)",
	conf => qq(
		SecRuleEngine On
		SecResponseBodyAccess On
		SecResponseBodyMimeType text/plain null
		SecResponseBodyLimit 8000
	),
	match_log => {
		error => [ qr/Content-Length \(\d+\) over the limit \(8000\)\./, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/8k.txt",
	),
},

# ResponseBodyLimitAction
{
	type => "config",
	comment => "SecResponseBodyLimitAction Reject",
	conf => qq(
		SecRuleEngine On
		SecResponseBodyAccess On
		SecResponseBodyMimeType text/plain null
		SecResponseBodyLimit 5
		SecResponseBodyLimitAction Reject
	),
	match_log => {
		error => [ qr/Content-Length \(\d+\) over the limit \(5\)\./, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/8k.txt",
	),
},
{
	type => "config",
	comment => "SecResponseBodyLimitAction ProcessPartial",
	conf => qq(
		SecRuleEngine On
		SecResponseBodyAccess On
		SecResponseBodyMimeType text/plain null
		SecResponseBodyLimit 5
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 4
		SecResponseBodyLimitAction ProcessPartial
	),
	match_log => {
		-error => [ qr/Content-Length \(\d+\) over the limit/, 1 ],
		debug => [ qr/Processing partial response body \(limit 5\)/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/8k.txt",
	),
},
