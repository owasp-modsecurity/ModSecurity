### Test the SecStatusEngine

# On
{
	type => "misc",
	comment => "Setting SecStatusEngine to On",
	conf => qq(
		SecRuleEngine On
		SecStatusEngine On
	),
	match_log => {
		error => [ qr/ModSecurity: StatusEngine call successfully sent/, 1],
		-error => [ qr/Status engine is currently disabled, enable it by set SecStatusEngine to On/, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},
# Off
{
	type => "misc",
	comment => "Setting SecStatusEngine to Off",
	conf => qq(
		SecRuleEngine On
		SecStatusEngine Off
	),
	match_log => {
		-error => [ qr/ModSecurity: StatusEngine call successfully sent/, 1],
		error => [ qr/Status engine is currently disabled, enable it by set SecStatusEngine to On/, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},
# On and SecServerSignature
{
	type => "misc",
	comment => "SecStatusEngine On using SecServerSignature",
	conf => qq(
		SecRuleEngine On
		SecServerSignature "SpiderServer v0.1a"
		SecStatusEngine On
	),
	match_log => {
		error => [ qr/ModSecurity: StatusEngine call successfully sent/, 1],
		-error => [ qr/StatusEngine call: .*SpiderServer v0.1a.*/, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},
# On and SecServerSignature
{
	type => "misc",
	comment => "SecStatusEngine On/SecServerSignature - checking signature",
	conf => qq(
		SecRuleEngine On
		SecServerSignature "SpiderServer v0.1a"
		SecStatusEngine On
	),
	match_log => {
		error => {
			apache => [ qr/StatusEngine call: \"[0-9]+.[0-9]+.[0-9]+[-RC]*[0-9]*\,Apache/, 1],
			nginx => [ qr/StatusEngine call: \"[0-9]+.[0-9]+.[0-9]+[-RC]*[0-9]*\,nginx/, 1],
		}
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},
# On and SecServerSignature
{
	type => "misc",
	comment => "SecStatusEngine On - checking signature",
	conf => qq(
		SecStatusEngine On
	),
	match_log => {
		error => {
			apache => [ qr/StatusEngine call: \"[0-9]+.[0-9]+.[0-9]+[-RC]*[0-9]*\,Apache/, 1],
			nginx => [ qr/StatusEngine call: \"[0-9]+.[0-9]+.[0-9]+[-RC]*[0-9]*\,nginx/, 1],
		}
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

