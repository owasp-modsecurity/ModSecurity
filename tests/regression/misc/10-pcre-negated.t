### Regex errors with negated @rx and @validateHash

# Neither the operator nor its negation must match when the regex fails.
# A failure must be visible through the TX:MSC_* flags instead.

{
	type => "misc",
	comment => "PCRE limits exceeded - negated rx does not match",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		SecRequestBodyAccess On
		SecRequestBodyLimit 1000000
		SecRequestBodyInMemoryLimit 1000000

		SecPcreMatchLimit 100
		SecPcreMatchLimitRecursion 100

		SecRule ARGS "!\@rx (?:(.{2,})\\1{32,})" "phase:2,deny,id:500060"
		SecRule TX:MSC_PCRE_LIMITS_EXCEEDED "\@streq 1" "phase:2,pass,log,msg:'PCRE limits flagged',id:500061"
	),
	match_log => {
		debug => [ qr/PCRE limits exceeded/, 1 ],
		error => [ qr/PCRE limits flagged/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"test=" . (("a" x 64) . "b") x 64 . "d",
	),
},
{
	type => "misc",
	comment => "PCRE limits exceeded - negated rx still checks the remaining targets",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		SecRequestBodyAccess On
		SecRequestBodyLimit 1000000
		SecRequestBodyInMemoryLimit 1000000

		SecPcreMatchLimit 100
		SecPcreMatchLimitRecursion 100

		SecRule ARGS "!\@rx (?:(.{2,})\\1{32,})" "phase:2,deny,log,id:500062"
	),
	match_log => {
		debug => [ qr/PCRE limits exceeded/, 1 ],
		error => [ qr/against "ARGS:other" required/, 1 ],
		-error => [ qr/against "ARGS:test" required/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"test=" . (("a" x 64) . "b") x 64 . "d" . "&other=x",
	),
},
{
	type => "misc",
	comment => "PCRE limits exceeded - rx does not match",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		SecRequestBodyAccess On
		SecRequestBodyLimit 1000000
		SecRequestBodyInMemoryLimit 1000000

		SecPcreMatchLimit 100
		SecPcreMatchLimitRecursion 100

		SecRule ARGS "\@rx (?:(.{2,})\\1{32,})" "phase:2,deny,id:500063"
	),
	match_log => {
		debug => [ qr/PCRE limits exceeded/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"test=" . (("a" x 64) . "b") x 64 . "d",
	),
},
{
	type => "misc",
	comment => "Invalid expanded pattern - negated rx does not match, MSC_PCRE_ERROR is set",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		SecRule ARGS:x "!\@rx %{ARGS.patt}" "phase:2,deny,id:500064"
		SecRule TX:MSC_PCRE_ERROR "\@streq 1" "phase:2,pass,log,msg:'PCRE error flagged',id:500065"
	),
	match_log => {
		debug => [ qr/Error compiling pattern/, 1 ],
		error => [ qr/PCRE error flagged/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?x=a&patt=x%28",
	),
},
{
	type => "misc",
	comment => "Invalid expanded pattern - TX:/^MSC_/ rule blocks the request",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		SecRule ARGS:x "!\@rx %{ARGS.patt}" "phase:2,deny,id:500066"
		SecRule TX:/^MSC_/ "!\@streq 0" "phase:2,t:none,log,deny,msg:'ModSecurity internal error flagged: %{MATCHED_VAR_NAME}',id:500067"
	),
	match_log => {
		error => [ qr/internal error flagged: TX:MSC_PCRE_ERROR/, 1 ],
		-error => [ qr/against "ARGS:x" required/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?x=a&patt=x%28",
	),
},
{
	type => "misc",
	comment => "Invalid expanded pattern - negated validateHash does not match, MSC_PCRE_ERROR is set",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecHashEngine On

		SecRule ARGS:x "!\@validateHash %{ARGS.patt}" "phase:2,deny,id:500068"
		SecRule TX:MSC_PCRE_ERROR "\@streq 1" "phase:2,pass,log,msg:'PCRE error flagged',id:500069"
	),
	match_log => {
		debug => [ qr/Error compiling pattern/, 1 ],
		error => [ qr/PCRE error flagged/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?x=a&patt=%2A",
	),
},
{
	type => "misc",
	comment => "PCRE limits exceeded - negated validateHash does not match",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecHashEngine On

		SecRequestBodyAccess On
		SecRequestBodyLimit 1000000
		SecRequestBodyInMemoryLimit 1000000

		SecPcreMatchLimit 100
		SecPcreMatchLimitRecursion 100

		SecRule ARGS "!\@validateHash (?:(.{2,})\\1{32,})" "phase:2,deny,id:500070"
		SecRule TX:MSC_PCRE_LIMITS_EXCEEDED "\@streq 1" "phase:2,pass,log,msg:'PCRE limits flagged',id:500071"
	),
	match_log => {
		debug => [ qr/PCRE limits exceeded/, 1 ],
		error => [ qr/PCRE limits flagged/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"test=" . (("a" x 64) . "b") x 64 . "d",
	),
},
{
	type => "misc",
	comment => "PCRE limits exceeded - MSC_PCRE_ERROR is set too",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		SecRequestBodyAccess On
		SecRequestBodyLimit 1000000
		SecRequestBodyInMemoryLimit 1000000

		SecPcreMatchLimit 100
		SecPcreMatchLimitRecursion 100

		SecRule ARGS "!\@rx (?:(.{2,})\\1{32,})" "phase:2,deny,id:500072"
		SecRule TX:MSC_PCRE_ERROR "\@streq 1" "phase:2,pass,log,msg:'PCRE error flagged',id:500073"
	),
	match_log => {
		debug => [ qr/PCRE limits exceeded/, 1 ],
		error => [ qr/PCRE error flagged/, 1 ],
		-error => [ qr/Access denied/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"test=" . (("a" x 64) . "b") x 64 . "d",
	),
},
{
	type => "misc",
	comment => "PCRE limits exceeded - TX:/^MSC_/ rule names MSC_PCRE_LIMITS_EXCEEDED",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		SecRequestBodyAccess On
		SecRequestBodyLimit 1000000
		SecRequestBodyInMemoryLimit 1000000

		SecPcreMatchLimit 100
		SecPcreMatchLimitRecursion 100

		SecRule ARGS "!\@rx (?:(.{2,})\\1{32,})" "phase:2,deny,id:500074"
		SecRule TX:/^MSC_/ "!\@streq 0" "phase:2,t:none,log,deny,msg:'ModSecurity internal error flagged: %{MATCHED_VAR_NAME}',id:500075"
	),
	match_log => {
		error => [ qr/internal error flagged: TX:MSC_PCRE_LIMITS_EXCEEDED/, 1 ],
		-error => [ qr/against "ARGS:test" required/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		"test=" . (("a" x 64) . "b") x 64 . "d",
	),
},
