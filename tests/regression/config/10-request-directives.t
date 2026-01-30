### Tests for directives altering how a request is handled

# SecArgumentSeparator
{
	type => "config",
	comment => "SecArgumentSeparator (get-pos)",
	conf => q(
		SecRuleEngine On
		SecArgumentSeparator ";"
		SecRule ARGS:a "@streq 1" "phase:1,deny,chain,id:500215"
		SecRule ARGS:b "@streq 2" ""
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\)\. String match "2" at ARGS:b\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?a=1;b=2",
	),
},
{
	type => "config",
	comment => "SecArgumentSeparator (get-neg)",
	conf => q(
		SecRuleEngine On
		SecRule ARGS:a "@streq 1" "phase:1,deny,chain,id:500217"
		SecRule ARGS:b "@streq 2" ""
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?a=1;b=2",
	),
},
{
	type => "config",
	comment => "SecArgumentSeparator (post-pos)",
	conf => q(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecArgumentSeparator ";"
		SecRule ARGS:a "@streq 1" "phase:2,deny,chain,id:500219"
		SecRule ARGS:b "@streq 2" ""
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 2\)\. String match "2" at ARGS:b\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1;b=2",
	),
},
{
	type => "config",
	comment => "SecArgumentSeparator (post-neg)",
	conf => q(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRule ARGS:a "@streq 1" "phase:2,deny,id:500221"
		SecRule ARGS:b "@streq 2" "phase:2,deny,id:500222"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1;b=2",
	),
},

# SecRequestBodyAccess
{
	type => "config",
	comment => "SecRequestBodyAccess (pos)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRule ARGS:a "\@streq 1" "phase:2,deny,chain,id:500223"
		SecRule ARGS:b "\@streq 2" ""
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 2\)\. String match "2" at ARGS:b\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1&b=2",
	),
},
{
	type => "config",
	comment => "SecRequestBodyAccess (neg)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess Off
		SecRule ARGS:a "\@streq 1" "phase:2,deny,id:500225"
		SecRule ARGS:b "\@streq 2" "phase:2,deny,id:500226"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1&b=2",
	),
},

# SecRequestBodyLimit
{
	type => "config",
	comment => "SecRequestBodyLimit (equal)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyLimit 7
	),
	match_log => {
		-error => [ qr/Request body is larger than the configured limit/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1&b=2",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimit (greater)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyLimit 5
	),
	match_log => {
		error => [ qr/Request body .*is larger than the configured limit \(5\)\./, 1 ],
	},
	match_response => {
		status => qr/^413$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1&b=2",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimit (equal - chunked)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyLimit 276
	),
	match_log => {
		-error => [ qr/Request body is larger than the configured limit/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: multipart/form-data; boundary=---------------------------69343412719991675451336310646
			Transfer-Encoding: chunked

		),
	)
	.encode_chunked(
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="a"

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				2
				-----------------------------69343412719991675451336310646--
			)
		),
		1024
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimit (greater - chunked)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyLimit 256
	),
	match_log => {
		error => [ qr/Request body .*is larger than the configured limit \(256\)\./, 1 ],
	},
	match_response => {
		status => qr/^413$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: multipart/form-data; boundary=---------------------------69343412719991675451336310646
			Transfer-Encoding: chunked

		),
	)
	.encode_chunked(
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="a"

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				2
				-----------------------------69343412719991675451336310646--
			)
		),
		1024
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimit (ctl:ruleEngine=off)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyLimit 5

		SecAction "phase:1,pass,nolog,ctl:ruleEngine=off,id:500081"
		SecRule REQUEST_BODY "." "phase:2,deny,id:500227"
	),
	match_log => {
		-error => [ qr/Request body .*is larger than the configured limit/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1&b=2",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimit (ctl:requestBodyAccess=off)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyLimit 5

		SecAction "phase:1,pass,nolog,ctl:requestBodyAccess=off,id:500082"
		SecRule REQUEST_BODY "." "phase:2,deny,id:500228"
	),
	match_log => {
		-error => [ qr/Request body .*is larger than the configured limit/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1&b=2",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimit (ctl:ruleEngine=off - chunked)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyLimit 256

		SecAction "phase:1,pass,nolog,ctl:ruleEngine=off,id:500083"
		SecRule REQUEST_BODY "." "phase:2,deny,id:500229"
	),
	match_log => {
		-error => [ qr/Request body .*is larger than the configured limit/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: multipart/form-data; boundary=---------------------------69343412719991675451336310646
			Transfer-Encoding: chunked

		),
	)
	.encode_chunked(
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="a"

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				2
				-----------------------------69343412719991675451336310646--
			)
		),
		1024
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimit (ctl:requestBodyAccess=off - chunked)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyLimit 256

		SecAction "phase:1,pass,nolog,ctl:requestBodyAccess=off,id:500084"
		SecRule REQUEST_BODY "." "phase:2,deny,id:500230"
	),
	match_log => {
		-error => [ qr/Request body .*is larger than the configured limit \(256\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: multipart/form-data; boundary=---------------------------69343412719991675451336310646
			Transfer-Encoding: chunked

		),
	)
	.encode_chunked(
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="a"

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				2
				-----------------------------69343412719991675451336310646--
			)
		),
		1024
	),
},

