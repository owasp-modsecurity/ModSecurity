### Logging tests

# log/nolog (pass)
{
	type => "action",
	comment => "log (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,log,id:500006"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning\. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/Message: Warning\. Unconditional match in SecAction\./, 1 ],
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
	comment => "nolog (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,nolog,id:500007"
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

# log/nolog (deny)
{
	type => "action",
	comment => "log (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,log,id:500008"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 403 \(phase 1\)\. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/Message: Access denied with code 403 \(phase 1\)\. Unconditional match in SecAction\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "nolog (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,nolog,id:500009"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# auditlog/noauditlog (pass)
{
	type => "action",
	comment => "auditlog (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,auditlog,id:500010"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning\. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/Message: Warning\. Unconditional match in SecAction\./, 1 ],
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
	comment => "noauditlog (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,noauditlog,id:500011"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning\. Unconditional match in SecAction\./, 1 ],
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# auditlog/noauditlog (deny)
{
	type => "action",
	comment => "auditlog (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,auditlog,id:500012"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 403 \(phase 1\)\. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/Message: Access denied with code 403 \(phase 1\)\. Unconditional match in SecAction\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "noauditlog (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,noauditlog,id:500013"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 403 \(phase 1\)\. Unconditional match in SecAction\./, 1 ],
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# All log/nolog auditlog/noauditlog combos (pass)
{
	type => "action",
	comment => "log,auditlog (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,log,auditlog,id:500014"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning\. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/Message: Warning\. Unconditional match in SecAction\./, 1 ],
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
	comment => "log,noauditlog (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,log,noauditlog,id:500015"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning\. Unconditional match in SecAction\./, 1 ],
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
	comment => "nolog,auditlog (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,nolog,auditlog,id:500016"
	),
	match_log => {
		audit => [ qr/-H--\s+Message: .*Stopwatch: /s, 1 ],
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
	comment => "nolog,noauditlog (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,nolog,noauditlog,id:500017"
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
	comment => "auditlog,log (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,auditlog,log,id:500018"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning\. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/Message: Warning\. Unconditional match in SecAction\./, 1 ],
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
	comment => "auditlog,nolog (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,auditlog,nolog,id:500019"
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
	comment => "noauditlog,log (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,noauditlog,log,id:500020"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning\. Unconditional match in SecAction\./, 1 ],
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
	comment => "noauditlog,nolog (pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,pass,noauditlog,nolog,id:500021"
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

# All log/nolog auditlog/noauditlog combos (deny)
{
	type => "action",
	comment => "log,auditlog (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,log,auditlog,id:500022"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 403 \(phase 1\)\. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/Message: Access denied with code 403 \(phase 1\)\. Unconditional match in SecAction\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "log,noauditlog (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,log,noauditlog,id:500023"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 403 \(phase 1\)\. Unconditional match in SecAction\./, 1 ],
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "nolog,auditlog (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,nolog,auditlog,id:500024"
	),
	match_log => {
		audit => [ qr/-H--\s+Message: .*Stopwatch: /s, 1 ],
	},
	match_response => {
		-error => [ qr/ModSecurity: /, 1 ],
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "nolog,noauditlog (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,nolog,noauditlog,id:500025"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "auditlog,log (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,auditlog,log,id:500026"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 403 \(phase 1\)\. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/Message: Access denied with code 403 \(phase 1\)\. Unconditional match in SecAction\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "auditlog,nolog (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,auditlog,nolog,id:500027"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "noauditlog,log (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,noauditlog,log,id:500028"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 403 \(phase 1\)\. Unconditional match in SecAction\./, 1 ],
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "noauditlog,nolog (deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
        SecAuditLogRelevantStatus xxx
		SecAuditEngine RelevantOnly
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAction "phase:1,deny,status:403,noauditlog,nolog,id:500029"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
