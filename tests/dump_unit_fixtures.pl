#!/usr/bin/env perl
#
# Evaluates an op/*.t or tfn/*.t unit-test data file the same way
# run-unit-tests.pl does (wraps the raw file contents in "@C = ( ... )" and
# evals it) and dumps the resulting list of test hashes as JSON on stdout.
#
# Perl already understands its own string-escape syntax (\xHH, \n, \t, "\\",
# etc.) correctly; letting Perl evaluate the file instead of regex-parsing the
# raw text avoids the double-escaping bugs a naive text-based converter runs
# into (see convert_perl_tests.py). param/input/output are base64-encoded
# since these fields are byte strings that are not guaranteed to be valid
# UTF-8 (e.g. deliberately malformed encoding-validator test cases).
#
# Usage: perl dump_unit_fixtures.pl path/to/file.t

use strict;
use warnings;
use JSON::PP;
use MIME::Base64 qw(encode_base64);

my $fn = shift or die "Usage: $0 <file.t>\n";

open(my $fh, "<", $fn) or die "Failed to open $fn: $!\n";
my @data = <$fh>;
close $fh;

my @C;
{
    no strict;
    my $edata = '@C = (' . join("", @data) . ')';
    eval $edata;
    die "Failed to eval $fn: $@" if $@;
}

my @out;
for my $t (@C) {
    my %h = %{ $t || {} };
    for my $key (qw(param input output)) {
        next unless exists $h{$key} and defined $h{$key};
        $h{$key} = encode_base64($h{$key}, "");
    }
    push @out, \%h;
}

print JSON::PP->new->canonical->encode(\@out);
