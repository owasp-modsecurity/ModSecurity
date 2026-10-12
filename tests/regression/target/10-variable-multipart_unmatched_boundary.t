### MULTIPART_UNMATCHED_BOUNDARY
#
# 0: no unmatched boundary
# 1: unmatched boundary, not followed by a valid one
# 2: unmatched boundary followed by a valid one (eg. part data starting with "--")
#
# Every case is checked with the 'strict' ("!@eq 0") and the 'permissive'
# ("@eq 1") rule.
#
# Value 1 remains only if the body is broken (no valid boundary follows the
# unmatched line), so in these cases REQBODY_ERROR is also set.

{
	type => "target",
	comment => "MULTIPART_UNMATCHED_BOUNDARY :: valid body, !\@eq 0",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_UNMATCHED_BOUNDARY "!\@eq 0" "id:500401,phase:2,deny,log"
	),
	match_log => {
		debug => [ qr/Target value: "0"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => q(multipart/form-data; boundary=0000),
		],
	    normalize_raw_request_data(
		q(
			--0000
			Content-Disposition: form-data; name="name"

			Brian Rectanus
			--0000
			Content-Disposition: form-data; name="image"; filename="image.jpg"
			Content-Type: image/jpeg

			BINARYDATA
			--0000--
		),
	    ),
	),
},

{
	type => "target",
	comment => "MULTIPART_UNMATCHED_BOUNDARY :: valid body, \@eq 1",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "id:500402,phase:2,deny,log"
	),
	match_log => {
		debug => [ qr/Target value: "0"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => q(multipart/form-data; boundary=0000),
		],
	    normalize_raw_request_data(
		q(
			--0000
			Content-Disposition: form-data; name="name"

			Brian Rectanus
			--0000
			Content-Disposition: form-data; name="image"; filename="image.jpg"
			Content-Type: image/jpeg

			BINARYDATA
			--0000--
		),
	    ),
	),
},

{
	type => "target",
	comment => "MULTIPART_UNMATCHED_BOUNDARY :: unmatched line not followed by a boundary, !\@eq 0",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_UNMATCHED_BOUNDARY "!\@eq 0" "id:500403,phase:2,deny,log"
	),
	match_log => {
		debug => [ qr/Final boundary missing.*Target value: "1".*Rule returned 1\./s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => q(multipart/form-data; boundary=0000),
		],
	    normalize_raw_request_data(
		q(
			--0000
			Content-Disposition: form-data; name="name"

			Brian Rectanus
			--0000
			Content-Disposition: form-data; name="image"; filename="image.jpg"
			Content-Type: image/jpeg

			--BINARYDATA
			more data
		),
	    ),
	),
},

{
	type => "target",
	comment => "MULTIPART_UNMATCHED_BOUNDARY :: unmatched line not followed by a boundary, \@eq 1",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "id:500404,phase:2,deny,log"
	),
	match_log => {
		debug => [ qr/Final boundary missing.*Target value: "1".*Rule returned 1\./s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => q(multipart/form-data; boundary=0000),
		],
	    normalize_raw_request_data(
		q(
			--0000
			Content-Disposition: form-data; name="name"

			Brian Rectanus
			--0000
			Content-Disposition: form-data; name="image"; filename="image.jpg"
			Content-Type: image/jpeg

			--BINARYDATA
			more data
		),
	    ),
	),
},

{
	type => "target",
	comment => "MULTIPART_UNMATCHED_BOUNDARY :: part data starts with '--', !\@eq 0",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_UNMATCHED_BOUNDARY "!\@eq 0" "id:500405,phase:2,deny,log"
	),
	match_log => {
		debug => [ qr/Target value: "2"/s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => q(multipart/form-data; boundary=0000),
		],
	    normalize_raw_request_data(
		q(
			--0000
			Content-Disposition: form-data; name="name"

			--Brian Rectanus
			--0000
			Content-Disposition: form-data; name="image"; filename="image.jpg"
			Content-Type: image/jpeg

			BINARYDATA
			--0000--
		),
	    ),
	),
},

{
	type => "target",
	comment => "MULTIPART_UNMATCHED_BOUNDARY :: part data starts with '--', \@eq 1",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "id:500406,phase:2,deny,log"
	),
	match_log => {
		debug => [ qr/Target value: "2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => q(multipart/form-data; boundary=0000),
		],
	    normalize_raw_request_data(
		q(
			--0000
			Content-Disposition: form-data; name="name"

			--Brian Rectanus
			--0000
			Content-Disposition: form-data; name="image"; filename="image.jpg"
			Content-Type: image/jpeg

			BINARYDATA
			--0000--
		),
	    ),
	),
},

{
	type => "target",
	comment => "MULTIPART_UNMATCHED_BOUNDARY :: '--' in last part, no final CRLF, !\@eq 0",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_UNMATCHED_BOUNDARY "!\@eq 0" "id:500407,phase:2,deny,log"
	),
	match_log => {
		debug => [ qr/Target value: "2"/s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => q(multipart/form-data; boundary=0000),
		],
	    normalize_raw_request_data(
		q(
			--0000
			Content-Disposition: form-data; name="name"

			Brian Rectanus
			--0000
			Content-Disposition: form-data; name="image"; filename="image.jpg"
			Content-Type: image/jpeg

			--BINARYDATA
			--0000--),
	    ),
	),
},

{
	type => "target",
	comment => "MULTIPART_UNMATCHED_BOUNDARY :: '--' in last part, no final CRLF, \@eq 1",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "id:500408,phase:2,deny,log"
	),
	match_log => {
		debug => [ qr/Target value: "2"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => q(multipart/form-data; boundary=0000),
		],
	    normalize_raw_request_data(
		q(
			--0000
			Content-Disposition: form-data; name="name"

			Brian Rectanus
			--0000
			Content-Disposition: form-data; name="image"; filename="image.jpg"
			Content-Type: image/jpeg

			--BINARYDATA
			--0000--),
	    ),
	),
},
