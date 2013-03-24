### Tests for basic rule components

# SecAction
{
	type => "rule",
	comment => "SecAction (override default)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 4
		SecAction "nolog,id:500001"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
		debug => [ qr/Warning\. Unconditional match in SecAction\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# SecRule
{
	type => "rule",
	comment => "SecRule (no action)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 5
        SecDefaultAction "phase:2,deny,status:403"
        SecRule ARGS:test "value,id:500032"
	),
	match_log => {
		error => [ qr/ModSecurity: /, 1 ],
		debug => [ qr/Rule [0-9a-f]+: SecRule "ARGS:test" "\@rx value" "phase:2,deny,status:403"$/m, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?test=value",
	),
},
{
	type => "rule",
	comment => "SecRule (action)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 5
        SecDefaultAction "phase:2,pass"
        SecRule ARGS:test "value" "deny,status:403,id:500033"
	),
	match_log => {
		error => [ qr/ModSecurity: /, 1 ],
		debug => [ qr/Rule [0-9a-f]+: SecRule "ARGS:test" "\@rx value" "phase:2,deny,status:403"$/m, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?test=value",
	),
},
{
	type => "rule",
	comment => "SecRule (chain)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 5
        SecDefaultAction "phase:2,log,noauditlog,pass,tag:foo"
        SecRule ARGS:test "value" "chain,phase:2,deny,status:403,id:500034"
        SecRule &ARGS "\@eq 1" "chain,setenv:tx.foo=bar,id:500035"
        SecRule REQUEST_METHOD "\@streq GET,id:500036"
	),
	match_log => {
		error => [ qr/ModSecurity: /, 1 ],
		debug => [ qr/Rule [0-9a-f]+: SecRule "ARGS:test" "\@rx value" "phase:2,log,noauditlog,tag:foo,chain,deny,status:403"\r?\n.*Rule [0-9a-f]+: SecRule "&ARGS" "\@eq 1" "chain,setenv:tx.foo=bar"\r?\n.*Rule [0-9a-f]+: SecRule "REQUEST_METHOD" "\@streq GET"\r?\n/s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?test=value",
	),
},
