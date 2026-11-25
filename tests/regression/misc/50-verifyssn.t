### @verifySSN area number ranges (#2970)

{
	type => "misc",
	comment => '@verifySSN area 574 (pre-2011 range)',
	conf => q(
		SecRuleEngine On
		SecRule ARGS "@verifySSN \\d{3}-?\\d{2}-?\\d{4}" "id:500970,phase:1,deny,status:403,capture"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\)/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ssn=574-57-8065",
	),
},
{
	type => "misc",
	comment => '@verifySSN area 800 is valid since the 2011 randomisation',
	conf => q(
		SecRuleEngine On
		SecRule ARGS "@verifySSN \\d{3}-?\\d{2}-?\\d{4}" "id:500970,phase:1,deny,status:403,capture"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\)/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ssn=800-57-8065",
	),
},
{
	type => "misc",
	comment => '@verifySSN area 900 is never assigned',
	conf => q(
		SecRuleEngine On
		SecRule ARGS "@verifySSN \\d{3}-?\\d{2}-?\\d{4}" "id:500970,phase:1,deny,status:403,capture"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ssn=900-57-8065",
	),
},
{
	type => "misc",
	comment => '@verifySSN area 666 is never assigned',
	conf => q(
		SecRuleEngine On
		SecRule ARGS "@verifySSN \\d{3}-?\\d{2}-?\\d{4}" "id:500970,phase:1,deny,status:403,capture"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ssn=666-57-8065",
	),
},
