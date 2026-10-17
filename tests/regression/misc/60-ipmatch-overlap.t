### @ipMatch with overlapping prefixes on the same key (#3588)

{
	type => "misc",
	comment => '@ipMatch narrow then wide on the same key, IP in the wide range',
	conf => q(
		SecRuleEngine On
		SecRule ARGS:ip "@ipMatch 10.0.0.0/16,10.0.0.0/8" "id:500588,phase:1,deny,status:403"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\)/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ip=10.5.0.1",
	),
},
{
	type => "misc",
	comment => '@ipMatch narrow then wide on the same key, IP in the narrow range',
	conf => q(
		SecRuleEngine On
		SecRule ARGS:ip "@ipMatch 10.0.0.0/16,10.0.0.0/8" "id:500588,phase:1,deny,status:403"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\)/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ip=10.0.5.1",
	),
},
{
	type => "misc",
	comment => '@ipMatch wide then narrow on the same key',
	conf => q(
		SecRuleEngine On
		SecRule ARGS:ip "@ipMatch 10.0.0.0/8,10.0.0.0/16" "id:500588,phase:1,deny,status:403"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\)/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ip=10.5.0.1",
	),
},
{
	type => "misc",
	comment => '@ipMatch host then network on the same key',
	conf => q(
		SecRuleEngine On
		SecRule ARGS:ip "@ipMatch 172.16.0.0,172.16.0.0/12" "id:500588,phase:1,deny,status:403"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\)/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ip=172.20.0.1",
	),
},
{
	type => "misc",
	comment => '@ipMatch duplicate prefixes still match',
	conf => q(
		SecRuleEngine On
		SecRule ARGS:ip "@ipMatch 10.0.0.0/8,127.0.0.1,10.0.0.0/8" "id:500588,phase:1,deny,status:403"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\)/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ip=10.5.0.1",
	),
},
{
	type => "misc",
	comment => '@ipMatch address outside every prefix',
	conf => q(
		SecRuleEngine On
		SecRule ARGS:ip "@ipMatch 10.0.0.0/16,10.0.0.0/8,192.168.0.0/16" "id:500588,phase:1,deny,status:403"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ip=11.0.0.1",
	),
},
{
	type => "misc",
	comment => '@ipMatch IPv6 narrow then wide on the same key',
	conf => q(
		SecRuleEngine On
		SecRule ARGS:ip "@ipMatch 2001:db8::/48,2001:db8::/32" "id:500588,phase:1,deny,status:403"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\)/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ip=2001:db8:1::1",
	),
},
{
	type => "misc",
	comment => '@ipMatch IPv6 address outside every prefix',
	conf => q(
		SecRuleEngine On
		SecRule ARGS:ip "@ipMatch 2001:db8::/48,2001:db8::/32" "id:500588,phase:1,deny,status:403"
	),
	match_log => {
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?ip=2001:db9::1",
	),
},
