### Logging tests

# log/nolog
{
	type => "action",
	comment => "log",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,log"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "nolog",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,nolog"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# auditlog/noauditlog
{
	type => "action",
	comment => "auditlog",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,auditlog"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/Message: Warning. Unconditional match in SecAction\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "noauditlog",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,noauditlog"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction\./, 1 ],
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# All log/nolog auditlog/noauditlog combos
{
	type => "action",
	comment => "log,auditlog",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,log,auditlog"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/Message: Warning. Unconditional match in SecAction\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "log,noauditlog",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,log,noauditlog"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction\./, 1 ],
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
	type => "action",
	comment => "nolog,auditlog",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,nolog,auditlog"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		# No message, but should have data.  This may need changed
		audit => [ qr/-H--\s+Stopwatch: /s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "nolog,noauditlog",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,nolog,noauditlog"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
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
	type => "action",
	comment => "auditlog,log",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,auditlog,log"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/Message: Warning. Unconditional match in SecAction\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "auditlog,nolog",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,auditlog,nolog"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
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
	type => "action",
	comment => "noauditlog,log",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,noauditlog,log"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction\./, 1 ],
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
	type => "action",
	comment => "noauditlog,nolog",
	conf => qq(
		SecRuleEngine On
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,noauditlog,nolog"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

