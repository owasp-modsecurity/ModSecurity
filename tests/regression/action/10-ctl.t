### ctl

### ruleRemoveById
{
	type => "action",
	comment => "ruleRemoveById existing rule across phases",
	conf => qq(
		SecRuleEngine On
		SecAction "phase:2,id:666,deny"
		SecAction "phase:1,pass,ctl:ruleRemoveById=666,id:500030"
	),
	match_log => {
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "ruleRemoveById future rule across phases",
	conf => qq(
		SecRuleEngine On
		SecAction "phase:1,pass,ctl:ruleRemoveById=666,id:500031"
		SecAction "phase:2,id:666,deny"
	),
	match_log => {
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "ruleRemoveById future rule same phase",
	conf => qq(
		SecRuleEngine On
		SecAction "phase:1,pass,ctl:ruleRemoveById=666,id:500032"
		SecAction "phase:1,id:666,deny"
	),
	match_log => {
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},


