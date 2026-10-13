### Content-Disposition error codes are named in the error message (#3655)

{
	type => "misc",
	comment => "multipart duplicate filename reports -20 with its description",
	conf => q(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRule REQBODY_ERROR "!@eq 0" "id:500655,phase:2,deny,status:403,msg:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		error => [ qr/Invalid Content-Disposition header \(-20: duplicate filename parameter\)/, 1 ],
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
				Content-Disposition: form-data; name="file"; filename="safe.jpg"; filename="safe.php"
				Content-Type: text/plain

				hello
				--0000--
			),
		),
	),
},
{
	type => "misc",
	comment => "multipart duplicate filename* reports -21 with its description",
	conf => q(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRule REQBODY_ERROR "!@eq 0" "id:500656,phase:2,deny,status:403,msg:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		error => [ qr/Invalid Content-Disposition header \(-21: duplicate filename\* parameter\)/, 1 ],
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
				Content-Disposition: form-data; name="file"; filename*=UTF8''safe.jpg; filename*=UTF8''safe.php
				Content-Type: text/plain

				hello
				--0000--
			),
		),
	),
},
