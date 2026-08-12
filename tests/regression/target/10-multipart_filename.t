
# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_FILENAME",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_FILENAME "\@contains 0" "id:1,phase:3,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Target value: "small_text_file.txt"/s, 1 ],
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
			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
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
	comment => "Testing Variables :: MULTIPART_FILENAME",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_FILENAME "\@contains 0" "id:1,phase:3,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Target value: "small_text_file2.txt"/s, 1 ],
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
			Content-Disposition: form-data; name="filedata"; filename="small_text_file2.txt"
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
	comment => "Testing Variables :: MULTIPART_FILENAME* (no regular filename)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_FILENAME "\@contains shell" "id:1,phase:2,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Target value: "shell.php"/s, 1 ],
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
			"Content-Length" => q(145),
			"Content-Type" => q(multipart/form-data; boundary=b),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--b
			Content-Disposition: form-data; name="file"; filename*=UTF-8''shell.php
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
	comment => "Testing Variables :: MULTIPART_FILENAME* (regular filename after the asterisked one)",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_FILENAME "\@contains shell" "id:1,phase:2,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Target value: "shell.php"/s, 1 ],
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



# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_FILENAME* with key",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_FILENAME:file "\@rx .*" "id:1,phase:2,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Target value: "shell.php"/s, 1 ],
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



# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_FILENAME_CHARSET",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_FILENAME_CHARSET "\@strEq utf-8" "id:1,phase:2,pass,t:none,t:trim,t:lowercase"
	),
	match_log => {
		debug => [ qr/Target value: "utf-8"/s, 1 ],
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
			"Content-Length" => q(145),
			"Content-Type" => q(multipart/form-data; boundary=b),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--b
			Content-Disposition: form-data; name="file"; filename*=UTF-8''shell.php
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
	comment => "Testing Variables :: MULTIPART_FILENAME_CHARSET with key",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_FILENAME_CHARSET:file "\@strEq utf-8" "id:1,phase:2,deny,t:none,t:trim,t:lowercase"
	),
	match_log => {
		debug => [ qr/Target value: "utf-8"/s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(145),
			"Content-Type" => q(multipart/form-data; boundary=b),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--b
			Content-Disposition: form-data; name="file"; filename*=UTF-8''shell.php
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
	comment => "Testing Variables :: MULTIPART_FILENAME_LANGUAGE",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_FILENAME_LANGUAGE:file "\@strEq en" "id:1,phase:2,pass,t:none,t:trim,t:lowercase"
	),
	match_log => {
		debug => [ qr/Target value: "en"/s, 1 ],
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
			"Content-Length" => q(147),
			"Content-Type" => q(multipart/form-data; boundary=b),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--b
			Content-Disposition: form-data; name="file"; filename*=UTF-8'en'shell.php
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
	comment => "Testing Variables :: MULTIPART_FILENAME_LANGUAGE with key",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_FILENAME_LANGUAGE:file "\@strEq fr" "id:1,phase:2,deny,t:none,t:trim,t:lowercase"
	),
	match_log => {
		debug => [ qr/Target value: "fr"/s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(143),
			"Content-Type" => q(multipart/form-data; boundary=b),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--b
			Content-Disposition: form-data; name="file"; filename*=UTF-8'fr'r%C3%A9sum%C3%A9.pdf
			Content-Type: application/pdf
			
			%PDF-1.7
			
			--b--
		),
	    ),
	),
},



# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_FILENAME with duplicate CD header",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_STRICT_ERROR "\@eq 1" "id:1,phase:2,deny,t:none,chain"
		SecRule MULTIPART_DUPLICATE_PART_HEADER "\@eq 1"
	),
	match_log => {
		debug => [ qr/Multipart: Duplicate part header: Content-Disposition./s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(212),
			"Content-Type" => q(multipart/form-data; boundary=b),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--b
			Content-Disposition: form-data; name="file"; filename*=UTF-8''shell.php
			Content-Disposition: form-data; name="file"; filename="image.jpg"
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
	comment => "Testing Variables :: MULTIPART_FILENAME with duplicate CT header",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_STRICT_ERROR "\@eq 1" "id:1,phase:2,deny,t:none,chain"
		SecRule MULTIPART_DUPLICATE_PART_HEADER "\@eq 1"
	),
	match_log => {
		debug => [ qr/Multipart: Duplicate part header: Content-Type./s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(178),
			"Content-Type" => q(multipart/form-data; boundary=b),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--b
			Content-Disposition: form-data; name="file"; filename*=UTF-8''shell.php
			Content-Type: image/jpeg
			Content-Type: application/x-php
			
			<?php system($_GET['c']); ?>
			
			--b--
		),
	    ),
	),
},


