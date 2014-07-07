### libinjection.

{
	type => "misc",
	comment => "SecHash engine - missing paramenter",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On
		SecContentInjection On
		SecStreamOutBodyInspection On
		SecDisableBackendCompression On

		SecHashEngine On
		SecHashParam "hmac"
		SecHashKey "some_key" KeyOnly
		SecHashMethodRx "HashHref" "\.(txt)"
		SecRule REQUEST_URI "\@validateHash \.(txt)" "phase:2,id:1000,t:none,ctl:hashEnforcement=On,deny"
	),
	match_log => {
		error => [ qr/Request URI matched .* at REQUEST_URI. No Hash parameter/, 1],
		debug => [ qr/Request URI without hash parameter/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
{
	type => "misc",
	comment => "SecHash engine - wrong paramenter",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On
		SecContentInjection On
		SecStreamOutBodyInspection On
		SecDisableBackendCompression On

		SecHashEngine On
		SecHashParam "hmac"
		SecHashKey "some_key" KeyOnly
		SecHashMethodRx "HashHref" "\.(txt)"
		SecRule REQUEST_URI "\@validateHash \.(txt)" "phase:2,id:1000,t:none,ctl:hashEnforcement=On,deny"
	),
	match_log => {
		error => [ qr/Request URI matched .* at REQUEST_URI. Hash parameter hash value = .* Requested URI hash value = .*/, 1],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?hmac=test",
	),
},
{
	type => "misc",
	comment => "SecHash engine - correct paramenter",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On
		SecContentInjection On
		SecStreamOutBodyInspection On
		SecDisableBackendCompression On

		SecHashEngine On
		SecHashParam "hmac"
		SecHashKey "some_key" KeyOnly
		SecHashMethodRx "HashHref" "\.(txt)"
		SecRule REQUEST_URI "\@validateHash \.(txt)" "phase:2,id:1000,t:none,ctl:hashEnforcement=On,deny"
	),
	match_log => {
		-error => [ qr/Request URI matched .* at REQUEST_URI. Hash parameter hash value = .* Requested URI hash value = .*/, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt?hmac=a17d5c0a78b54a15f19ccb5d47651470c7b8f31f",
	),
},
{
	type => "misc",
	comment => "SecHash engine - link verification",
	conf => qq(
		SecRuleEngine On
		SecDebugLogLevel 9
		SecDebugLog $ENV{DEBUG_LOG}

		SecContentInjection On
		SecStreamOutBodyInspection On
		SecDisableBackendCompression On

		SecHashEngine On
		SecHashParam "hmac"
		SecHashKey "rand" "KeyOnly"
		SecHashMethodRx "HashHref" "testparam"
	),
	match_log => {
		-error => [ qr/Request URI matched .* at REQUEST_URI. Hash parameter hash value = .* Requested URI hash value = .*/, 1],
	},
	match_response => {
		content => qr/testparam=123456&amp;hmac=a95a6c5324869521e82e8b2465dd36a27b0b6d27/,
	},

	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/hash_test.html",
	),
},