# SecRequestBodyInMemoryLimit
{
	type => "config",
	comment => "SecRequestBodyInMemoryLimit (equal)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimit 1000
		SecRequestBodyInMemoryLimit 276
	),
	match_log => {
		-debug => [ qr/Input filter: Request too large to store in memory, switching to disk\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: multipart/form-data; boundary=---------------------------69343412719991675451336310646
			Transfer-Encoding: chunked

		),
	)
	.encode_chunked(
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="a"

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				2
				-----------------------------69343412719991675451336310646--
			)
		),
		1024
	),
},
{
	type => "config",
	comment => "SecRequestBodyInMemoryLimit (greater)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimit 1000
		SecRequestBodyInMemoryLimit 16
	),
	match_log => {
		debug => [ qr/Input filter: Request too large to store in memory, switching to disk\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: multipart/form-data; boundary=---------------------------69343412719991675451336310646
			Transfer-Encoding: chunked

		),
	)
	.encode_chunked(
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="a"

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				2
				-----------------------------69343412719991675451336310646--
			)
		),
		1024
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (multipart/greater - chunked)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyLimit 20
		SecRule MULTIPART_STRICT_ERROR "!\@eq 0" \\
			"id:'200003',phase:2,t:none,log,deny,status:400, \\
			msg:'Multipart request body failed strict validation: \\
			PE %{REQBODY_PROCESSOR_ERROR}, \\
			BQ %{MULTIPART_BOUNDARY_QUOTED}, \\
			BW %{MULTIPART_BOUNDARY_WHITESPACE}, \\
			DB %{MULTIPART_DATA_BEFORE}, \\
			DA %{MULTIPART_DATA_AFTER}, \\
			HF %{MULTIPART_HEADER_FOLDING}, \\
			LF %{MULTIPART_LF_LINE}, \\
			SM %{MULTIPART_MISSING_SEMICOLON}, \\
			IQ %{MULTIPART_INVALID_QUOTING}, \\
			IP %{MULTIPART_INVALID_PART}, \\
			IH %{MULTIPART_INVALID_HEADER_FOLDING}, \\
			FL %{MULTIPART_FILE_LIMIT_EXCEEDED}'"
		SecRule MULTIPART_UNMATCHED_BOUNDARY "!\@eq 0" \\
			"id:'200004',phase:2,t:none,log,deny,msg:'Multipart parser detected a possible unmatched boundary.'"
	),
	match_log => {
		debug => [ qr/Request body is larger than the configured limit \(20\)./, 1 ],
	},
	match_response => {
		status => qr/^413$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: multipart/form-data; boundary=---------------------------69343412719991675451336310646
			Transfer-Encoding: chunked

		),
	)
	.encode_chunked(
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="a"

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				2
				-----------------------------69343412719991675451336310646--
			)
		),
		1024
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (plain/greater)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyLimit 131072
	),
	match_log => {
		-debug => [ qr/Request body is larger than the configured limit \(131072\)./, 1 ],
	},
	match_response => {
		status => qr/^413$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
		],
		normalize_raw_request_data(
			q(
				{
					) . "'abcdefghijlmnopq'='abcdefghijlmnopqrstuvxz',\\n" x 99000 . q(
				},
			),
		),
	),
},


