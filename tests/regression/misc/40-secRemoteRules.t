### SecRemoteRules

{
	type => "misc",
	comment => "SecRemoteRules load",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On
                SecRemoteRules 123 "https://www.modsecurity.org/modsecurity-regression-test-secremoterules.txt"
	),
	match_log => {
		error => [ qr/ModSecurity: Loaded 1 rule/, 1],
	},
},
{
	type => "misc",
	comment => "SecRemoteRules apply some remote rules",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On
                SecRemoteRules 123 "https://www.modsecurity.org/modsecurity-regression-test-secremoterules.txt"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Matched phrase \"127.0.0.1\" at REQUEST_FILENAME./, 1],
		debug => [ qr/Matched phrase \"127.0.0.1\" at REQUEST_FILENAME/, 1 ],
	},
	match_response => {
		status => qr/^404$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/127.0.0.1.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		#  Args
		"some_variable=-1' and 1=1 union/* foo */select load_file('/etc/passwd')--"
	),
},

