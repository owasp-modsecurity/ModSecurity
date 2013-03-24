### Misc directive tests

### TODO:
# SecTmpDir
# SecUploadKeepFiles
# SecChrootDir
# SecGuardianLog

# SecDefaultAction
{
	type => "config",
	comment => "SecDefaultAction",
	conf => qq(
		SecRuleEngine on
		SecDefaultAction "phase:1,deny,status:500"
		SecRule REQUEST_URI "test.txt,id:500240"
	),
	match_log => {
		error => [ qr/ModSecurity: Access denied with code 500 \(phase 1\)/, 1 ],
	},
	match_response => {
		status => qr/^500$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# SecServerSignature
{
	type => "config",
	comment => "SecServerSignature On",
	conf => qq(
		SecServerSignature "NewServerSignature"
	),
	match_log => {
		error => [ qr/NewServerSignature/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
		raw => qr/^Server: +NewServerSignature$/m,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# SecDataDir
{
	type => "config",
	comment => "SecDataDir",
	conf => qq(
		SecRuleEngine On
		SecDataDir "$ENV{DATA_DIR}"
		SecAction initcol:ip=%{REMOTE_ADDR},setvar:ip.dummy=1,pass,id:500085
	),
	match_log => {
		error => [ qr/ModSecurity: Warning. Unconditional match in SecAction\./, 1 ],
	},
	match_file => {
		"$ENV{DATA_DIR}/ip.pag" => qr/\x00\x06dummy\x00\x00\x021\x00/,
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},

# SecTmpDir/SecUploadDir/SecUploadKeepFiles
{
	type => "config",
	comment => "SecTmpDir/SecUploadDir/SecUploadKeepFiles",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 4
		SecTmpDir "$ENV{TEMP_DIR}"
		SecUploadKeepFiles On
		SecUploadDir "$ENV{UPLOAD_DIR}"
	),
	test => sub {
		# Get the filename and make sure the file exists
		my $fn = match_log(debug => qr/Moved file from .* to ".*"\./, 5);
		die "Failed to determine uploaded filename\n" unless (defined $fn);

		$fn =~ s/Moved file from .* to "(.*)"\..*/$1/;
		die "File does not exist: $fn\n" unless (-e $fn);

		# Check the contents of the file
		return 0 if (match_file($fn, qr/^TESTFILE$/m));
		
		msg("Failed to match contents of uploaded file: $fn");
		return 1;
	},
	match_log => {
		debug => [ qr/Created temporary file.*$ENV{TEMP_DIR}/, 1 ],
		-debug => [ qr/Failed to /, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[
			"Content-Type" => "multipart/form-data; boundary=---------------------------19813181771830765643996187206",
		],
		q(-----------------------------19813181771830765643996187206
Content-Disposition: form-data; name="upload-file"; filename="test"
Content-Type: application/octet-stream

TESTFILE
-----------------------------19813181771830765643996187206
Content-Disposition: form-data; name="file"

Upload File
-----------------------------19813181771830765643996187206--),
	),
},

# SecWebAppId
{
	type => "config",
	comment => "SecWebAppId",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 4
		SecAuditLog "$ENV{AUDIT_LOG}"
		SecAuditEngine RelevantOnly
		SecWebAppId "app-1"
		SecAction "pass,log,auditlog,id:1"
	),
	match_log => {
		error => [ qr/Warning\. Unconditional match in SecAction\./, 1 ],
		debug => [ qr/Warning\. Unconditional match in SecAction\./, 1 ],
		audit => [ qr/^WebApp-Info: "app-1"/m, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
	),
},
