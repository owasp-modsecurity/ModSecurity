{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (forcebodybuf, <=NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_URI "/test.txt" "id:500219,phase:1,t:none,pass,ctl:forceRequestBodyVariable=On"
	),
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/plain",
			"Content-Length" => "16384",
		],
		"a" x 16384,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (forcebodybuf, >NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_URI "/test.txt" "id:500219,phase:1,t:none,pass,ctl:forceRequestBodyVariable=On"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^413$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/plain",
			"Content-Length" => "16385",
		],
		"a" x 16385,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (forcebodybuf, >Limit, <=NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_URI "/test.txt" "id:500219,phase:1,t:none,pass,ctl:forceRequestBodyVariable=On"
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^413$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		"a" x 16385,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (forcebodybuf, >Limit, >NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_URI "/test.txt" "id:500219,phase:1,t:none,pass,ctl:forceRequestBodyVariable=On"
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(32768\)\./, 1 ],
	},
	match_response => {
		status => qr/^413$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "32769",
		],
		"a" x 32769,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (forcebodybuf, >NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_URI "/test.txt" "id:500219,phase:1,t:none,pass,ctl:forceRequestBodyVariable=On"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/plain",
			"Content-Length" => "16385",
		],
		"a" x 16385,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (forcebodybuf, >Limit, <=NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_URI "/test.txt" "id:500219,phase:1,t:none,pass,ctl:forceRequestBodyVariable=On"
	),
	match_log => {
 		error => [ qr/ModSecurity: Request body \(Content-Length\) is larger than the configured limit \(16384\)\./, 1 ],
		debug => [ qr/enable_partial_processing for none reqbody_processor/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
 			"Content-Type" => "text/plain",
			"Content-Length" => "16385",
		],
		"a" x 16385,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (forcebodybuf, >Limit, >NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_URI "/test.txt" "id:500219,phase:1,t:none,pass,ctl:forceRequestBodyVariable=On"
	),
	match_log => {
		error => [ qr/ModSecurity: Request body \(Content-Length\) is larger than the configured limit \(32768\)\./, 1 ],
		debug => [ qr/enable_partial_processing for none reqbody_processor/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "32769",
		],
		"a" x 32769,
	),
},
