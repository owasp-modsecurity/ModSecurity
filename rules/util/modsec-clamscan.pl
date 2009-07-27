#!/usr/bin/perl
#
# modsec-clamscan.pl
# ModSecurity for Apache (http://www.modsecurity.org)
# Copyright (c) 2002-2007 Breach Security, Inc. (http://www.breach.com)
#
# This script is an interface between mod_security and its
# ability to intercept files being uploaded through the
# web server, and ClamAV

# by default use the command-line version of ClamAV,
# which is slower but more likely to work out of the
# box
$CLAMSCAN = "/usr/bin/clamscan";

# using ClamAV in daemon mode is faster since the
# anti-virus engine is already running, but you also
# need to configure file permissions to allow ClamAV,
# usually running as a user other than the one Apache
# is running as, to access the files
# $CLAMSCAN = "/usr/bin/clamdscan";

if (@ARGV != 1) {
    print "Usage: modsec-clamscan.pl <filename>\n";
    exit;
}

my ($FILE) = @ARGV;

$cmd = "$CLAMSCAN --stdout --disable-summary $FILE";
$input = `$cmd`;
$input =~ m/^(.+)/;
$error_message = $1;

$output = "0 Unable to parse clamscan output [$1]";

if ($error_message =~ m/: Empty file\.?$/) {
    $output = "1 empty file";
}
elsif ($error_message =~ m/: (.+) ERROR$/) {
    $output = "0 clamscan: $1";
}
elsif ($error_message =~ m/: (.+) FOUND$/) {
    $output = "0 clamscan: $1";
}
elsif ($error_message =~ m/: OK$/) {
    $output = "1 clamscan: OK";
}

print "$output\n";
