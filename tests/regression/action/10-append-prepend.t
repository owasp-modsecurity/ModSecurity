# TODO: Need more tests here

### append
{
	type => "action",
	comment => "append content",
	conf => qq(
		SecRuleEngine On
        SecContentInjection On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
		SecAction "phase:1,setvar:tx.test=test,id:500002"
		SecAction "phase:2,append:'APPEND: \%{tx.test}',id:500003"
	),
	match_log => {
        debug => [ "Added content to bottom: APPEND: test", 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/APPEND: test$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

### prepend
{
	type => "action",
	comment => "prepend content",
	conf => qq(
		SecRuleEngine On
        SecContentInjection On
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
		SecAction "phase:1,setvar:tx.test=test,id:500004"
		SecAction "phase:2,prepend:'PREPEND: \%{tx.test}',id:500005"
	),
	match_log => {
        debug => [ "Added content to top: PREPEND: test", 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^PREPEND: test/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
