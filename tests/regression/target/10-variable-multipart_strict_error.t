
# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_STRICT_ERROR",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_STRICT_ERROR "\@contains 0" "id:1,phase:3,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Multipart: Warning: boundary whitespace in C-T header/s, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(523),
			"Content-Type" => q(multipart/form-data; boundary= ------------------------756b6d74fa1a8ee2),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="name"
			
			test
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
			Content-Type: text/plain
			
			This is a very small test file..
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
			Content-Type: text/plain
			
			This is another very small test file..
			--------------------------756b6d74fa1a8ee2--
		),
	    ),
	),
},



# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_STRICT_ERROR",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_STRICT_ERROR "\@contains 0" "id:1,phase:3,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Multipart: Warning: boundary was quoted./s, 1 ],
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
			"Content-Length" => q(523),
			"Content-Type" => q(multipart/form-data; boundary="------------------------756b6d74fa1a8ee2"),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="name"
			
			test
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
			Content-Type: text/plain
			
			This is a very small test file..
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
			Content-Type: text/plain
			
			This is another very small test file..
			--------------------------756b6d74fa1a8ee2--
		),
	    ),
	),
},



# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_STRICT_ERROR",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_STRICT_ERROR "\@contains 0" "id:1,phase:3,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Multipart: Warning: seen data before first boundary/s, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(528),
			"Content-Type" => q(multipart/form-data; boundary=--------------------------756b6d74fa1a8ee2),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="name"
			
			test
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
			Content-Type: text/plain
			
			This is a very small test file..
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
			Content-Type: text/plain
			
			This is another very small test file..
			--------------------------756b6d74fa1a8ee2--whee.
		),
	    ),
	),
},



# 
# can't check this because test framework sends the correct LF
#{
#	type => "misc",
#	comment => "Testing Variables :: MULTIPART_STRICT_ERROR",
#	conf => qq(
#		SecRuleEngine On
#		SecDebugLog $ENV{DEBUG_LOG}
#		SecDebugLogLevel 9
#		SecRequestBodyAccess On
#		SecRuleEngine On
#		SecRule MULTIPART_STRICT_ERROR "\@contains 0" "id:1,phase:3,pass,t:trim"
#	),
#	match_log => {
#		debug => [ qr/Warning: incorrect line endings used \(LF\)/s, 1 ],
#	},
#	match_response => {
#		status => qr/^200$/,
#	},
#	request => new HTTP::Request(
#		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
#		[
#			"Host" => q(localhost),
#			"User-Agent" => q(curl/7.38.0),
#			"Accept" => q(*/*),
#			"Content-Length" => q(531),
#			"Content-Type" => q(multipart/form-data; boundary=--------------------------756b6d74fa1a8ee2),
#			"Expect" => q(100-continue),
#		],
#	    normalize_raw_request_data(
#		q(
#			----------------------------756b6d74fa1a8ee2
#			Content-Disposition: form-data; name="name"
#			
#			test
#			----------------------------756b6d74fa1a8ee2
#			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
#			Content-Type: text/plain
#			
#			This is a very small test file..
#			----------------------------756b6d74fa1a8ee2
#			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
#			Content-Type: text/plain
#			
#			This is another very small test file..
#			----------------------------756b6d74fa1a8ee2--
#		),
#	    ),
#	),
#},



# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_STRICT_ERROR",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_STRICT_ERROR "\@contains 0" "id:1,phase:3,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Multipart: Warning: seen data before first boundary/s, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(523),
			"Content-Type" => q(multipart/form-data; boundary=--------------------------756b6d74fa1a8ee2),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="name"
			
			test
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name='filedata'; filename="small_text_file.txt"
			Content-Type: text/plain
			
			This is a very small test file..
			--------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="filedata"; filename="small_text_file.txt"
			Content-Type: text/plain
			
			This is another very small test file..
			--------------------------756b6d74fa1a8ee2--
		),
	    ),
	),
},



# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_STRICT_ERROR - RFC2046",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule REQBODY_ERROR "\@contains 0" "id:1,phase:3,pass,t:trim"
	),
	match_log => {
		debug => [ qr/Target value: "0"/s, 1 ],
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
			"Content-Length" => q(210),
			"Content-Type" => q(multipart/form-data; boundary=0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz '()+_,-./:=?),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz '()+_,-./:=?
			Content-Disposition: form-data; name="name"
			
			1
			--0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz '()+_,-./:=?--
		),
	    ),
	),
},



# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_STRICT_ERROR - IQ ",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_STRICT_ERROR "!\@eq 0" "id:1,phase:2,deny,status:403"
	),
	match_log => {
		debug => [ qr/Warning: invalid quoting used/s, 1 ],
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
			"Content-Length" => q(530),
			"Content-Type" => q(multipart/form-data; boundary=--------------------------756b6d74fa1a8ee2),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			----------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="name"
			
			test
			----------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name=file'data; filename="small_text_file.txt"
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
	comment => "Testing Variables :: MULTIPART_STRICT_ERROR - IQ ",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_INVALID_QUOTING "!\@eq 0" "id:1,phase:2,deny,status:403"
		SecRule MULTIPART_STRICT_ERROR "!\@eq 0" "id:2,phase:2,pass"
	),
	match_log => {

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
			Content-Disposition: form-data; name="file'data"; filename="small_text_file.txt"
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
	comment => "Testing Variables :: MULTIPART_STRICT_ERROR with duplicate CT header",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_STRICT_ERROR "!\@eq 0" "id:'200003',phase:2,t:none,log,deny,status:400,msg:'Multipart request body failed strict validation: DH %{MULTIPART_DUPLICATE_PART_HEADER}'"
	),
	match_log => {
		debug => [ qr/Multipart: Duplicate part header: Content-Type./s, 1 ],
	},
	match_response => {
		status => qr/^400$/,
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



# 
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_STRICT_ERROR with invalid filename* syntax",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecRule MULTIPART_STRICT_ERROR "!\@eq 0" "id:'200003',phase:2,t:none,log,deny,status:400,msg:'Multipart request body failed strict validation: IQ %{MULTIPART_INVALID_QUOTING}'"
	),
	match_log => {
		debug => [ qr/Multipart: Invalid Content-Disposition header \(-17/s, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(359),
			"Content-Type" => q(multipart/form-data; boundary=--------------------------756b6d74fa1a8ee2),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			----------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="name"
			
			test
			----------------------------756b6d74fa1a8ee2
			Content-Disposition: form-data; name="filedata"; filename="03CB1664.txt"; filename*=UTF-8'03CB1664.txt
			Content-Type: text/plain
			
			This is a very small test file..
			----------------------------756b6d74fa1a8ee2--
		),
	    ),
	),
},


#
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_STRICT_ERROR with duplicate filename syntax",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecAction "id:990100,phase:2,pass,log,t:none,msg:'PARSER_STATE',logdata:'PARSER_STATE',logdata:'RE %{REQBODY_ERROR}, BQ %{MULTIPART_BOUNDARY_QUOTED}, BW %{MULTIPART_BOUNDARY_WHITESPACE}, DB %{MULTIPART_DATA_BEFORE}, DA %{MULTIPART_DATA_AFTER}, DH %{MULTIPART_DUPLICATE_PART_HEADER}, HF %{MULTIPART_HEADER_FOLDING}, LF %{MULTIPART_LF_LINE}, SM %{MULTIPART_MISSING_SEMICOLON}, IQ %{MULTIPART_INVALID_QUOTING}, IP %{MULTIPART_INVALID_PART}, IH %{MULTIPART_INVALID_HEADER_FOLDING}, FL %{MULTIPART_FILE_LIMIT_EXCEEDED}'"
        SecRule REQBODY_ERROR "!\@eq 0" "id:990101,phase:2,deny,status:400,log,t:none"
        SecRule MULTIPART_STRICT_ERROR "!\@eq 0" "id:990102,phase:2,deny,status:400,log,t:none"
	),
	match_log => {
		debug => [ qr/Multipart parsing error: Multipart: Invalid Content-Disposition header \(\-20\): form-data; name="file"; filename="safe.jpg"; filename="safe.php"./s, 1 ],
		error => [ qr/data \"RE 1, BQ 0, BW 0, DB 0, DA 0, DH 1/s, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(157),
			"Content-Type" => q(multipart/form-data; boundary=AaB03x),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--AaB03x
			Content-Disposition: form-data; name="file"; filename="safe.jpg"; filename="safe.php"
			Content-Type: application/octet-stream

			test
			--AaB03x--
		),
	    ),
	),
},

#
{
	type => "misc",
	comment => "Testing Variables :: MULTIPART_STRICT_ERROR with duplicate filename syntax",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRequestBodyAccess On
		SecRuleEngine On
		SecAction "id:990100,phase:2,pass,log,t:none,msg:'PARSER_STATE',logdata:'RE %{REQBODY_ERROR}, BQ %{MULTIPART_BOUNDARY_QUOTED}, BW %{MULTIPART_BOUNDARY_WHITESPACE}, DB %{MULTIPART_DATA_BEFORE}, DA %{MULTIPART_DATA_AFTER}, DH %{MULTIPART_DUPLICATE_PART_HEADER}, HF %{MULTIPART_HEADER_FOLDING}, LF %{MULTIPART_LF_LINE}, SM %{MULTIPART_MISSING_SEMICOLON}, IQ %{MULTIPART_INVALID_QUOTING}, IP %{MULTIPART_INVALID_PART}, IH %{MULTIPART_INVALID_HEADER_FOLDING}, FL %{MULTIPART_FILE_LIMIT_EXCEEDED}'"
        SecRule REQBODY_ERROR "!\@eq 0" "id:990101,phase:2,deny,status:400,log,t:none"
        SecRule MULTIPART_STRICT_ERROR "!\@eq 0" "id:990102,phase:2,deny,status:400,log,t:none"
	),
	match_log => {
		debug => [ qr/Multipart parsing error: Multipart: Invalid Content-Disposition header \(\-20\): form-data; name="file"; filename\*=UTF8''safe.jpg; filename\*=UTF8''safe.php./s, 1 ],
		error => [ qr/data \"RE 1, BQ 0, BW 0, DB 0, DA 0, DH 1/s, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Host" => q(localhost),
			"User-Agent" => q(curl/7.38.0),
			"Accept" => q(*/*),
			"Content-Length" => q(167),
			"Content-Type" => q(multipart/form-data; boundary=AaB03x),
			"Expect" => q(100-continue),
		],
	    normalize_raw_request_data(
		q(
			--AaB03x
			Content-Disposition: form-data; name="file"; filename*=UTF8''safe.jpg; filename*=UTF8''safe.php
			Content-Type: application/octet-stream

			test
			--AaB03x--
		),
	    ),
	),
},
