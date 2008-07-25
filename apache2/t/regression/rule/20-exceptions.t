### Tests for rule exceptions

# SecRuleRemoveByMsg

# SecRuleRemoveById
{
	type => "config",
	comment => "SecRuleRemoveById (single)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:1"
		SecRuleRemoveById 1
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
		debug => [ qr/Starting phase REQUEST_HEADERS\..*This phase consists of 0 rule.*Starting phase RESPONSE_HEADERS\./s, 1 ],
		-debug => [ qr/Access denied/, 1 ],
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
	comment => "SecRuleRemoveById (multiple)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:1"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:2"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:3"
		SecRuleRemoveById 1 2 3
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
		debug => [ qr/Starting phase REQUEST_HEADERS\..*This phase consists of 0 rule.*Starting phase RESPONSE_HEADERS\./s, 1 ],
		-debug => [ qr/Access denied/, 1 ],
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
	comment => "SecRuleRemoveById (range)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:1"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:2"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:3"
		SecRuleRemoveById 1-3
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
		debug => [ qr/Starting phase REQUEST_HEADERS\..*This phase consists of 0 rule.*Starting phase RESPONSE_HEADERS\./s, 1 ],
		-debug => [ qr/Access denied/, 1 ],
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
	comment => "SecRuleRemoveById (multiple + range)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:1"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:2"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:3"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:4"
		SecRuleRemoveById 1 2-4
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
		debug => [ qr/Starting phase REQUEST_HEADERS\..*This phase consists of 0 rule.*Starting phase RESPONSE_HEADERS\./s, 1 ],
		-debug => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# SecRuleRemoveByMsg
{
	type => "config",
	comment => "SecRuleRemoveByMsg",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:1,msg:'testing rule'"
		SecRuleRemoveByMsg "testing rule"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
		debug => [ qr/Starting phase REQUEST_HEADERS\..*This phase consists of 0 rule.*Starting phase RESPONSE_HEADERS\./s, 1 ],
		-debug => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
