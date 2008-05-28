### Test for SecRuleScript

# Lua
{
	type => "rule",
	comment => "SecRuleScript (lua absolute nomatch)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 1
		SecRuleScript "$ENV{CONF_DIR}/test.lua" "phase:2,deny"
	),
	match_log => {
		-error => [ qr/Lua script matched\./, 1 ],
		debug => [ qr/Test message\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "rule",
	comment => "SecRuleScript (lua relative nomatch)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 1
		SecRuleScript "test.lua" "phase:2,deny"
	),
	match_log => {
		-error => [ qr/Lua script matched\./, 1 ],
		debug => [ qr/Test message\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "rule",
	comment => "SecRuleScript (lua relative match)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 1
		SecRuleScript "match.lua" "phase:2,deny"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 403 \(phase 2\)\. Lua script matched\./, 1 ],
		debug => [ qr/Test message\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
