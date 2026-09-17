### Regression tests for issue #2807: JSON completion errors must reach phase 2.

{
	type => "rule",
	comment => "json parser - issue #2807 - premature EOF uses the phase 2 error rule",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		error => [ qr/Access denied with code 400.*\[id "200002"\].*premature EOF/s, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},

{
	type => "rule",
	comment => "json parser - issue #2807 - incomplete object honors a custom error status",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:422,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		error => [ qr/Access denied with code 422.*\[id "200002"\].*premature EOF/s, 1 ],
	},
	match_response => {
		status => qr/^422$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{ "id" : "123"',
	),
},

{
	type => "rule",
	comment => "json parser - issue #2807 - premature EOF in DetectionOnly still runs phase 2",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		error => [ qr/Warning.*\[id "200002"\].*premature EOF/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},

{
	type => "rule",
	comment => "json parser - issue #2807 - premature EOF permits a non-disruptive error rule",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,pass,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		error => [ qr/Warning.*\[id "200002"\].*premature EOF/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},

{
	type => "rule",
	comment => "json parser - issue #2807 - valid empty object does not set REQBODY_ERROR",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		-error => [ qr/\[id "200002"\]/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{}',
	),
},

{
	type => "rule",
	comment => "json parser - issue #2807 - empty body retains existing behavior",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		-error => [ qr/\[id "200002"\]/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'',
	),
},

{
	type => "rule",
	comment => "json parser - issue #2807 - in-stream syntax errors still use the error rule",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		error => [ qr/Access denied with code 400.*\[id "200002"\]/s, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{ id : "123" }',
	),
},

{
	type => "rule",
	comment => "json parser - issue #2807 - valid JSON arguments are still inspected",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
		SecRule ARGS:foo "\@streq bar" "id:200003,phase:2,t:none,log,deny,status:403"
	),
	match_log => {
		error => [ qr/Access denied with code 403.*\[id "200003"\]/s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{"foo":"bar"}',
	),
},

{
	type => "rule",
	comment => "json parser - issue #2807 - chunked premature EOF reaches phase 2",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		error => [ qr/Access denied with code 400.*\[id "200002"\].*premature EOF/s, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: application/json
			Transfer-Encoding: chunked

		),
	) . encode_chunked('{ "id" : "123"', 3),
},

{
	type => "rule",
	comment => "json parser - issue #2807 - no error rule allows the request",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecAction "id:200005,phase:2,pass,log,msg:'JSON phase 2 reached'"
	),
	match_log => {
		error => [ qr/JSON parser error:.*premature EOF.*\[id "200005"\].*JSON phase 2 reached/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},

{
	type => "rule",
	comment => "json parser - issue #2807 - excluded error rule allows the request",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQUEST_URI "^/test[.]txt\$" \\
		    "id:200004,phase:1,pass,nolog,ctl:ruleRemoveById=200002"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error'"
		SecAction "id:200005,phase:2,pass,log,msg:'JSON phase 2 reached'"
	),
	match_log => {
		error => [ qr/JSON parser error:.*premature EOF.*\[id "200005"\].*JSON phase 2 reached/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},
