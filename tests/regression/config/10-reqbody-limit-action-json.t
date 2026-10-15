{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (JSON, <=NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
			"Content-Type" => "application/json",
			"Content-Length" => "16384",
		],
		'{"a":"' . '1' x 16376 . '"}',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (JSON, <=NoFilesLimit, deny bad value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
	),
	match_log => {
		-error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "16384",
		],
		'{"a":"' . '1' x 16360 . '","b":"bad_value"}',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (JSON, <=NoFilesLimit, deny bad name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
	),
	match_log => {
		-error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "16384",
		],
		'{"a":"' . '1' x 16363 . '","bad_name":1}',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (JSON, >NoFilesLimit, too long)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		'{"a":"' . '1' x 16377 . '"}',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (JSON, >Limit, <=NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
		'{"a":"' . '1' x 16377 . '"}',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (JSON, >Limit, >NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
		'{"a":"' . '1' x 32761 . '"}',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, <=NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
			"Content-Type" => "application/json",
			"Content-Length" => "16384",
		],
		'{"a":"' . '1' x 16376 . '"}',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, <=NoFilesLimit, deny bad value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
	),
	match_log => {
		-error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "16384",
		],
		'{"a":"' . '1' x 16360 . '","b":"bad_value"}',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, <=NoFilesLimit, deny bad name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
	),
	match_log => {
		-error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "16384",
		],
		'{"a":"' . '1' x 16363 . '","bad_name":1}',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, >NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		'{"a":"' . '1' x 16377 . '"}',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, >NoFilesLimit, deny bad value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		'{"a":"' . '1' x 16361 . '","b":"bad_value",',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, >NoFilesLimit, pass bad value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		'{"a":"' . '1' x 16361 . '","b":"bad_value "',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, >NoFilesLimit, deny bad name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		'{"a":"' . '1' x 16364 . '","bad_name":1 ',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, >NoFilesLimit, pass bad name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		'{"a":"' . '1' x 16364 . '","bad_name": 1',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, >Limit, <=NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		'{"a":"' . '1' x 16377 . '"}',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, >Limit, <=NoFilesLimit, deny bad value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		'{"a":"' . '1' x 16361 . '","b":"bad_value" ',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, >Limit, <=NoFilesLimit, pass bad value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		'{"a":"' . '1' x 16361 . '","b":" bad_value"',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, >Limit, <=NoFilesLimit, deny bad name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		'{"a":"' . '1' x 16364 . '","bad_name":1 ',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, >Limit, <=NoFilesLimit, pass bad name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
			"Content-Type" => "application/json",
			"Content-Length" => "16385",
		],
		'{"a":"' . '1' x 16364 . '","bad_name": 1',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (JSON, >Limit, >NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
		SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
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
			"Content-Type" => "application/json",
			"Content-Length" => "32769",
		],
		'{"a":"' . '1' x 32761 . '"}',
	),
},
