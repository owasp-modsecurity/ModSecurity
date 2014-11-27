### pmfromfile external resource

{
	type => "misc",
	comment => "pmfromfile",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On
		SecRule REQUEST_FILENAME "\@pmFromFile https://www.modsecurity.org/modsecurity-regression-test.txt" "id:'123',phase:2,log,pass,t:none"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Matched phrase \"127.0.0.1\" at REQUEST_FILENAME./, 1],
		debug => [ qr/Matched phrase \"127.0.0.1\" at REQUEST_FILENAME/, 1 ],
		-error => [ qr/ModSecurity: Problems loading external resources:/, 1],
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
{
	type => "misc",
	comment => "pmfromfile - 404 download",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On
		SecRemoteRulesFailAction Warn
		SecRule REQUEST_FILENAME "\@pmFromFile https://www.modsecurity.org/modsecurity-regression-test-404.txt" "id:'123',phase:2,log,pass,t:none"

	),
	match_log => {
		error => [ qr/ModSecurity: Problems loading external resources: Failed to download: \"https:\/\/www.modsecurity.org\/modsecurity-regression-test-404.txt\" error: HTTP response code said error./, 1],
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
{
	type => "misc",
	comment => "pmfromfile - bad certificate name",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On
		SecRemoteRulesFailAction Warn
		SecRule REQUEST_FILENAME "\@pmFromFile https://status.modsecurity.org/modsecurity-regression-test.txt" "id:'123',phase:2,log,pass,t:none"

	),
	match_log => {
		error => [ qr/ModSecurity: Problems loading external resources: Failed to download: \"https:\/\/status.modsecurity.org\/modsecurity-regression-test.txt\" error: SSL peer certificate or SSH remote key was not OK./, 1],
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

