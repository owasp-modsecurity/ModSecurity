### SecDebug* directive tests
{
	type => "config",
	comment => "SecDebugLog (pos)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
	),
	match_log => {
		debug => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "config",
	comment => "SecDebugLog (neg)",
	conf => qq(
		SecRuleEngine On
	),
	match_log => {
		-debug => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "config",
	comment => "SecDebugLogLevel 0",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 0
		SecRule REQUEST_URI "." "phase:1,deny,id:500241"
	),
	match_log => {
		-debug => [ qr/./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "config",
	comment => "SecDebugLogLevel 1",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 1
		SecRuleScript "test.lua" "phase:1"
		SecRule REQUEST_URI "(.)" "phase:4,deny,deprecatevar:bogus,id:500242"
	),
	match_log => {
		debug => [ qr/\]\[[1]\] /, 1 ],
		-debug => [ qr/\]\[[2-9]\] /, 1 ],
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
	comment => "SecDebugLogLevel 2",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 2
		SecRuleScript "test.lua" "phase:1"
		SecRule REQUEST_URI "(.)" "phase:4,deny,deprecatevar:bogus,id:500243"
	),
	match_log => {
		debug => [ qr/\]\[2\] /, 1 ],
		-debug => [ qr/\]\[[3-9]\] /, 1 ],
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
	comment => "SecDebugLogLevel 3",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 3
		SecRuleScript "test.lua" "phase:1"
		SecRule REQUEST_URI "(.)" "phase:4,deny,deprecatevar:bogus,id:500244"
	),
	match_log => {
		debug => [ qr/\]\[3\] /, 1 ],
		-debug => [ qr/\]\[[4-9]\] /, 1 ],
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
	comment => "SecDebugLogLevel 4",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 4
		SecRuleScript "test.lua" "phase:1"
		SecRule REQUEST_URI "(.)" "phase:4,deny,deprecatevar:bogus,id:500245"
	),
	match_log => {
		debug => [ qr/\]\[4\] /, 1 ],
		-debug => [ qr/\]\[[5-9]\] /, 1 ],
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
	comment => "SecDebugLogLevel 5",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 5
		SecRuleScript "test.lua" "phase:1"
		SecRule REQUEST_URI "(.)" "phase:4,deny,deprecatevar:bogus,id:500246"
	),
	match_log => {
		debug => [ qr/\]\[5\] /, 1 ],
		-debug => [ qr/\]\[[6-9]\] /, 1 ],
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
	comment => "SecDebugLogLevel 6",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 6
		SecRuleScript "test.lua" "phase:1"
		SecRule REQUEST_URI "(.)" "phase:4,deny,deprecatevar:bogus,id:500247"
	),
	match_log => {
		debug => [ qr/\]\[6\] /, 1 ],
		-debug => [ qr/\]\[[7-9]\] /, 1 ],
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
	comment => "SecDebugLogLevel 7",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 7
		SecRuleScript "test.lua" "phase:1"
		SecRule REQUEST_URI "(.)" "phase:4,deny,deprecatevar:bogus,id:500248"
	),
	match_log => {
		debug => [ qr/\]\[7\] /, 1 ],
		-debug => [ qr/\]\[[8-9]\] /, 1 ],
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
	comment => "SecDebugLogLevel 8",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 8
		SecRuleScript "test.lua" "phase:1"
		SecRule REQUEST_URI "(.)" "phase:4,deny,deprecatevar:bogus,id:500249"
	),
	match_log => {
		debug => [ qr/\]\[8\] /, 1 ],
		-debug => [ qr/\]\[9\] /, 1 ],
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
	comment => "SecDebugLogLevel 9",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRuleScript "test.lua" "phase:1"
		SecRule REQUEST_URI "(.)" "phase:4,deny,deprecatevar:bogus,id:500250"
	),
	match_log => {
		debug => [ qr/\]\[9\] /, 1 ],
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
