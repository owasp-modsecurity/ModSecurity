### Tests all of the actions in each phase in detection only mode

# Pass
{
	type => "action",
	comment => "pass in phase:1",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 9
		SecAction "phase:1,pass,msg:'PASSED',id:500057"
		SecAction "phase:1,deny,msg:'DENIED',id:500058"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*PASSED/, 1 ],
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
	comment => "pass in phase:2",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:2,pass,msg:'PASSED',id:500059"
		SecAction "phase:2,deny,msg:'DENIED',id:500060"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*PASSED/, 1 ],
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
	comment => "pass in phase:3",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecAction "phase:3,pass,msg:'PASSED',id:500061"
		SecAction "phase:3,deny,msg:'DENIED',id:500062"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*PASSED/, 1 ],
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
	comment => "pass in phase:4",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecAction "phase:4,pass,msg:'PASSED',id:500063"
		SecAction "phase:4,deny,msg:'DENIED',id:500064"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*PASSED/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# Allow
{
	type => "action",
	comment => "allow in phase:1",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:1,allow,msg:'ALLOWED',id:500065"
		SecAction "phase:1,deny,msg:'DENIED',id:500066"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*ALLOWED/, 1 ],
		-error => [ qr/Access allowed/, 1 ],
# TODO: Allow should probably stop rule execution
#		-error => [ qr/DENIED/, 1 ],
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
	comment => "allow in phase:2",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:2,allow,msg:'ALLOWED',id:500067"
		SecAction "phase:2,deny,msg:'DENIED',id:500068"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*ALLOWED/, 1 ],
		-error => [ qr/Access allowed/, 1 ],
# TODO: Allow should probably stop rule execution
#		-error => [ qr/DENIED/, 1 ],
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
	comment => "allow in phase:3",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:3,allow,msg:'ALLOWED',id:500069"
		SecAction "phase:3,deny,msg:'DENIED',id:500070"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*ALLOWED/, 1 ],
		-error => [ qr/Access allowed/, 1 ],
# TODO: Allow should probably stop rule execution
#		-error => [ qr/DENIED/, 1 ],
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
	comment => "allow in phase:4",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:4,allow,msg:'ALLOWED',id:500071"
		SecAction "phase:4,deny,msg:'DENIED',id:500072"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*ALLOWED/, 1 ],
		-error => [ qr/Access allowed/, 1 ],
# TODO: Allow should probably stop rule execution
#		-error => [ qr/DENIED/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# Deny
{
	type => "action",
	comment => "deny in phase:1",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:1,deny,msg:'DENIED',id:500073"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*DENIED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
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
	comment => "deny in phase:2",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:2,deny,msg:'DENIED',id:500074"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*DENIED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
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
	comment => "deny in phase:3",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:3,deny,msg:'DENIED',id:500075"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*DENIED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
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
	comment => "deny in phase:4",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:4,deny,msg:'DENIED',id:500076"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*DENIED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# Drop
{
	type => "action",
	comment => "drop in phase:1",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:1,drop,msg:'DROPPED',id:500077"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*DROPPED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
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
	comment => "drop in phase:2",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:2,drop,msg:'DROPPED',id:500078"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*DROPPED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
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
	comment => "drop in phase:3",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:3,drop,msg:'DROPPED',id:500079"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*DROPPED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
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
	comment => "drop in phase:4",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:4,drop,msg:'DROPPED',id:500080"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction.*DROPPED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# Redirect
{
	type => "action",
	comment => "redirect in phase:1 (get)",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:1,redirect:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',msg:'REDIRECTED',id:500009"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. String match "\/test2.txt" at REQUEST_URI.*REDIRECTED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST 2$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "redirect in phase:2 (get)",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:2,redirect:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',msg:'REDIRECTED',id:500010"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. String match "\/test2.txt" at REQUEST_URI.*REDIRECTED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST 2$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "redirect in phase:3 (get)",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:3,redirect:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',msg:'REDIRECTED',id:500011"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. String match "\/test2.txt" at REQUEST_URI.*REDIRECTED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST 2$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "redirect in phase:4 (get)",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:4,redirect:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',msg:'REDIRECTED',id:500012"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. String match "\/test2.txt" at REQUEST_URI.*REDIRECTED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST 2$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},

# Proxy
{
	type => "action",
	comment => "proxy in phase:1 (get)",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:1,proxy:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',msg:'PROXIED',id:500013"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. String match "\/test2.txt" at REQUEST_URI.*PROXIED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST 2$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "proxy in phase:2 (get)",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:2,proxy:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',msg:'PROXIED',id:500014"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. String match "\/test2.txt" at REQUEST_URI.*PROXIED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST 2$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "proxy in phase:3 (get)",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:3,proxy:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',msg:'PROXIED',id:500015"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. String match "\/test2.txt" at REQUEST_URI.*PROXIED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "proxy in phase:4 (get)",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:4,proxy:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',msg:'PROXIED',id:500016"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. String match "\/test2.txt" at REQUEST_URI.*PROXIED/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
