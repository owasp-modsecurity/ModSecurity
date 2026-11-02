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
{
	type => "rule",
	comment => "SecRuleScript m.setvar without a collection returns nil",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRuleScript "setvar-error-nodot.lua" "phase:2,pass"
	),
	match_log => {
		debug => [ qr/m.setvar: Must specify a collection using dot character.*m.setvar\(\) without a collection returned: nil/s, 1 ],
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
	comment => "SecRuleScript m.setvar with one argument returns nil",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRuleScript "setvar-error-onearg.lua" "phase:2,pass"
	),
	match_log => {
		debug => [ qr/m.setvar: Failed m.setvar function must have 2 arguments.*m.setvar\(\) with one argument returned: nil/s, 1 ],
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
	comment => "SecRuleScript m.setvar sets a TX variable and returns nil",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRuleScript "setvar-tx.lua" "phase:2,pass"
	),
	match_log => {
		debug => [ qr/m.setvar\(\) with TX returned: nil.*tx.a is now: x/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
