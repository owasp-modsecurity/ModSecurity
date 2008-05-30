### Test basic targets

# ARGS
{
	type => "target",
	comment => "ARGS (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS "val1" "phase:2,log,pass"
		SecRule ARGS "val2" "phase:2,log,pass"
	),
	match_log => {
		error => [ qr/Pattern match "val1" at ARGS.*Pattern match "val2" at ARGS/s, 1 ],
		debug => [ qr/Adding request argument \(QUERY_STRING\): name "arg1", value "val1".*Adding request argument \(QUERY_STRING\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?arg1=val1&arg2=val2",
	),
},
{
	type => "target",
	comment => "ARGS (post)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS "val1" "phase:2,log,pass"
		SecRule ARGS "val2" "phase:2,log,pass"
	),
	match_log => {
		error => [ qr/Pattern match "val1" at ARGS.*Pattern match "val2" at ARGS/s, 1 ],
		debug => [ qr/Adding request argument \(BODY\): name "arg1", value "val1".*Adding request argument \(BODY\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

# ARGS_COMBINED_SIZE
{
	type => "target",
	comment => "ARGS_COMBINED_SIZE (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule ARGS_COMBINED_SIZE "\@eq 16" "phase:2,log,pass"
	),
	match_log => {
		error => [ qr/Operator EQ matched 16 at ARGS_COMBINED_SIZE\./s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?arg1=val1&arg2=val2",
	),
},
{
	type => "target",
	comment => "ARGS_COMBINED_SIZE (post)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule ARGS_COMBINED_SIZE "\@eq 16" "phase:2,log,pass"
	),
	match_log => {
		error => [ qr/Operator EQ matched 16 at ARGS_COMBINED_SIZE\./s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

# ARGS_NAMES
{
	type => "target",
	comment => "ARGS_NAMES (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS_NAMES "arg1" "phase:2,log,pass"
		SecRule ARGS_NAMES "arg2" "phase:2,log,pass"
	),
	match_log => {
		error => [ qr/Pattern match "arg1" at ARGS.*Pattern match "arg2" at ARGS/s, 1 ],
		debug => [ qr/Adding request argument \(QUERY_STRING\): name "arg1", value "val1".*Adding request argument \(QUERY_STRING\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?arg1=val1&arg2=val2",
	),
},
{
	type => "target",
	comment => "ARGS_NAMES (post)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS_NAMES "arg1" "phase:2,log,pass"
		SecRule ARGS_NAMES "arg2" "phase:2,log,pass"
	),
	match_log => {
		error => [ qr/Pattern match "arg1" at ARGS_NAMES.*Pattern match "arg2" at ARGS_NAMES/s, 1 ],
		debug => [ qr/Adding request argument \(BODY\): name "arg1", value "val1".*Adding request argument \(BODY\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

# ARGS_GET
{
	type => "target",
	comment => "ARGS_GET (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS_GET "val1" "phase:2,log,pass"
		SecRule ARGS_GET "val2" "phase:2,log,pass"
	),
	match_log => {
		error => [ qr/Pattern match "val1" at ARGS_GET.*Pattern match "val2" at ARGS_GET/s, 1 ],
		debug => [ qr/Adding request argument \(QUERY_STRING\): name "arg1", value "val1".*Adding request argument \(QUERY_STRING\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?arg1=val1&arg2=val2",
	),
},
{
	type => "target",
	comment => "ARGS_GET (post)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS_GET "val1" "phase:2,log,pass"
		SecRule ARGS_GET "val2" "phase:2,log,pass"
	),
	match_log => {
		-error => [ qr/Pattern match/, 1 ],
		debug => [ qr/Adding request argument \(BODY\): name "arg1", value "val1".*Adding request argument \(BODY\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

# ARGS_GET_NAMES
{
	type => "target",
	comment => "ARGS_GET_NAMES (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS_GET_NAMES "arg1" "phase:2,log,pass"
		SecRule ARGS_GET_NAMES "arg2" "phase:2,log,pass"
	),
	match_log => {
		error => [ qr/Pattern match "arg1" at ARGS_GET.*Pattern match "arg2" at ARGS_GET/s, 1 ],
		debug => [ qr/Adding request argument \(QUERY_STRING\): name "arg1", value "val1".*Adding request argument \(QUERY_STRING\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?arg1=val1&arg2=val2",
	),
},
{
	type => "target",
	comment => "ARGS_GET_NAMES (post)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS_GET_NAMES "arg1" "phase:2,log,pass"
		SecRule ARGS_GET_NAMES "arg2" "phase:2,log,pass"
	),
	match_log => {
		-error => [ qr/Pattern match/, 1 ],
		debug => [ qr/Adding request argument \(BODY\): name "arg1", value "val1".*Adding request argument \(BODY\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

# ARGS_POST
{
	type => "target",
	comment => "ARGS_POST (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS_POST "val1" "phase:2,log,pass"
		SecRule ARGS_POST "val2" "phase:2,log,pass"
	),
	match_log => {
		-error => [ qr/Pattern match/, 1 ],
		debug => [ qr/Adding request argument \(QUERY_STRING\): name "arg1", value "val1".*Adding request argument \(QUERY_STRING\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?arg1=val1&arg2=val2",
	),
},
{
	type => "target",
	comment => "ARGS_POST (post)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS_POST "val1" "phase:2,log,pass"
		SecRule ARGS_POST "val2" "phase:2,log,pass"
	),
	match_log => {
		error => [ qr/Pattern match "val1" at ARGS_POST.*Pattern match "val2" at ARGS_POST/s, 1 ],
		debug => [ qr/Adding request argument \(BODY\): name "arg1", value "val1".*Adding request argument \(BODY\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

# ARGS_POST_NAMES
{
	type => "target",
	comment => "ARGS_POST_NAMES (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS_POST_NAMES "arg1" "phase:2,log,pass"
		SecRule ARGS_POST_NAMES "arg2" "phase:2,log,pass"
	),
	match_log => {
		-error => [ qr/Pattern match/, 1 ],
		debug => [ qr/Adding request argument \(QUERY_STRING\): name "arg1", value "val1".*Adding request argument \(QUERY_STRING\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?arg1=val1&arg2=val2",
	),
},
{
	type => "target",
	comment => "ARGS_POST_NAMES (post)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule ARGS_POST_NAMES "arg1" "phase:2,log,pass"
		SecRule ARGS_POST_NAMES "arg2" "phase:2,log,pass"
	),
	match_log => {
		error => [ qr/Pattern match "arg1" at ARGS_POST.*Pattern match "arg2" at ARGS_POST/s, 1 ],
		debug => [ qr/Adding request argument \(BODY\): name "arg1", value "val1".*Adding request argument \(BODY\): name "arg2", value "val2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"arg1=val1&arg2=val2",
	),
},

# TODO: AUTH_TYPE
# TODO: ENV
# TODO: FILES
# TODO: FILES_COMBINED_SIZE
# TODO: FILES_NAMES
# TODO: FILES_SIZES
# TODO: FILES_TMPNAMES
# TODO: GEO
# TODO: HIGHEST_SEVERITY
# TODO: MATCHED_VAR
# TODO: MATCHED_VAR_NAME
# TODO: MODSEC_BUILD
# TODO: MULTIPART_CRLF_LF_LINES
# TODO: MULTIPART_STRICT_ERROR
# TODO: MULTIPART_UNMATCHED_BOUNDARY
# TODO: PATH_INFO
# TODO: QUERY_STRING
# TODO: REMOTE_ADDR
# TODO: REMOTE_HOST
# TODO: REMOTE_PORT
# TODO: REMOTE_USER
# TODO: REQBODY_PROCESSOR
# TODO: REQBODY_PROCESSOR_ERROR
# TODO: REQBODY_PROCESSOR_ERROR_MSG
# TODO: REQUEST_BASENAME
# TODO: REQUEST_BODY
# TODO: REQUEST_COOKIES
# TODO: REQUEST_COOKIES_NAMES
# TODO: REQUEST_FILENAME
# TODO: REQUEST_HEADERS
# TODO: REQUEST_HEADERS_NAMES
# TODO: REQUEST_LINE
# TODO: REQUEST_METHOD
# TODO: REQUEST_PROTOCOL
# TODO: REQUEST_URI
# TODO: REQUEST_URI_RAW
# TODO: RESPONSE_BODY
# TODO: RESPONSE_CONTENT_LENGTH
# TODO: RESPONSE_CONTENT_TYPE
# TODO: RESPONSE_HEADERS
# TODO: RESPONSE_HEADERS_NAMES
# TODO: RESPONSE_PROTOCOL
# TODO: RESPONSE_STATUS
# TODO: RULE
# TODO: SCRIPT_BASENAME
# TODO: SCRIPT_FILENAME
# TODO: SCRIPT_GID
# TODO: SCRIPT_GROUPNAME
# TODO: SCRIPT_MODE
# TODO: SCRIPT_UID
# TODO: SCRIPT_USERNAME
# TODO: SERVER_ADDR
# TODO: SERVER_NAME
# TODO: SERVER_PORT
# TODO: SESSION
# TODO: SESSIONID
# TODO: TIME
# TODO: TIME_DAY
# TODO: TIME_EPOCH
# TODO: TIME_HOUR
# TODO: TIME_MIN
# TODO: TIME_MON
# TODO: TIME_SEC
# TODO: TIME_WDAY
# TODO: TIME_YEAR
# TODO: TX
# TODO: USERID
# TODO: WEBAPPID
# TODO: WEBSERVER_ERROR_LOG
# TODO: XML

