### libinjection.

{
	type => "misc",
	comment => "fuzzyHash test",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On

		SecRule REQUEST_BODY "\@fuzzyHash $ENV{CONF_DIR}/ssdeep.txt 1" "id:192372,log,deny"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 403 \(phase 2\)\. Fuzzy hash of REQUEST_BODY matched with 96:MbQ1L0LDX8GPI8ov3D2D9zd6.*"modsecurity.conf-recommended"|ModSecurity was not compiled with ssdeep support./, 1],
		debug => [ qr/Access denied with code 403 \(phase 2\)\. Fuzzy hash of REQUEST_BODY matched with 96:MbQ1L0LDX8GPI8ov3D2D9zd6.*"modsecurity.conf-recommended"|ModSecurity was not compiled with ssdeep support./, 1],
	},
	match_response => {
		status => qr/^403|200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/index.html",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		#  Args
		"
# -- Rule engine initialization ----------------------------------------------

# Enable ModSecurity, attaching it to every transaction. Use detection
# only to start with, because that minimises the chances of post-installation
# disruption.
#
SecRuleEngine DetectionOnly


# -- Request body handling ---------------------------------------------------

# Allow ModSecurity to access request bodies. If you don't, ModSecurity
# won't be able to see any POST parameters, which opens a large security
# hole for attackers to exploit.
#
SecRequestBodyAccess On


# Enable XML request body parser.
# Initiate XML Processor in case of xml content-type
#
SecRule REQUEST_HEADERS:Content-Type \"text/xml\" \
     \"id:'200000',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=XML\"

# Enable JSON request body parser.
# Initiate JSON Processor in case of JSON content-type; change accordingly
# if your application does not use 'application/json'
#
SecRule REQUEST_HEADERS:Content-Type \"application/json\" \
     \"id:'200001',phase:1,t:none,t:lowercase,pass,nolog,ctl:requestBodyProcessor=JSON\"

# Maximum request body size we will accept for buffering. If you support
# file uploads then the value given on the first line has to be as large
# as the largest file you are willing to accept. The second value refers
# to the size of data, with files excluded. You want to keep that value as
# low as practical.
#
SecRequestBodyLimit 13107200
SecRequestBodyNoFilesLimit 131072

# Store up to 128 KB of request body data in memory. When the multipart
# parser reachers this limit, it will start using your hard disk for
# storage. That is slow, but unavoidable.
#
SecRequestBodyInMemoryLimit 131072

# What do do if the request body size is above our configured limit.
# Keep in mind that this setting will automatically be set to ProcessPartial
# when SecRuleEngine is set to DetectionOnly mode in order to minimize
# disruptions when initially deploying ModSecurity.
#
SecRequestBodyLimitAction Reject

# Verify that we've correctly processed the request body.
# As a rule of thumb, when failing to process a request body
# you should reject the request (when deployed in blocking mode)
# or log a high-severity alert (when deployed in detection-only mode).
#
SecRule REQBODY_ERROR \"!\@eq 0\" \
\"id:'200002', phase:2,t:none,log,deny,status:400,msg:'Failed to parse request body.',logdata:'%{reqbody_error_msg}',severity:2\"
		"
	),
},

{
	type => "misc",
	comment => "fuzzyHash test",
	conf => qq(
		SecRuleEngine On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
                SecRequestBodyAccess On

		SecRule REQUEST_BODY "\@fuzzyHash $ENV{CONF_DIR}/ssdeep.txt 1" "id:192372,log,deny"
	),
	match_log => {
		-error => [ qr/Fuzzy hash of REQUEST_BODY matched/, 1],
		-debug => [ qr/Fuzzy hash of REQUEST_BODY matched/, 1],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "application/x-www-form-urlencoded",
		],
		#  Args
		"
		wheee
		"
	),
},

{
    type => "misc",
    comment => "fuzzy hash with FILES_TMP_CONTENT",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
	SecUploadKeepFiles On

	SecRule FILES_TMP_CONTENT "\@fuzzyHash $ENV{CONF_DIR}/ssdeep.txt 1" "id:192372,log,deny"
    ),
    match_log => {
        debug => [ qr/operator \"fuzzyHash\" with param \".*ssdeep.txt 1\" against FILES_TMP_CONTENT:image1/s, 1 ],
    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary=0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"

                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"

                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image1"; filename="image1.jpg"
                Content-Type: image/jpeg

                BINARYDATA1
                --0000
                Content-Disposition: form-data; name="image2"; filename="image2.jpg"
                Content-Type: image/jpeg

                BINARYDATA2
                --0000
                Content-Disposition: form-data; name="image3"; filename="image3.jpg"
                Content-Type: image/jpeg

                BINARYDATA3
                --0000
                Content-Disposition: form-data; name="test"

                This is test data.
                --0000--
            ),
        ),
    ),
},


