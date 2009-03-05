# PDF XSS Protection

{
	type => "misc",
	comment => "pdf-xss - GET",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		SecPdfProtect On
		SecPdfProtectMethod TokenRedirection
		SecPdfProtectSecret FooBar
		SecPdfProtectTimeout 10
	),
	match_log => {
		debug => [ qr/PdfProtect: PDF request without a token - redirecting to/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.pdf",
	),
},
{
	type => "misc",
	comment => "pdf-xss - POST",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		SecPdfProtect On
		SecPdfProtectMethod TokenRedirection
		SecPdfProtectSecret FooBar
		SecPdfProtectTimeout 10
	),
	match_log => {
		-error => [ qr/exit signal/, 1 ],
		debug => [ qr/PdfProtect: Not intercepting.*method=POST\/2/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.pdf",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		#  Args
		"a=1&b=2",
	),
},
