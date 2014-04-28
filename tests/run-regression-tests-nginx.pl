#!/usr/bin/env perl
#
# Run regression tests.
#
# Syntax: run-regression-tests.pl [options] [file [N]]
#
#          All: run-regression-tests.pl
#   All in file: run-regression-tests.pl file
#   Nth in file: run-regression-tests.pl file N
#
use strict;
use Time::HiRes qw(gettimeofday sleep);
use POSIX qw(WIFEXITED WEXITSTATUS WIFSIGNALED WTERMSIG);
use File::Spec qw(rel2abs);
use File::Basename qw(basename dirname);
use File::Path qw(make_path);
use FileHandle;
use IPC::Open2 qw(open2);
use IPC::Open3 qw(open3);
use Getopt::Std;
use Data::Dumper;
use IO::Socket;
use LWP::UserAgent;
use Cwd 'abs_path';
use Template;
use File::Copy::Recursive qw(dircopy);

my @TYPES = qw(action config misc rule target);
my $SCRIPT = basename($0);
my $SCRIPT_DIR = File::Spec->rel2abs(dirname($0));
my $REG_DIR = "$SCRIPT_DIR/regression";
my $NGINX_DIR = "$REG_DIR/nginx";
my $NGINX_CONF_TEMP = "$REG_DIR/nginx/conf/nginx.conf.template";
my $NGINX = q(/usr/local/nginx/sbin/nginx);
my $SROOT_DIR = "$REG_DIR/server_root";
my $TEMP_DIR = "$SROOT_DIR/tmp";
my $DATA_DIR = "$SROOT_DIR/data";
my $UPLOAD_DIR = "$SROOT_DIR/upload";

my $PASSED = 0;
my $TOTAL = 0;
my $BUFSIZ = 32768;
my %C = ();
my %FILE = ();
my $UA_NAME = "ModSecurity Regression Tests/1.2.3";
my $UA = LWP::UserAgent->new;
$UA->agent($UA_NAME);

$SIG{TERM} = $SIG{INT} = \&handle_interrupt;


my $platform = "nginx";

my %opt;
getopts('A:E:D:C:T:H:a:p:dvh', \%opt);

if ($opt{d}) {
	$Data::Dumper::Indent = 1;
	$Data::Dumper::Terse = 1;
	$Data::Dumper::Pad = "";
	$Data::Dumper::Quotekeys = 0;
}

sub usage {
	print stderr <<"EOT";
@_
		Usage: $SCRIPT [options] [file [N]]

		Options:
		-P path   Specify nginx prefix path (default: $NGINX_DIR)
		-a file   Specify nginx binary (default: $NGINX)
		-p port   Specify nginx port (default: 8088)
		-v        Enable verbose output (details on failure).
		-d        Enable debugging output.
		-h        This help.
EOT

		exit(1);
}

usage() if ($opt{h});

### Check nginx binary
if (defined $opt{a}) {
	$NGINX = $opt{a};
}
else {
	$opt{a} = $NGINX;
}
usage("Invalid Apache startup script: $NGINX\n") unless (-e $NGINX);


### Defaults
$opt{P} = "$NGINX_DIR" unless (defined $opt{P});

my $CONF_DIR = "$opt{P}/conf";
my $FILES_DIR = "$opt{P}/logs";
my $PID_FILE = "$FILES_DIR/nginx.pid";

$opt{A} = "$FILES_DIR/modsec_audit.log";
$opt{D} = "$FILES_DIR/modsec_debug.log";
$opt{E} = "$FILES_DIR/error.log";
$opt{C} = "$CONF_DIR/nginx.conf";
$opt{p} = 8088 unless (defined $opt{p});
$opt{v} = 1 if ($opt{d});

if ( !-d "$opt{P}" ) {
    make_path($opt{P}) or die $!;
}

if ( !-d "$opt{P}/logs" ) {
    make_path("$opt{P}/logs") or die $!;
}

if ( !-d "$opt{P}/html" ) {
    make_path("$opt{P}/html") or die $!;
}

dircopy("$REG_DIR/server_root/htdocs","$opt{P}/html") or die $!;

