{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (urlencoded, <=NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
	),
	match_log => {
		-error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16384",
		],
		"a=1&b=" . "2" x 16378,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (urlencoded, >NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		"a=1&b=" . "2" x 16379,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (urlencoded, >Limit, <=NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		"a=1&b=" . "2" x 16379,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (urlencoded, >Limit, >NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "32769",
		],
		"a=1&b=" . "2" x 32763,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		"a=1&b=" . "2" x 16379,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >Limit, <=NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		"a=1&b=" . "2" x 16379,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >Limit, >NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(32768\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "32769",
		],
		"a=1&b=" . "2" x 32763,
	),
},