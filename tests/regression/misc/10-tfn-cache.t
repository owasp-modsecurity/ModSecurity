### Transformation Caching

{
	type => "misc",
	comment => "tfncache (simple fully cached)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		# We need to make this work no matter what the defaults may change to
		SecCacheTransformations On "minlen:1,maxlen:0"

		# This should cache it
		SecRule ARGS_GET "WillNotMatch" "phase:1,t:none,t:removeWhiteSpace,t:lowercase,pass,nolog,id:500037"

		# This should use the cached value
		SecRule ARGS_GET:test "foobar" "phase:1,t:none,t:removeWhiteSpace,t:lowercase,deny,id:500038"
	),
	match_log => {
		debug => [ qr/removeWhiteSpace,lowercase: "foobar" .*cached/, 1 ],
		-debug => [ qr/partially cached/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?test=Foo+Bar",
	),
},
{
	type => "misc",
	comment => "tfncache (simple partially cached)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		# We need to make this work no matter what the defaults may change to
		SecCacheTransformations On "minlen:1,maxlen:0,incremental:off,maxitems:0"

		# This should cache it
		SecRule ARGS_GET "WillNotMatch" "phase:1,t:none,t:removeWhiteSpace,pass,nolog,id:500039"

		# This should use the partially cached value
		SecRule ARGS_GET:test "foobar" "phase:1,t:none,t:removeWhiteSpace,t:lowercase,deny,id:500040"
	),
	match_log => {
		debug => [ qr/removeWhiteSpace: "FooBar" .*partially cached/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?test=Foo+Bar",
	),
},
{
	type => "misc",
	comment => "tfncache (separate phases)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		# We need to make this work no matter what the defaults may change to
		SecCacheTransformations On "minlen:1,maxlen:0"

		# This should cache it
		SecRule ARGS_GET "WillNotMatch" "phase:1,t:none,t:removeWhiteSpace,t:lowercase,pass,nolog,id:500041"

		# This should use the cached value
		SecRule ARGS_GET:test "foobar" "phase:2,t:none,t:removeWhiteSpace,t:lowercase,deny,id:500042"
	),
	match_log => {
		-debug => [ qr/removeWhiteSpace,lowercase: "foobar" .*cached/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?test=Foo+Bar",
	),
},
{
	type => "misc",
	comment => "tfncache (non-modifying tfns cached)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9

		# We need to make this work no matter what the defaults may change to
		SecCacheTransformations On "minlen:1,maxlen:0"

		# This should cache it
		SecRule ARGS_GET "WillNotMatch" "phase:1,t:none,t:removeWhiteSpace,t:lowercase,pass,nolog,id:500043"

		# This should use the cached value
		SecRule ARGS_GET:test "foobar" "phase:1,t:none,t:removeWhiteSpace,t:lowercase,deny,id:500044"
	),
	match_log => {
		debug => [ qr/removeWhiteSpace,lowercase: "foobar" .*cached/, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?test=foo+bar",
	),
},
{
	type => "misc",
	comment => "tfncache (unique keys)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On

		# We need to make this work no matter what the defaults may change to
		SecCacheTransformations On "minlen:1,maxlen:0"

		# This should cache it
		SecRule ARGS "WillNotMatch" "phase:2,t:none,t:removeWhiteSpace,t:lowercase,pass,id:500045"

		# This should see cached versions of *both* ARGS_GET
		SecRule ARGS:test "queryval" "phase:2,t:none,t:removeWhiteSpace,t:lowercase,deny,chain,id:500046"
		SecRule ARGS:test "firstval" "t:none,t:removeWhiteSpace,t:lowercase,chain,id:500047"
		SecRule ARGS:test "secondval" "t:none,t:removeWhiteSpace,t:lowercase,id:500017"
	),
	match_log => {
		debug => [ qr/removeWhiteSpace,lowercase: "queryval" .*removeWhiteSpace,lowercase: "firstval" .*cached.*removeWhiteSpace,lowercase: "secondval" .*cached/s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html?test=Query+Val",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		#  Args
		"test=First+Val&test=Second+Val",
	),
},
{
	type => "misc",
	comment => "tfncache (large cache)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On

		<IfDefine MODSEC_2.5>
			SecRequestBodyNoFilesLimit 1048576
		</IfDefine>
		SecRequestBodyInMemoryLimit 131072
		SecResponseBodyLimit 1048576

		# We need to make this work no matter what the defaults may change to
		SecCacheTransformations On "minlen:1,maxlen:0,maxitems:0"

		# This should cache it in all phases
		SecRule ARGS "WillNotMatch" "phase:1,t:none,t:removeWhiteSpace,t:lowercase,pass,nolog,id:500048"
		SecRule ARGS "WillNotMatch" "phase:2,t:none,t:removeWhiteSpace,t:lowercase,pass,nolog,id:500049"
		SecRule ARGS "WillNotMatch" "phase:3,t:none,t:removeWhiteSpace,t:lowercase,pass,nolog,id:500050"
		SecRule ARGS "WillNotMatch" "phase:4,t:none,t:removeWhiteSpace,t:lowercase,pass,nolog,id:500051"

		# This should use the cached value
		SecRule ARGS "foobar" "phase:4,t:none,t:removeWhiteSpace,t:lowercase,deny,id:500052"
	),
	match_log => {
		debug => [ qr/Adding request argument \(BODY\): name "test", value "Foo Bar"/, 60, "Waiting for httpd to process request: "],
		-error => [ qr/segmentation fault/i, 60 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		# 1000 Args
		join("&", map { sprintf "arg%08d=0123456789abcdef+0123456789ABCDEF+0123456789abcdef", $_ } (1 .. 1000))."&test=Foo+Bar",
	),
},