%ENV = (
		%ENV,
		$NGINX_DIR => $opt{P},
		SERVER_PORT => $opt{p},
		SERVER_NAME => "localhost",
#    TEST_NGX_PREFIX => $NGINX_DIR,
		DATA_DIR => $DATA_DIR,
		TEMP_DIR => $TEMP_DIR,
		UPLOAD_DIR => $UPLOAD_DIR,
		CONF_DIR => $CONF_DIR,
#    MODULES_DIR => $MODULES_DIR,
		LOGS_DIR => $FILES_DIR,
		SCRIPT_DIR => $SCRIPT_DIR,
		REGRESSION_DIR => $REG_DIR,
		DIST_ROOT => File::Spec->rel2abs(dirname("$SCRIPT_DIR/../../..")),
		AUDIT_LOG => $opt{A},
		DEBUG_LOG => $opt{D},
		ERROR_LOG => $opt{E},
		NGINX_CONF => $opt{C},
#    HTDOCS => $opt{H},
		USER_AGENT => $UA_NAME,
		);

#dbg("OPTIONS: ", \%opt);

if (-e "$PID_FILE") {
	msg("Shutting down previous instance: $PID_FILE");
	nginx_stop();
}

if (defined $ARGV[0]) {
	runfile(dirname($ARGV[0]), basename($ARGV[0]), $ARGV[1]);
	done();
}

for my $type (@TYPES) {
	my $dir = "$SCRIPT_DIR/regression/$type";
	my @cfg = ();

# Get test names
	opendir(DIR, "$dir") or quit(1, "Failed to open \"$dir\": $!");
	@cfg = grep { /\.t$/ && -f "$dir/$_" } readdir(DIR);
	closedir(DIR);

	for my $cfg (sort @cfg) {
		runfile($dir, $cfg);
	}
}
done();