{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/greater - chunked)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 131072
		SecRule MULTIPART_STRICT_ERROR "!\@eq 0" \\
			"id:'200003',phase:2,t:none,log,deny,status:400, \\
			msg:'Multipart request body failed strict validation: \\
			PE %{REQBODY_PROCESSOR_ERROR}, \\
			BQ %{MULTIPART_BOUNDARY_QUOTED}, \\
			BW %{MULTIPART_BOUNDARY_WHITESPACE}, \\
			DB %{MULTIPART_DATA_BEFORE}, \\
			DA %{MULTIPART_DATA_AFTER}, \\
			HF %{MULTIPART_HEADER_FOLDING}, \\
			LF %{MULTIPART_LF_LINE}, \\
			SM %{MULTIPART_MISSING_SEMICOLON}, \\
			IQ %{MULTIPART_INVALID_QUOTING}, \\
			IP %{MULTIPART_INVALID_PART}, \\
			IH %{MULTIPART_INVALID_HEADER_FOLDING}, \\
			FL %{MULTIPART_FILE_LIMIT_EXCEEDED}'"
		SecRule MULTIPART_UNMATCHED_BOUNDARY "!\@eq 0" \\
			"id:'200004',phase:2,t:none,log,deny,msg:'Multipart parser detected a possible unmatched boundary.'"
	),
	match_log => {
		-error => [ qr/Multipart parsing error: Multipart: Final boundary missing./, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: multipart/form-data; boundary=---------------------------69343412719991675451336310646
			Transfer-Encoding: chunked

		),
	)
	.encode_chunked(
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="a"

				1) . "a" x 131072 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				2) . "b" x 131072 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		131072*3
	),
},
# Known issue on nginx, disable it for now.
#{
#	type => "config",
#	comment => "SecRequestBodyLimitAction ProcessPartial (plain/greater)",
#	conf => qq(
#		SecRuleEngine On
#		SecDebugLog $ENV{DEBUG_LOG}
#		SecDebugLogLevel 9
#		SecRequestBodyAccess On
#		SecRequestBodyLimitAction ProcessPartial
#		SecRequestBodyLimit 131072
#	),
#	match_log => {
#		-debug => [ qr/Request body is larger than the configured limit/, 1],
#	},
#	match_response => {
#		status => qr/^200$/,
#	},
#	request => new HTTP::Request(
#		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
#		[
#			"Content-Type" => "application/json",
#		],
#		normalize_raw_request_data(
#			q(
#				{
#					) . "'abcdefghijlmnopq'='abcdefghijlmnopqrstuvxz',\\n" x 99000 . q(
#				},
#			),
#		),
#	),
#},

