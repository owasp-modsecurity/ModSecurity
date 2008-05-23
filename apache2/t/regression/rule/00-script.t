
# Lua
{
	type => "config",
	comment => "SecRuleScript (lua)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 1
		SecRuleScript "test.lua" "phase:1"
	),
	match_log => {
		debug => [ qr/Test message\./, 1 ],
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
