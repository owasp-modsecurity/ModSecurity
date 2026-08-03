### Transformation tests

### pass, t:trim
{
	type => "rule",
	comment => "transformation: pass,t:trim",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRuleEngine On
		SecRule ARGS "\@contains test " "id:1,pass,t:trim"
	),
	match_log => {
		debug => [ qr/T \(0\) trim: "test"/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?param1=%20%20test%20%20&param2=test2"
	),
},

### lowercase
{
	type => "rule",
	comment => "transformation: pass,t:trim,t:lowercase",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRuleEngine On
		SecRule ARGS "\@contains wee" "id:1,pass,t:trim,t:lowercase"
	),
	match_log => {
		debug => [ qr/T \(0\) lowercase: "wee"/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?param1=%20%20WEE%20%20&param2=test2"
	),
},

### complex chained tfn
{
	type => "rule",
	comment => "transformation: none,utf8toUnicode,urlDecodeUni,htmlEntityDecode,jsDecode,cssDecode,removeNulls",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRuleEngine On
		SecRule ARGS "\@rx \(?i\)<script[^>]*>[\\s\\S]*?" "id:941110,phase:2,deny,capture,t:none,t:utf8toUnicode,t:urlDecodeUni,t:htmlEntityDecode,t:jsDecode,t:cssDecode,t:removeNulls"
	),
	match_log => {
		debug => [ qr/T \(0\) removeNulls: "<script >alert\(1\)<\/script>"/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?var=%ef%bc%9cscript%20%ef%bc%9ealert%281%29%ef%bc%9c/script%ef%bc%9e"
	),
},

### invalide base64 encoded with base64DecodeExt
{
	type => "rule",
	comment => "transformation: none,base64DecodeExt",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRuleEngine On
		SecRule \&REQUEST_HEADERS:foo "\@gt 0" "id:110,phase:1,deny,capture,t:none,t:base64DecodeExt"
	),
	match_log => {
		debug => [ qr/T \(0\) base64DecodeExt: ""/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Foo" => "dzBzV==YWxlcnQoMSk="
		]
	),
},

### decode base64 string with '-'
{
	type => "rule",
	comment => "transformation: none,base64DecodeExt",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Authorization "\@rx \(?i\)^Bearer[\\s\\x0b]+\(eyJ[\\-0-9_a-z]+\)\\." "id:100,phase:1,deny,capture,t:none,chain"
		SecRule TX:1 "\@rx \(?i\)alg[^0-9A-Z_a-z]*:[^0-9A-Z_a-z]*none" "t:base64DecodeExt"
	),
	match_log => {
		debug => [ qr/T \(0\) base64DecodeExt: "{\"p\":\"xx~\",\"alg\":\"none\",\"typ\":\"JWT\"}"/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Authorization" => "Bearer eyJwIjoieHh-IiwiYWxnIjoibm9uZSIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJ0ZXN0In0."
		]
	),
},
