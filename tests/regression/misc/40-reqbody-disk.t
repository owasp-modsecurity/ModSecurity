### Request body moved to disk and then processed as raw/urlencoded (#3300)

# The body exceeds SecRequestBodyInMemoryLimit, so it is written to a
# temporary file and the in-memory chunks are released. A processor that
# needs the raw buffer (URLENCODED forced through ctl) used to dereference
# the released chunk array and crash the worker.
{
	type => "misc",
	comment => "reqbody on disk, forced URLENCODED processor",
	conf => q(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyInMemoryLimit 64
		SecRule REQUEST_HEADERS:Content-Type "text/plain" "id:500300,phase:1,pass,nolog,ctl:requestBodyProcessor=URLENCODED"
		SecRule ARGS:a "@streq 1" "id:500301,phase:2,deny,status:403"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 2\)/, 1 ],
		-error => [ qr/Segmentation fault|buffer overflow/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/plain",
		],
		"a=1&pad=" . ("x" x 200) . "&b=2",
	),
},
{
	type => "misc",
	comment => "reqbody on disk, forced URLENCODED processor, REQUEST_BODY populated",
	conf => q(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRequestBodyInMemoryLimit 64
		SecRule REQUEST_HEADERS:Content-Type "text/plain" "id:500302,phase:1,pass,nolog,ctl:requestBodyProcessor=URLENCODED"
		SecRule REQUEST_BODY "@contains &b=2" "id:500303,phase:2,deny,status:403"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 2\)/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "text/plain",
		],
		"a=1&pad=" . ("x" x 200) . "&b=2",
	),
},
