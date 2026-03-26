{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (XML, <=NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		-error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16384",
		],
		'<root><a>' . '1' x 16364 . '</a></root>',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (XML, <=NoFilesLimit, deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		-error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16384",
		],
		'<root><a>' . '1' x 16355 . 'bad_value</a></root>',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (XML, >NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^413$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16365 . '</a></root>',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (XML, >Limit, <=NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^413$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16365 . '</a></root>',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction Reject (XML, >Limit, >NoFilesLimit)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction Reject
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(32768\)\./, 1 ],
	},
	match_response => {
		status => qr/^413$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "32769",
		],
		'<root><a>' . '1' x 32749 . '</a></root>',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, <=NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		-error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16384",
		],
		'<root><a>' . '1' x 16364 . '</a></root>',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, <=NoFilesLimit, deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		-error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16384",
		],
		'<root><a>' . '1' x 16355 . 'bad_value</a></root>',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16365 . '</a></root>',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >NoFilesLimit, deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16366 . 'bad_value ',
	),
},

# The four tests below checks whether the partial text content is added or not.
#
# xmlParseChunk adds a text content whose length is greater than or equal to 300 bytes even when
# you pass 0 as the terminate argument. If the length is less than 300 bytes it does not a text content
# at end of the chunk without a tag being followed.
# https://gitlab.gnome.org/GNOME/libxml2/-/blob/ca6a8cf94672e6f3c48c08d6af2201599788fc17/parser.c#L11061-11079
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >NoFilesLimit, chunk_len>=300, deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16068 . '</a><b>' . '2' x 291 . 'bad_value ',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >NoFilesLimit, chunk_len<300, pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16069 . '</a><b>' . '2' x 290 . 'bad_value ',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >NoFilesLimit, nested, chunk_len>=300, deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16072 . '<b>' . '2' x 291 . 'bad_value ',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >NoFilesLimit, nested, chunk_len<300, pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16073 . '<b>' . '2' x 290 . 'bad_value ',
	),
},

{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >NoFilesLimit, pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16366 . ' bad_value',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >Limit, <=NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16365 . '</a></root>',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >Limit, <=NoFilesLimit, deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16366 . 'bad_value ',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >Limit, <=NoFilesLimit, pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 32768
		SecRequestBodyLimit 16384
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "16385",
		],
		'<root><a>' . '1' x 16366 . ' bad_value',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >Limit, >NoFilesLimit, no bad)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body \(Content-Length\) is larger than the configured limit \(32768\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "32769",
		],
		'<root><a>' . '1' x 32749 . '</a></root>',
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >Limit, >NoFilesLimit, deny)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "32769",
		],
		'<root><a>' . '1' x 16366 . 'bad_value ' . '1' x 16384,
	),
},
{
	type => "config",
	comment => "SecRequestBodyLimitAction ProcessPartial (XML, >Limit, >NoFilesLimit, pass)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRequestBodyLimitAction ProcessPartial
		SecRequestBodyNoFilesLimit 16384
		SecRequestBodyLimit 32768
		SecRule REQUEST_HEADERS:Content-Type "(?:application(?:/soap\\+|/)|text/)xml" "id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML"
		SecRule XML:/* "bad_value" "id:'200002',phase:2,t:none,deny"
	),
	match_log => {
		error => [ qr/Request body no files data length is larger than the configured limit \(16384\)\./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/xml",
			"Content-Length" => "32769",
		],
		'<root><a>' . '1' x 16366 . ' bad_value' . '1' x 16384,
	),
},
