### libinjection.

{
	type => "misc",
	comment => "libinjection SQLi - with SQLi",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On

		SecRule REQUEST_BODY "\@detectSQLi" "id:192372,log,deny"
	),
	match_log => {
		error => [ qr/detected SQLi using libinjection/, 1],
		debug => [ qr/detected SQLi using libinjection/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		#  Args
		"some_variable=-1' and 1=1 union/* foo */select load_file('/etc/passwd')--"
	),
},
{
	type => "misc",
	comment => "libinjection SQLi - with SQLi, check capture",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On

		SecRule REQUEST_BODY "\@detectSQLi" "id:192372,log,deny,capture,chain"
		SecRule TX:0 "\@strEq s&1UE"
	),
	match_log => {
		error => [ qr/String match "s&1UE" at TX:0/, 1 ],
		debug => [ qr/String match "s&1UE" at TX:0/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		#  Args
		"some_variable=-1' and 1=1 union/* foo */select load_file('/etc/passwd')--"
	),
},
{
	type => "misc",
	comment => "libinjection SQLi - without SQLi",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On

		SecRule REQUEST_BODY "\@detectSQLi" "id:192372,log,deny"
	),
	match_log => {
		-error => [ qr/detected SQLi using libinjection/, 1],
		-debug => [ qr/detected SQLi using libinjection/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		#  Args
		"some_variable=hello cruel world"
	),
},
{
	type => "misc",
	comment => "libinjection XSS - with XSS",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On

		SecRule REQUEST_BODY "\@detectXSS" "id:192372,log,deny"
	),
	match_log => {
		error => [ qr/detected XSS using libinjection/, 1],
		debug => [ qr/detected XSS using libinjection/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		#  Args
		"some_variable=<a href=\"javascript:alert(1)\">"
	),
},
{
	type => "misc",
	comment => "libinjection XSS - with XSS, check capture",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On

		SecRule REQUEST_BODY "\@detectXSS" "id:192372,log,deny,capture,chain"
		SecRule TX:0 "\@strEq some_variable=<a href='javascript:alert(1)'>"
	),
	match_log => {
		error => [ qr/String match "some_variable=<a href='javascript:alert\(1\)'>" at TX:0/, 1],
		debug => [ qr/String match "some_variable=<a href='javascript:alert\(1\)'>" at TX:0/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		#  Args
		"some_variable=<a href='javascript:alert(1)'>"
	),
},
{
	type => "misc",
	comment => "libinjection XSS - without XSS",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On

		SecRule REQUEST_BODY "\@detectXSS" "id:192372,log,deny"
	),
	match_log => {
		-error => [ qr/detected XSS using libinjection/, 1],
		-debug => [ qr/detected XSS using libinjection/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		#  Args
		"some_variable=hello cruel world"
	),
}
