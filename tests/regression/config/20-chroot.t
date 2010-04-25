### SecChroot tests
# TODO: Will not work as we need root access

#{
#	type => "config",
#	comment => "SecChroot",
#	httpd_opts => qw(
#		-DCHROOT
#	),
#	conf => qq(
#		# These will be in the chroot
#		PidFile /logs/httpd.pid
#		ScoreBoardFile /logs/httpd.scoreboard
#   User nobody
#		Group nogroup
#
#		SecAuditEngine On
#		SecDebugLog $ENV{DEBUG_LOG}
#		SecDebugLogLevel 9
#		SecAuditLog $ENV{AUDIT_LOG}
#		SecAuditLogStorageDir "/logs/audit"
#		SecAuditLogType Concurrent
#		SecChrootDir "$ENV{TEST_SERVER_ROOT}"
#	),
#	match_log => {
#		debug => [ qr/./, 1 ],
#		audit => [ qr/./, 1 ],
#	},
#	match_response => {
#		status => qr/^200$/,
#	},
#	request => new HTTP::Request(
#		GET => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
#	),
#},
