### Transformation tests

# NOTE: individual tests done in unit tests

# TODO: t:none to override default
# TODO: t:none inline
# TODO: combined
# TODO: caching

{
	type => "action",
	comment => "t:htmlEntityDecode test - GHSA-cxqf-vgrr-xxrv regression",
	conf => qq(
		SecRuleEngine On
		SecDebugLogLevel 9
		SecDebugLog $ENV{DEBUG_LOG}
		SecRequestBodyAccess On
		SecRule REQUEST_HEADERS:Content-Type "^application/json" "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS "\@contains javascript:exec" "id:1,phase:2,deny,t:trim,t:lowercase,t:htmlEntityDecode"
	),
	match_log => {
		debug => [ "htmlEntityDecode: \"javascript:execute_my_code\\(\\);", 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
		],
		"{\"q\":\"javascript&colon;execute_my_code();\"}",
	),
},
