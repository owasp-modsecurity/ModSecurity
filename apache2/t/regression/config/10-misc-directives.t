### Misc directive tests

### TODO:
# SecTmpDir
# SecUploadDir
# SecUploadKeepFiles
# SecWebAppId
# SecDataDir
# SecChrootDir
# SecGuardianLog

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

# SecDefaultAction
{
	type => "config",
	comment => "SecServerSignature On",
	conf => qq(
		SecRuleEngine on
		SecDefaultAction "phase:1,deny,status:500"
		SecRule REQUEST_URI "test.txt"
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
