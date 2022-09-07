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
},
{
	type => "rule",
	comment => "json parser - issue #1576 - 1",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "application/json" \\
		     "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS "bar" "id:'200441',phase:3,log"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Pattern match "bar" at ARGS:foo.|ModSecurity: JSON support was not enabled/s, 1 ],
		debug => [ qr/ARGS:foo|ARGS:mod|ARGS:ops.ops.ops|ARGS:ops.ops.ops|ARGS:ops.ops|ARGS:ops.ops|ARGS:ops.ops.eins.eins|ARGS:ops.ops.eins.eins|ARGS:whee/, 1 ],
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
                    "mod":"sec",
                    "ops":[
                        [
                            "um",
                            "um e meio"
                        ],
                        "dois",
                        "tres",
                        {
                            "eins":[
                                "zwei",
                                "drei"
                            ]
                        }
                    ],
                    "whee":"lhebs"
                }
			),
		),
	),
},
{
	type => "rule",
	comment => "json parser - issue #1576 - 2",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "application/json" \\
		     "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS "um" "id:'200441',phase:3,log"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Pattern match "um" at ARGS:array.array|ModSecurity: JSON support was not enabled/s, 1 ],
		debug => [ qr/ARGS:array.array|ARGS:array.array/, 1 ],
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
                        [
                            "um",
                            "um e meio"
                        ]
			),
		),
	),
},
{
	type => "rule",
	comment => "json parser - issue #1576 - 3",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "application/json" \\
		     "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule ARGS "seis" "id:'200441',phase:3,log"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Pattern match "seis" at ARGS:array.array.cinco.|ModSecurity: JSON support was not enabled/s, 1 ],
		debug => [ qr/ARGS:array.array|ARGS:array.array|ARGS:array.array.tres|ARGS:array.array.cinco/, 1 ],
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
                        [
                            "um",
                            "um e meio",
                            {
                                "tres": "quatro",
                                "cinco": "seis"
                            }
                        ]
			),
		),
	),
},
{
	type => "rule",
	comment => "json parser - parsing depth not exceeded",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyJsonDepthLimit 5
		SecRule REQUEST_HEADERS:Content-Type "application/json" \\
		     "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200442',phase:2,log,deny,status:403,msg:'Failed to parse request body'"
	),
	match_log => {
		debug => [ qr/key/s, 1 ],
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
					"key1":{"key2":{"key3":{"key4":{"key5":"thevalue"}}}}
				}
			),
		),
	),
},
{
	type => "rule",
	comment => "json parser - parsing depth exceeded",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
                SecAuditEngine RelevantOnly
                SecAuditLog "$ENV{AUDIT_LOG}"
		SecDebugLogLevel 9
		SecRequestBodyJsonDepthLimit 3
		SecRule REQUEST_HEADERS:Content-Type "application/json" \\
		     "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200443',phase:2,log,deny,status:403,msg:'Failed to parse request body'"
	),
	match_log => {
		audit => [ qr/JSON parsing error: JSON depth limit exceeded/s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
		],
		normalize_raw_request_data(
			q(
				{
					"key1":{"key2":{"key3":{"key4":{"key5":"thevalue"}}}}
				}
			),
		),
	),
},
{
	type => "rule",
	comment => "json parser - no-key single value",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
                SecAuditEngine RelevantOnly
                SecAuditLog "$ENV{AUDIT_LOG}"
		SecDebugLogLevel 9
		SecRequestBodyJsonDepthLimit 3
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		     "id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200444',phase:2,log,deny,status:403,msg:'Failed to parse request body'"
		SecRule ARGS "\@streq 25" "id:'200445',phase:2,log,deny,status:403"
	),
	match_log => {
		audit => [ qr/200445/s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
		],
		normalize_raw_request_data(
			q(
				25
			),
		),
	),
}


