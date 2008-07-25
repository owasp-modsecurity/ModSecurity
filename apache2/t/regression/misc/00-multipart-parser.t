### Multipart parser tests

# Final CRLF or not, we should still work
{
	type => "misc",
	comment => "multipart parser (final CRLF)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
		SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
		SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
	),
	match_log => {
		debug => [ qr/Adding request argument \(BODY\): name "a", value "1".*Adding request argument \(BODY\): name "b", value "2"/s, 1 ],
		-debug => [ qr/Multipart error:/, 1 ],

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
				Content-Disposition: form-data; name="a"

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				2
				-----------------------------69343412719991675451336310646--
			),
		),
	),
},
{
	type => "misc",
	comment => "multipart parser (no final CRLF)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
		SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
		SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
	),
	match_log => {
		debug => [ qr/Adding request argument \(BODY\): name "a", value "1".*Adding request argument \(BODY\): name "b", value "2"/s, 1 ],
		-debug => [ qr/Multipart error:/, 1 ],

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
				Content-Disposition: form-data; name="a"

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				2
				-----------------------------69343412719991675451336310646--),
		),
	),
},

# Should work with a boundary of "boundary"
{
	type => "misc",
	comment => "multipart parser (boundary contains \"boundary\")",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
		SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
		SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
	),
	match_log => {
		debug => [ qr/Adding request argument \(BODY\): name "a", value "1".*Adding request argument \(BODY\): name "b", value "2"/s, 1 ],
		-debug => [ qr/Multipart error:/, 1 ],

	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=------------------------------------------------boundary",
		],
		normalize_raw_request_data(
			q(
				--------------------------------------------------boundary
				Content-Disposition: form-data; name="a"

				1
				--------------------------------------------------boundary
				Content-Disposition: form-data; name="b"

				2
				--------------------------------------------------boundary--
			),
		),
	),
},
{
	type => "misc",
	comment => "multipart parser (boundary contains \"bOuNdArY\")",
	note => q(
		KHTML Boundary
	),
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
		SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
		SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
	),
	match_log => {
		debug => [ qr/Adding request argument \(BODY\): name "a", value "1".*Adding request argument \(BODY\): name "b", value "2"/s, 1 ],
		-debug => [ qr/Multipart error:/, 1 ],

	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=--------0xKhTmLbOuNdArY",
		],
		normalize_raw_request_data(
			q(
				----------0xKhTmLbOuNdArY
				Content-Disposition: form-data; name="a"

				1
				----------0xKhTmLbOuNdArY
				Content-Disposition: form-data; name="b"

				2
				----------0xKhTmLbOuNdArY--
			),
		),
	),
},

# We should handle data starting with a "--"
{
	type => "misc",
	comment => "multipart parser (data contains \"--\")",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
		SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
	),
	match_log => {
		debug => [ qr/Adding request argument \(BODY\): name "a", value "--test".*Adding request argument \(BODY\): name "b", value "--"/s, 1 ],
		-debug => [ qr/Multipart error:/, 1 ],

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
				Content-Disposition: form-data; name="a"

				--test
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				--
				-----------------------------69343412719991675451336310646--),
		),
	),
},

# We should emit warnings for parsing errors
{
	type => "misc",
	comment => "multipart parser error (no final boundary)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAuditEngine RelevantOnly
	),
	match_log => {
		audit => [ qr/Final boundary missing/, 1 ],
		debug => [ qr/Final boundary missing/, 1 ],

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
				Content-Disposition: form-data; name="a"

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				2
			),
		),
	),
},
{
	type => "misc",
	comment => "multipart parser error (no disposition)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAuditEngine RelevantOnly
	),
	match_log => {
		-debug => [ qr/Multipart error:/, 1 ],
		audit => [ qr/Part missing Content-Disposition header/, 1 ],
		debug => [ qr/Part missing Content-Disposition header/, 1 ],

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

				1
				-----------------------------69343412719991675451336310646

				2
				-----------------------------69343412719991675451336310646--
			),
		),
	),
},
{
	type => "misc",
	comment => "multipart parser error (bad disposition)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAuditEngine RelevantOnly
	),
	match_log => {
		audit => [ qr/Invalid Content-Disposition header/, 1 ],
		debug => [ qr/Invalid Content-Disposition header/, 1 ],

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
				Content-Disposition: form-data name="a"

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data name="b"

				2
				-----------------------------69343412719991675451336310646--
			),
		),
	),
},
{
	type => "misc",
	comment => "multipart parser error (no disposition name)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAuditEngine RelevantOnly
	),
	match_log => {
		-debug => [ qr/Multipart error:/, 1 ],
		audit => [ qr/Content-Disposition header missing name field/, 1 ],
		debug => [ qr/Content-Disposition header missing name field/, 1 ],

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
				Content-Disposition: form-data;

				1
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data;

				2
				-----------------------------69343412719991675451336310646--
			),
		),
	),
},