sub runfile {
	my($dir, $cfg, $testnum) = @_;
	my $fn = "$dir/$cfg";
	my @data = ();
	my $edata;
	my @C = ();
	my @test = ();
	my $teststr;
	my $n = 0;
	my $pass = 0;

	open(CFG, "<$fn") or quit(1, "Failed to open \"$fn\": $!");
	@data = <CFG>;

	$edata = q/@C = (/ . join("", @data) . q/)/;
	eval $edata;
	quit(1, "Failed to read test data \"$cfg\": $@") if ($@);

	unless (@C) {
		msg("\nNo tests defined for $fn");
		return;
	}

	msg("\nLoaded ".@C." tests from $fn");
	for my $t (@C) {
		$n++;
		next if (defined $testnum and $n != $testnum);

		my $nginx_up = 0;
		my %t = %{$t || {}};
		my $id = sprintf("%3d", $n);
		my $out = "";
		my $rc = 0;
		my $conf_fn;

		# watch for segfaults 
		if ($t and !$t->{match_log}) {
			$t->{match_log} = {};
		}
		if ($t and $t->{match_log} and !$t->{match_log}{-error}) {
			$t->{match_log}{-error} = [];
		}
		push $t->{match_log}{-error}, qr/(core dump)/;
		push $t->{match_log}{-error}, 1;

# Startup nginx with optionally included conf.
		if (exists $t{conf} and defined $t{conf}) {
			$conf_fn = sprintf "%s/%s_%s_%06d.conf",
				$CONF_DIR, $t{type}, $cfg, $n;
#dbg("Writing test config to: $conf_fn");
			open(CONF, ">$conf_fn") or die "Failed to open conf \"$conf_fn\": $!\n";
			print CONF (ref $t{conf} eq "CODE" ? eval { &{$t{conf}} } : $t{conf});
			msg("$@") if ($@);
			close CONF;
			my %conf=(config => $conf_fn, enable => "on");
			$nginx_up = nginx_start($t, \%conf) ? 0 : 1;
		}
		else {
			$nginx_up = nginx_start($t) ? 0 : 1;
		}

# Run any prerun setup
		if ($rc == 0 and exists $t{prerun} and defined $t{prerun}) {
			vrb("Executing perl prerun...");
			$rc = &{$t{prerun}};
			vrb("Perl prerun returned: $rc");
		}

		if ($nginx_up) {
# Perform the request and check response
			if (exists $t{request}) {
				my $resp = do_request($t{request});
				if (!$resp) {
					msg("invalid response");
					vrb("RESPONSE: ", $resp);
					$rc = 1;
				}
				else {
					for my $key (keys %{ $t{match_response} || {}}) {
						my($neg,$mtype) = ($key =~ m/^(-?)(.*)$/);
						my $m = $t{match_response}{$key};
						if (ref($m) eq "HASH") {
							if ($m->{$platform}) {
								$m = $m->{$platform};
							}
							else {
								my $ap = join(", ", keys %{$m});
								msg("Warning: trying to match response. Nothing " .
									"to match in current platform: $platform. " .
									"This test only contains cotent for: $ap.");
								last;
							}
						}

						my $match = match_response($mtype, $resp, $m);
						if ($neg and defined $match) {
							$rc = 1;
							msg("response $mtype matched: $m");
							vrb($resp);
							last;
						}
						elsif (!$neg and !defined $match) {
							$rc = 1;
							msg("response $mtype failed to match: $m");
							vrb($resp);
							last;
						}
					}
				}
			}

# Run any arbitrary perl tests
			if ($rc == 0 and exists $t{test} and defined $t{test}) {
				dbg("Executing perl test(s)...");
				$rc = eval { &{$t{test}} };
				if (! defined $rc) {
					msg("Error running test: $@");
					$rc = -1;
				}
				dbg("Perl tests returned: $rc");
			}

# Search for all log matches
			if ($rc == 0 and exists $t{match_log} and defined $t{match_log}) {
				for my $key (keys %{ $t{match_log} || {}}) {
					my($neg,$mtype) = ($key =~ m/^(-?)(.*)$/);
					my $m = $t{match_log}{$key};
					if (ref($m) eq "HASH") {
						if ($m->{$platform}) {
							$m = $m->{$platform};
						}
						else {
							my $ap = join(", ", keys %{$m});
							msg("Warning: trying to match: $mtype. Nothing " .
								"to match in current platform: $platform. " .
								"This test only contains cotent for: $ap.");
							last;
						}
					}
					my $match = match_log($mtype, @{$m || []});
					if ($neg and defined $match) {
						$rc = 1;
						msg("$mtype log matched: $m->[0]");
						last;
					}
					elsif (!$neg and !defined $match) {
						$rc = 1;
						msg("$mtype log failed to match: $m->[0]");
						last;
					}
				}
			}

# Search for all file matches
			if ($rc == 0 and exists $t{match_file} and defined $t{match_file}) {
				sleep 1; # Make sure the file exists
					for my $key (keys %{ $t{match_file} || {}}) {
						my($neg,$fn) = ($key =~ m/^(-?)(.*)$/);
						my $m = $t{match_file}{$key};
						my $match = match_file($fn, $m);
						if ($neg and defined $match) {
							$rc = 1;
							msg("$fn file matched: $m");
							last;
						}
						elsif (!$neg and !defined $match) {
							$rc = 1;
							msg("$fn file failed match: $m");
							last;
						}
					}
			}
		}
		else {
			msg("Failed to start nginx.");
			$rc = 1;
		}

		if ($rc == 0) {
			$pass++;
		}
		else {
			vrb("Test Config: $conf_fn");
			vrb("Debug Log: $FILE{debug}{fn}");
			dbg(escape("$FILE{debug}{buf}"));
			vrb("Error Log: $FILE{error}{fn}");
			dbg(escape("$FILE{error}{buf}"));
		}

		msg(sprintf("%s) %s%s: %s%s", $id, $t{type}, (exists($t{comment}) ? " - $t{comment}" : ""), ($rc ? "failed" : "passed"), ((defined($out) && $out ne "")? " ($out)" : "")));

		if ($nginx_up) {
			$nginx_up = nginx_stop(\%t) ? 0 : 1;
		}

	}

	$TOTAL += $testnum ? 1 : $n;
	$PASSED += $pass;

	msg(sprintf("Passed: %2d; Failed: %2d", $pass, $testnum ? (1 - $pass) : ($n - $pass)));
}

