### SecAudit* directive tests
{
	type => "config",
	comment => "SecAuditEngine On",
	conf => qq(
		SecAuditEngine On
		SecAuditLog $ENV{AUDIT_LOG}
	),
	match_log => {
		audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "config",
	comment => "SecAuditEngine Off",
	conf => qq(
		SecAuditEngine Off
		SecAuditLog $ENV{AUDIT_LOG}
	),
	match_log => {
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "config",
	comment => "SecAuditEngine RelevantOnly (pos)",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog $ENV{AUDIT_LOG}
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecResponseBodyAccess On
		SecDefaultAction "phase:2,log,auditlog,pass"
		SecRule REQUEST_URI "." "phase:4,deny"
	),
	match_log => {
		audit => [ qr/./, 1 ],
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
	comment => "SecAuditEngine RelevantOnly (neg)",
	conf => qq(
		SecAuditEngine RelevantOnly
		SecAuditLog $ENV{AUDIT_LOG}
		SecResponseBodyAccess On
		SecDefaultAction "phase:2,log,auditlog,pass"
	),
	match_log => {
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "config",
	comment => "SecAuditLogRelevantStatus (pos)",
	conf => qq(
		SecAuditEngine RelevantOnly
		SecAuditLog $ENV{AUDIT_LOG}
		SecAuditLogRelevantStatus "^4"
	),
	match_log => {
		audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^404$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/bogus",
	),
},
{
	type => "config",
	comment => "SecAuditLogRelevantStatus (neg)",
	conf => qq(
		SecAuditEngine RelevantOnly
		SecAuditLog $ENV{AUDIT_LOG}
		SecAuditLogRelevantStatus "^4"
	),
	match_log => {
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "config",
	comment => "SecAuditLogParts (minimal)",
	conf => qq(
		SecAuditEngine On
		SecAuditLog $ENV{AUDIT_LOG}
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecAuditLogParts "AZ"
	),
	match_log => {
		audit => [ qr/-A--.*-Z--/s, 1 ],
		-audit => [ qr/-[B-Y]--/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1r&=2",
	),
},
{
	type => "config",
	comment => "SecAuditLogParts (default)",
	conf => qq(
		SecAuditEngine On
		SecAuditLog $ENV{AUDIT_LOG}
		SecRequestBodyAccess On
		SecResponseBodyAccess On
	),
	match_log => {
		audit => [ qr/-A--.*-B--.*-F--.*-H--.*-Z--/s, 1 ],
		-audit => [ qr/-[DEGIJK]--/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1r&=2",
	),
},
{
	type => "config",
	comment => "SecAuditLogParts (all)",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine On
		SecAuditLog $ENV{AUDIT_LOG}
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecAuditLogParts "ABCDEFGHIJKZ"
		SecAction "phase:4,log,auditlog,allow"
	),
	match_log => {
		audit => [ qr/-A--.*-B--.*-C--.*-F--.*-E--.*-H--.*-K--.*-Z--/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1r&=2",
	),
},
