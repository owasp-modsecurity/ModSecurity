
### SecArgumentSeparator
{
	type => "config",
	comment => "SecArgumentSeparator (get-pos)",
	conf => q(
		SecRuleEngine On
		SecArgumentSeparator ";"
		SecRule ARGS:a "@streq 1" "phase:1,deny,chain"
		SecRule ARGS:b "@streq 2"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\). String match "2" at ARGS:b./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?a=1;b=2",
	),
},
{
	type => "config",
	comment => "SecArgumentSeparator (get-neg)",
	conf => q(
		SecRuleEngine On
		SecRule ARGS:a "@streq 1" "phase:1,deny,chain"
		SecRule ARGS:b "@streq 2"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?a=1;b=2",
	),
},
{
	type => "config",
	comment => "SecArgumentSeparator (post-pos)",
	conf => q(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecArgumentSeparator ";"
		SecRule ARGS:a "@streq 1" "phase:2,deny,chain"
		SecRule ARGS:b "@streq 2"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 2\). String match "2" at ARGS:b./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1;b=2",
	),
},
{
	type => "config",
	comment => "SecArgumentSeparator (post-neg)",
	conf => q(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRule ARGS:a "@streq 1" "phase:2,deny"
		SecRule ARGS:b "@streq 2" "phase:2,deny"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1;b=2",
	),
},

### SecRequestBodyAccess
{
	type => "config",
	comment => "SecRequestBodyAccess (pos)",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine On
		SecAuditLogParts ABCDEFGHIJKZ
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecAuditLog $ENV{AUDIT_LOG}
		SecRequestBodyAccess On
		SecRule ARGS:a "\@streq 1" "phase:2,deny,chain"
		SecRule ARGS:b "\@streq 2"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 2\). String match "2" at ARGS:b./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1&b=2",
	),
},
{
	type => "config",
	comment => "SecRequestBodyAccess (neg)",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine On
		SecAuditLogParts ABCDEFGHIJKZ
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecAuditLog $ENV{AUDIT_LOG}
		SecRequestBodyAccess Off
		SecRule ARGS:a "\@streq 1" "phase:2,deny"
		SecRule ARGS:b "\@streq 2" "phase:2,deny"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1&b=2",
	),
},