# Take out any indenting and translate LF -> CRLF
sub normalize_raw_request_data {
	my $r = $_[0];

# Allow for indenting in test file
	$r =~ s/^[ \t]*\x0d?\x0a//s;
	my($indention) = ($r =~ m/^([ \t]*)/s); # indention taken from first line
		$r =~ s/^$indention//mg;
	$r =~ s/(\x0d?\x0a)[ \t]+$/$1/s;

# Translate LF to CRLF
	$r =~ s/^\x0a/\x0d\x0a/mg;
	$r =~ s/([^\x0d])\x0a/$1\x0d\x0a/mg;

	return $r;
}

sub do_raw_request {
	my $sock = new IO::Socket::INET(
			Proto => "tcp",
			PeerAddr => "localhost",
			PeerPort => $opt{p},
			) or msg("Failed to connect to localhost:$opt{p}: $@");
	return unless ($sock);

	my $timeo = pack("qq", 2, 0);
	$sock->sockopt(SO_RCVTIMEO, $timeo);

# Join togeather the request
	my $r = join("", @_);
	dbg($r);

# Write to socket
	print $sock "$r";
	$sock->shutdown(1);

# Read from socket
	my @resp = <$sock>;
	$sock->close();

	return HTTP::Response->parse(join("", @resp));
}

sub do_request {
	my $r = $_[0];

# Allow test to execute code
	if (ref $r eq "CODE") {
		$r = eval { &$r };
		msg("$@") unless (defined $r);
	}

	if (ref $r eq "HTTP::Request") {
		my $resp = $UA->request($r);
		dbg($resp->request()->as_string()) if ($opt{d});
		return $resp
	}
	else {
		return do_raw_request($r);
	}

	return;
}


sub match_response {
	my($name, $resp, $re) = @_;

	msg("Warning: Empty regular expression.") if (!defined $re or $re eq "");

	if ($name eq "status") {
		return $& if ($resp->code =~ m/$re/);
	}
	elsif ($name eq "content") {
		return $& if ($resp->content =~ m/$re/m);
	}
	elsif ($name eq "raw") {
		return $& if ($resp->as_string =~ m/$re/m);
	}

	return;
}

sub read_log {
	my($name, $timeout, $graph) = @_;
	return match_log($name, undef, $timeout, $graph);
}

sub match_log {
	my($name, $re, $timeout, $graph) = @_;
	my $t0 = gettimeofday;
	my($fh,$rbuf) = ($FILE{$name}{fd}, \$FILE{$name}{buf});
	my $n = length($$rbuf);
	my $rc = undef;

	unless (defined $fh) {
		msg("Error: File \"$name\" is not opened for matching.");
		return;
	}

	$timeout = 1 unless (defined $timeout);

	if ($timeout == 1)
	{
		$timeout = 0.5;
	}

	# Audit logs are taking too long to be written on the disk. One of the
	# consequence of that is to have tests that demands to read from audit
	# log failing. Increase the timeout here, make it wait a little bit
	# more for the logs before gave up.
	if ($name eq "audit") {
		$timeout = 8;
	}

	my $i = 0;
	my $graphed = 0;
READ: {
	      do {
		      my $nbytes = $fh->sysread($$rbuf, $BUFSIZ, $n);
		      if (!defined($nbytes)) {
			      msg("Error: Could not read \"$name\" log: $!");
			      last;
		      }
		      elsif (!defined($re) and $nbytes == 0) {
			      last;
		      }

# Remove APR pool debugging
		      $$rbuf =~ s/POOL DEBUG:[^\n]+PALLOC[^\n]+\n//sg;

		      $n = length($$rbuf);

#dbg("Match \"$re\" in $name \"$$rbuf\" ($n)");
		      if ($$rbuf =~ m/$re/m) {
			      $rc = $&;
			      last;
		      }
# TODO: Use select()/poll()
		      sleep 0.1 unless ($nbytes == $BUFSIZ);
		      if ($graph and $opt{d}) {
			      $i++;
			      if ($i == 10) {
				      $graphed++;
				      $i=0;
				      print STDERR $graph if ($graphed == 1);
				      print STDERR "."
			      }
		      }
		      system("sync");
	      } while (gettimeofday - $t0 < $timeout);
      }
      print STDERR "\n" if ($graphed);

      return $rc;
}