# SecRequestBodyLimitAction ProcessPartial
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/bad_name before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 59
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_NAME "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="bad_name"
			),
		) . "\r\n" . "a",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/bad_name after limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 58
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_NAME "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="bad_name"
			),
		) . "\r\n",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/bad_filename before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 81
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_FILENAME "bad_filename" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="bad_filename"
			),
		) . "\r\n" . "a",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/bad_filename after limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 80
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_FILENAME "bad_filename" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="bad_filename"
			),
		) . "\r\n",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/no epilogue)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 176
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
	),
	match_log => {
		-error => [ qr/Multipart parsing error: Multipart: Final boundary missing./, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
		],
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="name1"

				value1
				-----------------------------69343412719991675451336310646--),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CR after limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 176
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
	),
	match_log => {
		-error => [ qr/Multipart parsing error: Multipart: Final boundary missing./, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
		],
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="name1"

				value1
				-----------------------------69343412719991675451336310646--) . "\r",
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CR just in limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 177
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
	),
	match_log => {
		-error => [ qr/"Multipart: Invalid epilogue after final boundary."/, 1],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
		],
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="name1"

				value1
				-----------------------------69343412719991675451336310646--) . "\r",
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF across limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 177
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
	),
	match_log => {
		-error => [ qr/Multipart parsing error: Multipart: Final boundary missing./, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
		],
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="name1"

				value1
				-----------------------------69343412719991675451336310646--
			),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CR before limit, non-LF after)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 177
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
	),
	match_log => {
		-error => [ qr/Multipart parsing error: Multipart: Final boundary missing./, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
		],
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="name1"

				value1
				-----------------------------69343412719991675451336310646--) . "\rbad epilogue after just CR",
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/empty epilogue just in limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 178
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
	),
	match_log => {
		-error => [ qr/Multipart parsing error: Multipart: Final boundary missing./, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
		],
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="name1"

				value1
				-----------------------------69343412719991675451336310646--
			),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/bad-header in part across limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 114
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS:name1 "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 115 bytes./, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: bad_type

				value
				--000),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/bad-header in part before limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 115
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS:name1 "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 116 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: bad_type

				value
				--0000),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/parital/bad-header in part before limit #2)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 116
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS:name1 "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 117 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: bad_type

				value
				--0000),
		) . "\rX",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/bad-header in part before limit #3)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 117
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS:name1 "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 118 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: bad_type

				value
				--0000
			)
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/bad-header in part before limit #4)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 118
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS:name1 "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 119 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: bad_type

				value
				--0000
			)
		) . q(C) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/bad-header in part before limit #5)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 160
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS:name1 "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 161 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: bad_type

				value
				--0000
			)
		) . q(Content-Disposition: form-data; name="name2) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/bad-header in final part across limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 116
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 117 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: bad_type

				value
				--0000-),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/bad-header in final part before limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 117
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 118 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: bad_type

				value
				--0000--),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/bad-header in final part across limit #2)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 205
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 206 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: text/plain

				value
				--0000
				Content-Disposition: form-data; name="name2"
				Content-Type: bad_type

				value
				--0000-),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/bad-header in final part before limit #2)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 206
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 207 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: text/plain

				value
				--0000
				Content-Disposition: form-data; name="name2"
				Content-Type: bad_type

				value
				--0000--),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/invalid boundary before limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 118
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 119 bytes./, 1],
		error => [ qr/Multipart parsing error: Multipart: Invalid boundary./, 1],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: text/plain

				value
				--0000!)
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/invalid boundary before limit #2)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 119
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 120 bytes./, 1],
		error => [ qr/Multipart parsing error: Multipart: Invalid boundary./, 1],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: text/plain

				value
				--0000)
		) . "\r!" . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/CRLF/partial/invalid final boundary before limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 119
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 120 bytes./, 1],
		error => [ qr/Multipart parsing error: Multipart: Invalid final boundary./, 1],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		normalize_raw_request_data(
			q(
				--0000
				Content-Disposition: form-data; name="name1"; filename="name1.txt"
				Content-Type: text/plain

				value
				--0000-!),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/LF/partial/bad-header in part across limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 109
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS:name1 "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 110 bytes./, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		join("\n",
			q(--0000),
			q(Content-Disposition: form-data; name="name1"; filename="name1.txt"),
			q(Content-Type: bad_type),
			q(),
			q(value),
			q(--000),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/LF/partial/bad-header in part before limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 110
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS:name1 "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 111 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		join("\n",
			q(--0000),
			q(Content-Disposition: form-data; name="name1"; filename="name1.txt"),
			q(Content-Type: bad_type),
			q(),
			q(value),
			q(--0000),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/LF/parital/bad-header in part before limit #2)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 111
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS:name1 "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 112 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		join("\n",
			q(--0000),
			q(Content-Disposition: form-data; name="name1"; filename="name1.txt"),
			q(Content-Type: bad_type),
			q(),
			q(value),
			q(--0000),
		) . "\n" . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/LF/parital/bad-header in part before limit #3)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 112
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS:name1 "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 113 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		join("\n",
			q(--0000),
			q(Content-Disposition: form-data; name="name1"; filename="name1.txt"),
			q(Content-Type: bad_type),
			q(),
			q(value),
			q(--0000),
			q(C),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/LF/parital/bad-header in part before limit #4)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 154
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS:name1 "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 155 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		join("\n",
			q(--0000),
			q(Content-Disposition: form-data; name="name1"; filename="name1.txt"),
			q(Content-Type: bad_type),
			q(),
			q(value),
			q(--0000),
			q(Content-Disposition: form-data; name="name2),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/LF/partial/bad-header in final part across limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 111
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 112 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		join("\n",
			q(--0000),
			q(Content-Disposition: form-data; name="name1"; filename="name1.txt"),
			q(Content-Type: bad_type),
			q(),
			q(value),
			q(--0000-),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/LF/partial/bad-header in final part before limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 112
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 113 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		join("\n",
			q(--0000),
			q(Content-Disposition: form-data; name="name1"; filename="name1.txt"),
			q(Content-Type: bad_type),
			q(),
			q(value),
			q(--0000--),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/LF/partial/bad-header in final part across limit #2)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 195
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 196 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		join("\n",
			q(--0000),
			q(Content-Disposition: form-data; name="name1"; filename="name1.txt"),
			q(Content-Type: text/plain),
			q(),
			q(value),
			q(--0000),
			q(Content-Disposition: form-data; name="name2"),
			q(Content-Type: bad_type),
			q(),
			q(value),
			q(--0000-),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/LF/partial/bad-header in final part before limit #2)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 196
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule MULTIPART_PART_HEADERS "content-type:.*bad_type" "id:'200002',phase:2,t:none,t:lowercase,deny
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 197 bytes./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		join("\n",
			q(--0000),
			q(Content-Disposition: form-data; name="name1"; filename="name1.txt"),
			q(Content-Type: text/plain),
			q(),
			q(value),
			q(--0000),
			q(Content-Disposition: form-data; name="name2"),
			q(Content-Type: bad_type),
			q(),
			q(value),
			q(--0000--),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/LF/partial/invalid boundary before limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 113
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 114 bytes./, 1],
		error => [ qr/Multipart parsing error: Multipart: Invalid boundary./, 1],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		join("\n",
			q(--0000),
			q(Content-Disposition: form-data; name="name1"; filename="name1.txt"),
			q(Content-Type: text/plain),
			q(),
			q(value),
			q(--0000!),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart/LF/partial/invalid final boundary before limit #1)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 114
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
	),
	match_log => {
		debug => [ qr/Input filter: Bucket type HEAP contains 115 bytes./, 1],
		error => [ qr/Multipart parsing error: Multipart: Invalid final boundary./, 1],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=0000",
		],
		join("\n",
			q(--0000),
			q(Content-Disposition: form-data; name="name1"; filename="name1.txt"),
			q(Content-Type: text/plain),
			q(),
			q(value),
			q(--0000-!),
		) . "X",
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (url-encoded/entire/bad_name without value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 15
		SecRequestBodyLimit 16
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Request body no files data length is larger than the configured limit \(15\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		normalize_raw_request_data(
			q(a=1&b=2&bad_name),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (url-encoded/partial/bad_name without value without delimeter before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 8
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		normalize_raw_request_data(
			q(bad_nameX),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (url-encoded/partial/bad_name without value with delimiter before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 9
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		normalize_raw_request_data(
			q(bad_name&X),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (url-encoded/entire/bad_name with value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 10
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		normalize_raw_request_data(
			q(bad_name=1),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (url-encoded/partial/bad_name with value without delimeter before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 10
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		normalize_raw_request_data(
			q(bad_name=1X),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (url-encoded/partial/bad_name with value with delimiter before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 11
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		normalize_raw_request_data(
			q(bad_name=1&X),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (url-encoded/entire/bad_value)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 11
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		normalize_raw_request_data(
			q(a=bad_value),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (url-encoded/partial/bad_value without delimeter before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 11
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		normalize_raw_request_data(
			q(a=bad_valueX),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (url-encoded/partial/bad_value with delimeter before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 12
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		normalize_raw_request_data(
			q(a=bad_value&X),
		),
	),
},{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (json/bad_name after limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 12
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
		],
		normalize_raw_request_data(
			q({"bad_name":1}),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (json/bad_name before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 13
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
		],
		normalize_raw_request_data(
			q({"bad_name":1}),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (json/bad_value after limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 15
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
		],
		normalize_raw_request_data(
			q({"a":"bad_value"}),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (json/bad_value before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 16
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
		],
		normalize_raw_request_data(
			q({"a":"bad_value"}),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (json/ill-formed after limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 17
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
		],
		normalize_raw_request_data(
			q({"a":"bad_value"}]),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (json/ill-formed before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 18
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
		],
		normalize_raw_request_data(
			q({"a":"bad_value"}]),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (xml/bad_value after limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 11
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
		],
		normalize_raw_request_data(
			q(<a>bad_value</a>),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (xml/bad_value before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 12
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
		],
		normalize_raw_request_data(
			q(<a>bad_value</a>),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (xml/ill-formed after limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 19
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
		],
		normalize_raw_request_data(
			q(<a>bad_value</a></b>),
		),
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (xml/ill-formed before limit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyLimit 20
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
		],
		normalize_raw_request_data(
			q(<a>bad_value</a></b>),
		),
	),
},

# SecCookieFormat
{
	type => "config",
	comment => "SecCookieFormat (pos)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 5
		SecCookieFormat 1
		SecRule REQUEST_COOKIES_NAMES "\@streq SESSIONID" "phase:1,deny,chain,id:500231"
		SecRule REQUEST_COOKIES:\$SESSIONID_PATH "\@streq /" "chain"
		SecRule REQUEST_COOKIES:SESSIONID "\@streq cookieval"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\)\. String match "cookieval" at REQUEST_COOKIES:SESSIONID\./, 1 ],
		debug => [ qr(Adding request cookie: name "\$SESSIONID_PATH", value "/"), 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Cookie" => q($Version="1"; SESSIONID="cookieval"; $PATH="/"),
		],
		undef,
	),
},
{
	type => "config",
	comment => "SecCookieFormat (neg)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 5
		SecCookieFormat 0
		SecRule REQUEST_COOKIES_NAMES "\@streq SESSIONID" "phase:1,deny,chain,id:500234"
		SecRule REQUEST_COOKIES:\$SESSIONID_PATH "\@streq /" "chain"
		SecRule REQUEST_COOKIES:SESSIONID "\@streq cookieval"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
		-debug => [ qr(Adding request cookie: name "\$SESSIONID_PATH", value "/"), 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Cookie" => q($Version="1"; SESSIONID="cookieval"; $PATH="/"),
		],
		undef,
	),
},

# SecArgumentsLimit
{
	type => "config",
	comment => "SecArgumentsLimit (pos)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecArgumentsLimit 5
		SecRule REQBODY_ERROR "!\@eq 0" "id:'500232',phase:2,log,deny,status:403,msg:'Failed to parse request body'"
	),
	match_log => {
		error => [ qr/Access denied with code 403 /, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1&b=2&c=3&d=4&e=5&f=6",
	),
},
{
	type => "config",
	comment => "SecArgumentsLimit (neg)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecArgumentsLimit 5
		SecRule REQBODY_ERROR "!\@eq 0" "id:'500233',phase:2,log,deny,status:403,msg:'Failed to parse request body'"
	),
	match_log => {
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=1&b=2&c=3&d=4&e=5",
	),
},

# SecRequestBodyNoFilesLimit
{
	type => "config",
	comment => "SecRequestBodyNoFilesLimit - length is equal to limit",
	conf => q(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyNoFilesLimit 16
	),
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=0123456789ABCD",
	),
},
{
	type => "config",
	comment => "SecRequestBodyNoFilesLimit - length is larger than limit",
	conf => q(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyNoFilesLimit 16
	),
	match_response => {
		status => qr/^413$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"a=0123456789ABCDE",
	),
},
# "long-body" means that we have multiple buckets in input filter brigade
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (url-encoded/long-body/NoFilesLimit<Limit<size/deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8208
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Request body no files data length is larger than the configured limit \(2048\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "8209",
		],
		'a=1&b=' . 'b' x 8192 . '&bad_name&c',
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (url-encoded/long-body/NoFilesLimit<Limit<size/pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8208
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Request body no files data length is larger than the configured limit \(2048\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "8209",
		],
		'a=1&b=' . 'b' x 8193 . '&bad_name&',
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (url-encoded/long-body/NoFilesLimit<Limit=size/deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8208
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Request body no files data length is larger than the configured limit \(2048\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "8208",
		],
		'a=1&b=' . 'b' x 8193 . '&bad_name',
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (url-encoded/long-bodyNoFilesLimit<Limit=size/pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8200
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Request body no files data length is larger than the configured limit \(2048\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
			"Content-Length" => "8200",
		],
		'a=1&b=' . 'b' x 8193 . '&',
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (json/long-body/NoFilesLimit<Limit<size/deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8219
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Request body no files data length is larger than the configured limit \(2048\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "8220",
		],
		'{"a":1,"b":"' . 'b' x 8192 . '","bad_name":1, ',
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (json/long-body/NoFilesLimit<Limit<size/pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8219
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Request body no files data length is larger than the configured limit \(2048\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "8220",
		],
		'{"a":1,"b":"' . 'b' x 8192 . '", "bad_name": 1',
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (json/long-body/NoFilesLimit<Limit=size/bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8219
		SecRule REQUEST_HEADERS:Content-Type "application/json" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Request body no files data length is larger than the configured limit \(2048\)\./, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/json",
			"Content-Length" => "8192",
		],
		'{"a":1,"b":"' . 'b' x 8192 . '","bad_name":1,',
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (xml/long-body/NoFilesLimit<Limit<size/deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8214
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Request body no files data length is larger than the configured limit \(2048\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
			"Content-Length" => "8215",
		],
		'<a><b>' . 'b' x 8192 . '</b><c>bad_value ',
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (xml/long-body/NoFilesLimit<Limit<size/pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8214
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Request body no files data length is larger than the configured limit \(2048\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
			"Content-Length" => "8215",
		],
		'<a><b>' . 'b' x 8192 . '</b><c> bad_value',
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (xml/long-body/NoFilesLimit<Limit=size/bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8214
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200001', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Request body no files data length is larger than the configured limit \(2048\)\./, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/xml",
			"Content-Length" => "8214",
		],
		'<a><b>' . 'b' x 8192 . '</b><c>bad_value',
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (multipart/long-body/NoFilesLimit<Limit<size/deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8472
		SecRule MULTIPART_STRICT_ERROR "!\@eq 0" \\
			"id:'200003',phase:2,t:none,log,deny,status:400, \\
			msg:'Multipart request body failed strict validation: \\
			PE %{REQBODY_PROCESSOR_ERROR}, \\
			BQ %{MULTIPART_BOUNDARY_QUOTED}, \\
			BW %{MULTIPART_BOUNDARY_WHITESPACE}, \\
			DB %{MULTIPART_DATA_BEFORE}, \\
			DA %{MULTIPART_DATA_AFTER}, \\
			HF %{MULTIPART_HEADER_FOLDING}, \\
			LF %{MULTIPART_LF_LINE}, \\
			SM %{MULTIPART_MISSING_SEMICOLON}, \\
			IQ %{MULTIPART_INVALID_QUOTING}, \\
			IP %{MULTIPART_INVALID_PART}, \\
			IH %{MULTIPART_INVALID_HEADER_FOLDING}, \\
			FL %{MULTIPART_FILE_LIMIT_EXCEEDED}'"
		SecRule MULTIPART_UNMATCHED_BOUNDARY "!\@eq 0" \\
			"id:'200004',phase:2,t:none,log,deny,msg:'Multipart parser detected a possible unmatched boundary.'"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200005', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS "bad_value" "id:'200006',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Multipart: Allow partial processing of request body/, 1 ],
		-error => [ qr/Multipart parsing error: Multipart: Final boundary missing./, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: multipart/form-data; boundary=---------------------------69343412719991675451336310646
			Transfer-Encoding: chunked

		),
	)
	.encode_chunked(
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="a"

				1) . "a" x 8192 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				bad_value
				-----------------------------69343412719991675451336310646)
		) . "\r",
		8192
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (multipart/long-body/NoFilesLimit<Limit<size/pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8471
		SecRule MULTIPART_STRICT_ERROR "!\@eq 0" \\
			"id:'200003',phase:2,t:none,log,deny,status:400, \\
			msg:'Multipart request body failed strict validation: \\
			PE %{REQBODY_PROCESSOR_ERROR}, \\
			BQ %{MULTIPART_BOUNDARY_QUOTED}, \\
			BW %{MULTIPART_BOUNDARY_WHITESPACE}, \\
			DB %{MULTIPART_DATA_BEFORE}, \\
			DA %{MULTIPART_DATA_AFTER}, \\
			HF %{MULTIPART_HEADER_FOLDING}, \\
			LF %{MULTIPART_LF_LINE}, \\
			SM %{MULTIPART_MISSING_SEMICOLON}, \\
			IQ %{MULTIPART_INVALID_QUOTING}, \\
			IP %{MULTIPART_INVALID_PART}, \\
			IH %{MULTIPART_INVALID_HEADER_FOLDING}, \\
			FL %{MULTIPART_FILE_LIMIT_EXCEEDED}'"
		SecRule MULTIPART_UNMATCHED_BOUNDARY "!\@eq 0" \\
			"id:'200004',phase:2,t:none,log,deny,msg:'Multipart parser detected a possible unmatched boundary.'"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200005', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS "bad_value" "id:'200006',phase:2,t:none,deny
	),
	match_log => {
		debug => [ qr/Multipart: Allow partial processing of request body/, 1 ],
		-error => [ qr/Multipart parsing error: Multipart: Final boundary missing./, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: multipart/form-data; boundary=---------------------------69343412719991675451336310646
			Transfer-Encoding: chunked

		),
	)
	.encode_chunked(
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="a"

				1) . "a" x 8192 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				bad_value
				-----------------------------69343412719991675451336310646)
		),
		8192
	),
},
{
	type => "config",
	comment => "ProcessPartial NoFilesLimit (multipart/long-body/NoFilesLimit<Limit=size/bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 2048
		SecRequestBodyLimit 8472
		SecRule MULTIPART_STRICT_ERROR "!\@eq 0" \\
			"id:'200003',phase:2,t:none,log,deny,status:400, \\
			msg:'Multipart request body failed strict validation: \\
			PE %{REQBODY_PROCESSOR_ERROR}, \\
			BQ %{MULTIPART_BOUNDARY_QUOTED}, \\
			BW %{MULTIPART_BOUNDARY_WHITESPACE}, \\
			DB %{MULTIPART_DATA_BEFORE}, \\
			DA %{MULTIPART_DATA_AFTER}, \\
			HF %{MULTIPART_HEADER_FOLDING}, \\
			LF %{MULTIPART_LF_LINE}, \\
			SM %{MULTIPART_MISSING_SEMICOLON}, \\
			IQ %{MULTIPART_INVALID_QUOTING}, \\
			IP %{MULTIPART_INVALID_PART}, \\
			IH %{MULTIPART_INVALID_HEADER_FOLDING}, \\
			FL %{MULTIPART_FILE_LIMIT_EXCEEDED}'"
		SecRule MULTIPART_UNMATCHED_BOUNDARY "!\@eq 0" \\
			"id:'200004',phase:2,t:none,log,deny,msg:'Multipart parser detected a possible unmatched boundary.'"
		SecRule REQBODY_ERROR "!\@eq 0" "id:'200005', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2"
		SecRule ARGS "bad_value" "id:'200006',phase:2,t:none,deny
	),
	match_log => {
		-debug => [ qr/Multipart: Allow partial processing of request body/, 1 ],
		error => [ qr/Multipart parsing error: Multipart: Final boundary missing./, 1],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: multipart/form-data; boundary=---------------------------69343412719991675451336310646
			Transfer-Encoding: chunked

		),
	)
	.encode_chunked(
		normalize_raw_request_data(
			q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="a"

				1) . "a" x 8192 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				bad_value
				-----------------------------69343412719991675451336310646)
		),
		8192
	),
},
