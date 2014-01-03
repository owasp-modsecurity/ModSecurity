### Tests for rule exceptions

# SecRuleRemoveById
{
	type => "rule",
	comment => "SecRuleRemoveById (single)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:101010"
		SecRuleRemoveById 101010
	),
	match_log => {
		-error => [ qr/101010/, 1 ],
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
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:101010"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:202020"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:303030"
		SecRuleRemoveById 101010 202020 303030
	),
	match_log => {
		-error => [ qr/101010|202020|303030/, 1 ],
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
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:101010"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:202020"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:303030"
		SecRuleRemoveById 101010-303030
	),
	match_log => {
		-error => [ qr/101010|202020|303030/, 1 ],
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
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:101010"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:202020"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:303030"
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,id:404040"
		SecRuleRemoveById 101010 202020-404040
	),
	match_log => {
		-error => [ qr/101010|202020|303030|404040/, 1 ],
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
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,msg:'testing rule',id:500001"
		SecRuleRemoveByMsg "testing rule"
	),
	match_log => {
		-error => [ qr/500001/, 1 ],
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
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,msg:'testing rule',id:500002"
		SecRuleUpdateActionById 500002 "pass,nolog"
	),
	match_log => {
		-error => [ qr/500002/, 1 ],
		-audit => [ qr/./, 1 ],
		debug => [ qr/id:500002,pass,nolog/, 1 ],
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
		SecRule REQUEST_URI "test" "phase:1,deny,status:500,msg:'testing rule',chain,id:500003"
        SecRule ARGS "bar"
		SecRuleUpdateActionById 500003 "pass,nolog"
	),
	match_log => {
		-error => [ qr/500003/, 1 ],
		-audit => [ qr/./, 1 ],
		debug => [ qr/id:500003,pass,nolog/, 1 ],
		-debug => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?foo=bar",
	),
},
