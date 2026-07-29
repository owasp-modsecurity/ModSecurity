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

### removeComments C, SQL style
{
	type => "rule",
	comment => "transformation: removeComments, C, SQL style",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRuleEngine On
		SecRule ARGS "\@rx UNIONSELECT" "id:941110,phase:2,deny,t:removeComments"
	),
	match_log => {
		debug => [ qr/T \(0\) removeComments: "UNIONSELECT"/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?q=UNION%2F%2A%2A%2F%2F%2A%2A%2FSELECT"
	),
},

### removeComments HTML style
{
	type => "rule",
	comment => "transformation: removeComments, HTML style",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRuleEngine On
		SecRule ARGS "\@rx foobar" "id:941110,phase:2,deny,t:removeComments"
	),
	match_log => {
		debug => [ qr/T \(0\) removeComments: "foobar"/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?q=foo%3C%21--%20comment%201%20--%3E%3C%21--%20comment%202%20--%3Ebar"
	),
},


