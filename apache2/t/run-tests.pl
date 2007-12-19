#!/usr/bin/perl
use strict;
use File::Basename qw(basename dirname);

my @TYPES = qw(tfns operators);
my $TEST = "./msc-test";
my $SCRIPT = basename($0);
my $SCRIPTDIR = dirname($0);
my $PASSED = 0;
my $TOTAL = 0;

if (defined $ARGV[0]) {
	runfile(dirname($ARGV[0]), basename($ARGV[0]));
	done();
}

for my $type (sort @TYPES) {
	my $dir = "$SCRIPTDIR/$type";
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
	my($dir, $cfg) = @_;
	my $fn = "$dir/$cfg";
	my @data = ();
	my $edata;
	my @C = ();
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
		my %t = %{$t || {}};
		my $id = sprintf("%6d", $n + 1);
		my $in = $t{input};
		my $out = escape($t{output}); # Escape so we can send via commandline
		quit(1, "Failed to interpret output \"$cfg\": $@") if ($@);
		my $rc = 0;

		open(TEST, "|-", $TEST, $t{type}, $t{name}, $out, (exists($t{ret}) ? ($t{ret}) : ())) or quit(1, "Failed to execute test \"$cfg\": $!");
		print TEST "$in";
		close TEST;

		$n++;

		$rc = $?;
		$pass += $rc ? 0 : 1;
		msg(sprintf("%s) %s \"%s\": %s", $id, $t{type}, $t{name}, ($rc ? "failed" : "passed")));
		
	}

	$TOTAL += $n;
	$PASSED += $pass;

	msg(sprintf("Passed: %2d; Failed %2d", $pass, ($n - $pass)));
}

sub escape {
	my @new = ();
	for my $c (split(//, $_[0])) {
		push @new, ((ord($c) >= 0x20 and ord($c) <= 0x7e) ? $c : sprintf("\\x%02x", ord($c)));
	}
	join('', @new);
}

sub msg {
	print STDOUT "@_\n" if (@_);
}

sub quit {
	my($ec,$msg) = @_;
	$ec = 0 unless (defined $_[0]);

	msg("$msg") if (defined $msg);

	exit $ec;
}

sub done {
	if ($PASSED != $TOTAL) {
		quit(1, "\nOnly $PASSED/$TOTAL tests passed.");
	}

	quit(0, "\nAll tests passed ($TOTAL).");
}