sub match_file {
	my($neg,$fn) = ($_[0] =~ m/^(-?)(.*)$/);
	unless (exists $FILE{$fn}) {
		eval {
			$FILE{$fn}{fn} = $fn;
			$FILE{$fn}{fd} = new FileHandle($fn, O_RDONLY) or die "$!\n";
			$FILE{$fn}{fd}->blocking(0);
			$FILE{$fn}{buf} = "";
		};
		if ($@) {
			msg("Warning: Failed to open file \"$fn\": $@");
			return;
		}
	}
	return match_log($_[0], $_[1]); # timeout makes no sense
}

sub quote_shell {
	my($s) = @_;
	return $s unless ($s =~ m|[^\w!%+,\-./:@^]|);
	$s =~ s/(['\\])/\\$1/g;
	return "'$s'";
}

sub escape {
	my @new = ();
	for my $c (split(//, $_[0])) {
		my $oc = ord($c);
	push @new, ((($oc >= 0x20 and $oc <= 0x7e) or $oc == 0x0a or $oc == 0x0d) ? $c : sprintf("\\x%02x", ord($c)));
}
join('', @new);
}

sub dbg {
	return unless(@_ and $opt{d});
	my $out = join "", map {
		(ref $_ ne "" ? Dumper($_) : $_)
	} @_;
	$out =~ s/^/DBG: /mg;
	print STDOUT "$out\n";
}

sub vrb {
	return unless(@_ and $opt{v});
	msg(@_);
}

sub msg {
	return unless(@_);
	my $out = join "", map {
		(ref $_ ne "" ? Dumper($_) : $_)
	} @_;
	print STDOUT "$out\n";
}

sub handle_interrupt {
	$SIG{TERM} = $SIG{INT} = \&handle_interrupt;

	msg("Interrupted via SIG$_[0].  Shutting down tests...");
	nginx_stop();

	quit(1);
}

sub quit {
	my($ec,$msg) = @_;
	$ec = 0 unless (defined $_[0]);

	msg("$msg") if (defined $msg);

	exit $ec;
}

sub done {
	if ($PASSED != $TOTAL) {
		quit(1, "\n$PASSED/$TOTAL tests passed.");
	}

	quit(0, "\nAll tests passed ($TOTAL).");
}

sub nginx_stop {
	my $t = shift;
	my @p = (
			$NGINX,
			-p => $opt{P},
			-s => "quit",
		);

	my $nginx_out;
	my $nginx_pid = open3(undef, $nginx_out, undef, @p) or quit(1);
	my $out = join("\\n", grep(!/POOL DEBUG/, (<$nginx_out>)));
	close $nginx_out;
	waitpid($nginx_pid, 0);

	my $rc = $?;
	if ( WIFEXITED($rc) ) {
		$rc = WEXITSTATUS($rc);
		vrb("Nginx stop returned with $rc.") if ($rc);
	}
	elsif( WIFSIGNALED($rc) ) {
		msg("Nginx stop failed with signal " . WTERMSIG($rc) . ".");
		$rc = -1;
	}
	else {
		msg("Nginx stop failed with unknown error.");
		$rc = -1;
	}

	unless (defined match_log("error", qr/signal [0-9]+ \(SIGCHLD\) received/, 60, "exited with code 0")) {
		vrb(join(" ", map { quote_shell($_) } @p));
		msg("nginx server failed to shutdown.");
		sleep 0.5;
		return -1;
        }

	if (-e $PID_FILE) {
		msg("nginx server failed to shutdown.");
		return -1;
	}

	return $rc;
}


sub nginx_reset_fd {
	my($t) = @_;

# Cleanup
	for my $key (keys %FILE) {
		if (exists $FILE{$key}{fd} and defined $FILE{$key}{fd}) {
			$FILE{$key}{fd}->close();
		}
		delete $FILE{$key};
	}

# Error
	eval {
		$FILE{error}{fn} = $opt{E};
		$FILE{error}{fd} = new FileHandle($opt{E}, O_RDWR|O_CREAT) or die "$!\n";
		$FILE{error}{fd}->blocking(0);
		$FILE{error}{fd}->sysseek(0, 2);
		$FILE{error}{buf} = "";
	};
	if ($@) {
		msg("Warning: Failed to open file \"$opt{E}\": $@");
		return undef;
	}

# Audit
	eval {
		$FILE{audit}{fn} = $opt{A};
		$FILE{audit}{fd} = new FileHandle($opt{A}, O_RDWR|O_CREAT) or die "$!\n";
		$FILE{audit}{fd}->blocking(0);
		$FILE{audit}{fd}->sysseek(0, 2);
		$FILE{audit}{buf} = "";
	};
	if ($@) {
		msg("Warning: Failed to open file \"$opt{A}\": $@");
		return undef;
	}

# Debug
	eval {
		$FILE{debug}{fn} = $opt{D};
		$FILE{debug}{fd} = new FileHandle($opt{D}, O_RDWR|O_CREAT) or die "$!\n";
		$FILE{debug}{fd}->blocking(0);
		$FILE{debug}{fd}->sysseek(0, 2);
		$FILE{debug}{buf} = "";
	};
	if ($@) {
		msg("Warning: Failed to open file \"$opt{D}\": $@");
		return undef;
	}


# Any extras listed in "match_log"
	if ($t and exists $t->{match_log}) {
		for my $k (keys %{ $t->{match_log} || {} }) {
			my($neg,$fn) = ($k =~ m/^(-?)(.*)$/);
			next if (!$fn or exists $FILE{$fn});
			eval {
				$FILE{$fn}{fn} = $fn;
				$FILE{$fn}{fd} = new FileHandle($fn, O_RDWR|O_CREAT) or die "$!\n";
				$FILE{$fn}{fd}->blocking(0);
				$FILE{$fn}{fd}->sysseek(0, 2);
				$FILE{$fn}{buf} = "";
			};
			if ($@) {
				msg("Warning: Failed to open file \"$fn\": $@");
				return undef;
			}
		}
	}
}

sub encode_chunked {
	my($data, $size) = @_;
	$size = 128 unless ($size);
	my $chunked = "";

	my $n = 0;
	my $bytes = length($data);
	while ($bytes >= $size) {
		$chunked .= sprintf "%x\x0d\x0a%s\x0d\x0a", $size, substr($data, $n, $size);
		$n += $size;
		$bytes -= $size;
	}
	if ($bytes) {
		$chunked .= sprintf "%x\x0d\x0a%s\x0d\x0a", $bytes, substr($data, $n, $bytes);
	}
	$chunked .= "0\x0d\x0a\x0d\x0a"
}

sub nginx_start {
	my ($t) = shift;
	my($C) = shift;

	my %conf = (
			listen => "$opt{p}",
			config => "$REG_DIR/nginx/conf/empty.conf",
			enable => "off",
		   );

	while(my($k,$v)= each %$C){
		$conf{$k}=$v;
	}

	my ($tt) = Template->new(INCLUDE_PATH => "$REG_DIR/nginx/conf/");
	my ($output);
	$tt->process("nginx.conf.template", \%conf, \$output) || die $tt->error;

	open (OUTFILE, ">$opt{C}");
	print OUTFILE  "$output";
	close(OUTFILE);

	nginx_reset_fd($t);

	my @p = ($NGINX, -p => $opt{P});

	my $nginx_out;
	my $nginx_pid = open3(undef, $nginx_out, undef, @p) or quit(1);
	my $out = join("\\n", grep(!/POOL DEBUG/, (<$nginx_out>)));
	close $nginx_out;
	waitpid($nginx_pid, 0);

	my $rc = $?;
	if ( WIFEXITED($rc) ) {
		$rc = WEXITSTATUS($rc);
		vrb("Nginx start returned with $rc.") if ($rc);
	}
	elsif( WIFSIGNALED($rc) ) {
		msg("Nginx start failed with signal " . WTERMSIG($rc) . ".");
		$rc = -1;
	}
	else {
		msg("Nginx start failed with unknown error.");
		$rc = -1;
	}

	# Look for startup msworker cycleg
	unless (defined match_log("error", qr/start worker process/, 60)) {
		vrb(join(" ", map { quote_shell($_) } @p));
		msg("Nginx server failed to start.");
		nginx_stop();
		return -1;
	}

	return $rc;
}

