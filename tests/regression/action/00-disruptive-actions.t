### Tests all of the actions in each phase

# Pass
{
	type => "action",
	comment => "pass in phase:1",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:1,pass,id:500033"
		SecAction "phase:1,deny,id:500034"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "pass in phase:2",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:2,pass,id:500035"
		SecAction "phase:2,deny,id:500036"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "pass in phase:3",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecAction "phase:3,pass,id:500037"
		SecAction "phase:3,deny,id:500038"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "pass in phase:4",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecAction "phase:4,pass,id:500039"
		SecAction "phase:4,deny,id:500040"
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
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
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:1,allow,id:500041"
		SecAction "phase:1,deny,id:500042"
	),
	match_log => {
		error => [ qr/ModSecurity: Access allowed \(phase 1\). Unconditional match in SecAction/, 1 ],
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
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:2,allow,id:500043"
		SecAction "phase:2,deny,id:500044"
	),
	match_log => {
		error => [ qr/ModSecurity: Access allowed \(phase 2\). Unconditional match in SecAction/, 1 ],
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
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecAction "phase:3,allow,id:500045"
		SecAction "phase:3,deny,id:500046"
	),
	match_log => {
		error => [ qr/ModSecurity: Access allowed \(phase 3\). Unconditional match in SecAction/, 1 ],
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
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecAction "phase:4,allow,id:500047"
		SecAction "phase:4,deny,id:500048"
	),
	match_log => {
		error => [ qr/ModSecurity: Access allowed \(phase 4\). Unconditional match in SecAction/, 1 ],
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
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:1,deny,id:500049"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 1\). Unconditional match in SecAction./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "deny in phase:2",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:2,deny,id:500050"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 2\). Unconditional match in SecAction./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "deny in phase:3",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecAction "phase:3,deny,id:500051"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 3\). Unconditional match in SecAction./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "deny in phase:4",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecAction "phase:4,deny,id:500052"
	),
	match_log => {
		error => [ qr/Access denied with code 403 \(phase 4\). Unconditional match in SecAction./, 1 ],
	},
	match_response => {
		status => qr/^403$/,
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
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:1,drop,id:500053"
	),
	match_log => {
		error => [ qr/Access denied with connection close \(phase 1\). Unconditional match in SecAction./, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "drop in phase:2",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecAction "phase:2,drop,id:500054"
	),
	match_log => {
		error => [ qr/Access denied with connection close \(phase 2\). Unconditional match in SecAction./, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "drop in phase:3",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecAction "phase:3,drop,id:500055"
	),
	match_log => {
		error => [ qr/Access denied with connection close \(phase 3\). Unconditional match in SecAction./, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "action",
	comment => "drop in phase:4",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecAction "phase:4,drop,id:500056"
	),
	match_log => {
		error => [ qr/Access denied with connection close \(phase 4\). Unconditional match in SecAction./, 1 ],
	},
	match_response => {
		status => qr/^500$/,
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
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:1,redirect:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',id:500001"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with redirection to .* using status 302 \(phase 1\)/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "redirect in phase:2 (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:2,redirect:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',id:500002"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with redirection to .* using status 302 \(phase 2\)/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "redirect in phase:3 (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:3,redirect:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',id:500003"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with redirection to .* using status 302 \(phase 3\)/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "redirect in phase:4 (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:4,redirect:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',id:500004"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with redirection to .* using status 302 \(phase 4\)/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST$/,
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
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:1,proxy:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',id:500005"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied using proxy to \(phase 1\)/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "proxy in phase:2 (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:2,proxy:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',id:500006"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied using proxy to \(phase 2\)/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		content => qr/^TEST$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "proxy in phase:3 (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:3,proxy:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',id:500007"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 500 \(phase 3\) \(Configuration Error: Proxy action requested but it does not work in output phases\)./, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
{
	type => "action",
	comment => "proxy in phase:4 (get)",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecResponseBodyAccess On
		SecResponseBodyMimeType null
		SecDebugLog "$ENV{DEBUG_LOG}"
		SecDebugLogLevel 4
		SecRule REQUEST_URI "\@streq /test2.txt" "phase:4,proxy:'http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt',id:500008"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 500 \(phase 4\) \(Configuration Error: Proxy action requested but it does not work in output phases\)./, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test2.txt",
	),
},
