### Test for JSON parser

### 
# OK
{
	type => "rule",
	comment => "json parser",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "application/json" \\
		     "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS:foo "bar" "id:'200441',phase:3,log"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Pattern match "bar" at ARGS:foo.|ModSecurity: JSON support was not enabled/s, 1 ],
		debug => [ qr/Adding JSON argument 'foo' with value 'bar'|JSON support was not enabled/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
		],
		normalize_raw_request_data(
			q(
				{
					"foo":"bar",
					"mod":"sec"
				}
			),
		),
	),
}

