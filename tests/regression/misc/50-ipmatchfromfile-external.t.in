### ipMatchFromFile external resource

{
	type => "misc",
	comment => "ipMatchFromFile",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On
		SecRule REMOTE_ADDR "\@ipMatchFromFile https://www.modsecurity.org/modsecurity-regression-test.txt" "id:10500,pass"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. IPmatchFromFile: \"127.0.0.1\" matched at REMOTE_ADDR./, 1],
		debug => [ qr/IPmatchFromFile: \"127.0.0.1\" matched at REMOTE_ADDR./, 1 ],
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
	comment => "ipMatchFromFile - 404 download",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On
		SecRemoteRulesFailAction Warn
		SecRule REMOTE_ADDR "\@ipMatchFromFile https://www.modsecurity.org/modsecurity-regression-test-404.txt" "id:10500,pass"
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
	comment => "ipMatchFromFile - bad certificate name",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On
		SecRemoteRulesFailAction Warn
		SecRule REMOTE_ADDR "\@ipMatchFromFile https://status.modsecurity.org/modsecurity-regression-test-huge-ip-list.txt" "id:10500,pass"
	),
	match_log => {
		error => [ qr/ModSecurity: Problems loading external resources: Failed to download: \"https:\/\/status.modsecurity.org\/modsecurity-regression-test-huge-ip-list.txt\" error: SSL peer certificate or SSH remote key was not OK./, 1],
	},
},

