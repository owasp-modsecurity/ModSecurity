### Test for SecRuleScript

# Python
{
	type => "rule",
	comment => "SecRuleScript (Python match)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRuleScript "script.py" "phase:2,deny"
	),
	match_log => {
		-error => [ qr/Python script matched\./, 1 ],
		debug => [ qr/Python test message\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?foo=bar&foo2=bar2",
	),
}

