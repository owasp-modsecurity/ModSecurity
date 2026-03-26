{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (multipart, chunked, <=NoFilesLimit, no bad)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction Reject
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Multipart flags: \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		error => [ qr/Multipart flags: PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0./, 1],
		debug => [ qr/Request body no files length: 16384/, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 4096 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . "b" x 16278 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (multipart, chunked, <=NoFilesLimit, deny bad name)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction Reject
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Multipart flags: \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		error => [ qr/Multipart flags: PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0./, 1],
		debug => [ qr/Request body no files length: 16384/, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 4096 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="bad_name"

				) . "b" x 16271 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (multipart, chunked, <=NoFilesLimit, deny bad value)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction Reject
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Multipart flags: \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		error => [ qr/Multipart flags: PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0./, 1],
		debug => [ qr/Request body no files length: 16384/, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 4096 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . 'b' x 16269 . q(bad_value
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (multipart, chunked, >NoFilesLimit)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction Reject
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Multipart flags: \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		-error => [ qr/Multipart flags:/, 1],
		debug => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 4096 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . "b" x 16279 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (multipart, chunked, >Limit)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction Reject
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Multipart flags: \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		-error => [ qr/Multipart flags:/, 1],
		debug => [ qr/Request body is larger than the configured limit \(32768\)\./, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 16199 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . "b" x 16278 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart, chunked, =NoFilesLimit, no bad)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		),
		4096
	),
	match_log => {
		error => [ qr/heck values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Request body no files length: 16384/, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 4096 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . "b" x 16278 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart, chunked, =NoFilesLimit, deny bad name)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		),
		4096
	),
	match_log => {
		error => [ qr/heck values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Request body no files length: 16384/, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 4096 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="bad_name"

				) . "b" x 16271 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart, chunked, =NoFilesLimit, deny bad value)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		),
		4096
	),
	match_log => {
		error => [ qr/heck values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Request body no files length: 16384/, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 4096 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . 'b' x 16269 . q(bad_value
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart, chunked, >NoFilesLimit, no bad)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		),
		4096
	),
	match_log => {
		error => [ qr/heck values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 4096 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . "b" x 16279 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart, chunked, >NoFilesLimit, deny bad name)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		),
		4096
	),
	match_log => {
		error => [ qr/heck values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 4096 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="bad_name"

				) . "b" x 16272 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart, chunked, >NoFilesLimit, deny bad value)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		),
		4096
	),
	match_log => {
		error => [ qr/heck values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 4096 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . 'b' x 16270 . q(bad_value
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart, chunked, =Limit, no bad)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		error => [ qr/Check values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Input filter: Completed receiving request body \(length 32768\)\./, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 16199 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . "b" x 16278 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart, chunked, =Limit, deny bad name)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		error => [ qr/Check values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Input filter: Completed receiving request body \(length 32768\)\./, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 16199 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="bad_name"

				) . "b" x 16271 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart, chunked, =Limit, deny bad value)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		error => [ qr/Check values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Input filter: Completed receiving request body \(length 32768\)\./, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 16199 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . "b" x 16270 . q(ad_value
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart, chunked, >Limit, no bad)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		error => [ qr/Check values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Multipart: Allow partial processing of request body/, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 16199 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . "b" x 16279 . q(
				-----------------------------69343412719991675451336310646--
			)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart no CRLF after final boundary, chunked, >Limit, deny bad name)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		error => [ qr/Check values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Multipart: Allow partial processing of request body/, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 16199 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="bad_name"

				) . "b" x 16274 . q(
				-----------------------------69343412719991675451336310646--)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart no CRLF after final boundary, chunked, >Limit, pass bad name)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		error => [ qr/Check values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Multipart: Allow partial processing of request body/, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 16199 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="bad_name"

				) . "b" x 16275 . q(
				-----------------------------69343412719991675451336310646--)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart no CRLF after final boundary, chunked, >Limit, deny bad value)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		error => [ qr/Check values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Multipart: Allow partial processing of request body/, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 16199 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . "b" x 16273 . q(ad_value
				-----------------------------69343412719991675451336310646--)
		),
		4096
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (multipart no CRLF after final boundary, chunked, >Limit, pass bad value)",
	conf => trim_action_indent(
		qq(
			SecRuleEngine On
			SecDebugLog $ENV{DEBUG_LOG}
			SecDebugLogLevel 9
			SecRequestBodyAccess On
			SecRequestBodyLimitAction ProcessPartial
			SecRequestBodyNoFilesLimit 16384
			SecRequestBodyLimit 32768
			SecRule REQBODY_PROCESSOR "^MULTIPART\$" \\
				"id:'200000',phase:2,t:none,log, \\
				msg:'Check values for test: \\
				RE %{REQBODY_ERROR}, \\
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
				FL %{MULTIPART_FILE_LIMIT_EXCEEDED}, \\
				UB %{MULTIPART_UNMATCHED_BOUNDARY}'"
			SecRule ARGS_NAMES "bad_name" "id:'200002',phase:2,t:none,deny"
			SecRule ARGS "bad_value" "id:'200003',phase:2,t:none,deny"
		)
	),
	match_log => {
		error => [ qr/Check values for test: RE 0, PE 0, BQ 0, BW 0, DB 0, DA 0, HF 0, LF 0, SM 0, IQ 0, IP 0, IH 0, FL 0, UB 0/, 1],
		debug => [ qr/Multipart: Allow partial processing of request body/, 1],
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
				Content-Disposition: form-data; name="a"; filename="a.txt"

				) . "a" x 16199 . q(
				-----------------------------69343412719991675451336310646
				Content-Disposition: form-data; name="b"

				) . "b" x 16274 . q(ad_value
				-----------------------------69343412719991675451336310646--)
		),
		4096
	),
},
