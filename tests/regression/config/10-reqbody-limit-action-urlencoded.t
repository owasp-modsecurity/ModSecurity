{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (urlencoded, <=NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16384",
		],
		'a=1&b=' . '2' x 16378,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (urlencoded, <=NoFilesLimit, deny bad_value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16384",
		],
		'a=' . '1' x 16370 . '&b=bad_value',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (urlencoded, <=NoFilesLimit, deny bad_name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16384",
		],
		'a=' . '1' x 16373 . '&bad_name',
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=1&b=' . '2' x 16379,
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=1&b=' . '2' x 16379,
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "32769",
		],
		'a=1&b=' . '2' x 32763,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, <=NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16384",
		],
		'a=1&b=' . '2' x 16378,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, <=NoFilesLimit, deny bad_value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16384",
		],
		'a=' . '1' x 16370 . '&b=bad_value',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, <=NoFilesLimit, deny bad_name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16384",
		],
		'a=' . '1' x 16373 . '&bad_name',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=' . '1' x 16379 . '&b=2',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >NoFilesLimit, deny bad value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=' . '1' x 16369 . '&b=bad_value&c',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >NoFilesLimit, pass bad value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=' . '1' x 16370 . '&b=bad_value&',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >NoFilesLimit, deny bad name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=' . '1' x 16370 . '&bad_name=2&c',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >NoFilesLimit, deny bad name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=' . '1' x 16371 . '&bad_name=2&',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >Limit, <=NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=1&b=' . '2' x 16379,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >Limit, <=NoFilesLimit, deny bad value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=' . '1' x 16369 . '&b=bad_value&c',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >Limit, <=NoFilesLimit, pass bad value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=' . '1' x 16370 . '&b=bad_value&',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >Limit, <=NoFilesLimit, deny bad name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=' . '1' x 16370 . '&bad_name=2&c',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >Limit, <=NoFilesLimit, pass bad name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "16385",
		],
		'a=' . '1' x 16371 . '&bad_name=2&',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (urlencoded, >Limit, >NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
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
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "32769",
		],
		'a=1&b=' . '2' x 32763,
	),
},
