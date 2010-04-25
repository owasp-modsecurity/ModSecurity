{
	type => "config",
	comment => "module loaded",
	match_log => {
		error => [ qr/ModSecurity for Apache.* configured\./, 10 ],
	},
},
{
	type => "config",
	comment => "minimal config",
	conf => sub {
		# Open the minimal conf file, substituting the
		# relative log paths with full paths.
		open(C, "<$ENV{DIST_ROOT}/modsecurity.conf-minimal") or die "$!\n";
		(my $conf = join('', <C>)) =~ s#Log logs/#Log $ENV{TEST_SERVER_ROOT}/logs/#g;
		close C;

		return $conf;
	},
	match_log => {
		error => [ qr/ModSecurity for Apache.* configured\./, 10 ],
	},
},
