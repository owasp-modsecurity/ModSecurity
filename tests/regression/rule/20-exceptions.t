### Tests for rule exceptions

# SecRuleRemoveById
{
	type => "rule",
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
	type => "rule",
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
	type => "rule",
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
	type => "rule",
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
	type => "rule",
	comment => "SecRuleRemoveByMsg",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:1,msg:'testing rule',id:500001"
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

# SecRuleUpdateActionById
{
	type => "rule",
	comment => "SecRuleUpdateActionById",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:1,msg:'testing rule',id:500002"
		SecRuleUpdateActionById 1 "pass,nolog"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
		debug => [ qr/id:1,.*,pass,nolog/, 1 ],
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
	type => "rule",
	comment => "SecRuleUpdateActionById (chain)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:1,msg:'testing rule',chain,id:500003"
        SecRule ARGS "bar,id:500004"
		SecRuleUpdateActionById 1 "pass,nolog"
	),
	match_log => {
		-error => [ qr/ModSecurity: /, 1 ],
		-audit => [ qr/./, 1 ],
		debug => [ qr/id:1,.*,pass,nolog/, 1 ],
		-debug => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?foo=bar",
	),
},
