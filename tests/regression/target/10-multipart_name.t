
# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_NAME",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_NAME "\@contains abc" "id:1,phase:3,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Target value: "123abc45"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(531),
			"Content-Type" => q(multipart/form-data; boundary=--------------------------756b6d74fa1a8ee2),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			----------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="name"
			
			test
			----------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="123abc45"; filename="small_text_file.txt"
			Content-Type: text/plain
			
			This is a very small test file..
			----------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
			Content-Type: text/plain
			
			This is another very small test file..
			----------------------------756b6d74fa1a8ee2--
		),
	    ),
	),
},



# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_NAME",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_NAME "\@contains abc" "id:1,phase:3,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Target value: "123abc45"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(532),
			"Content-Type" => q(multipart/form-data; boundary=--------------------------756b6d74fa1a8ee2),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			----------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="name"
			
			test
			----------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
			Content-Type: text/plain
			
			This is a very small test file..
			----------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="123abc45"; filename="small_text_file2.txt"
			Content-Type: text/plain
			
			This is another very small test file..
			----------------------------756b6d74fa1a8ee2--
		),
	    ),
	),
},


# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_NAME (no 'name' attr)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_NAME "\@contains shell" "id:1,phase:2,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Multipart parsing error: Multipart: Content-Disposition header missing name field./s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(126),
			"Content-Type" => q(multipart/form-data; boundary=b),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--b
			Content-Disposition: form-data; filename="shell.jpg"
			Content-Type: image/jpeg
			
			<?php system($_GET['c']); ?>
			
			--b--
		),
	    ),
	),
},



# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_NAME with key",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_NAME:file "\@rx .*" "id:1,phase:2,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Target value: "file"/s, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(167),
			"Content-Type" => q(multipart/form-data; boundary=b),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--b
			Content-Disposition: form-data; name="file"; filename*=UTF-8''shell.php; filename="image.jpg"
			Content-Type: image/jpeg
			
			<?php system($_GET['c']); ?>
			
			--b--
		),
	    ),
	),
},



