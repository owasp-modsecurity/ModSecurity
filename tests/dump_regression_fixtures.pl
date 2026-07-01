#!/usr/bin/env perl
#
# Evaluates a tests/regression/<type>/*.t file the same way
# run-regression-tests.pl does (same %ENV setup, same "@C = ( ... )" eval
# trick) and dumps the resulting list of test hashes as JSON on stdout.
#
# This is a one-time/dev-time migration tool: it lets Perl itself evaluate
# the file (resolving $ENV{...} interpolation, qr// objects, HTTP::Request
# objects, and conf=>sub{} coderefs exactly the way the real Perl harness
# does) instead of a hand-rolled Python parser re-implementing Perl syntax.
#
# Usage: perl dump_regression_fixtures.pl path/to/file.t

use strict;
use warnings;
use JSON::PP;
use MIME::Base64 qw(encode_base64);
use File::Basename qw(basename dirname);
use File::Spec::Functions qw(rel2abs);
use HTTP::Request;
use re 'regexp_pattern';

my $SCRIPT_DIR = File::Spec->rel2abs(dirname(__FILE__));
my $REG_DIR = "$SCRIPT_DIR/regression";
my $SROOT_DIR = "$REG_DIR/server_root";
my $DATA_DIR = "$SROOT_DIR/data";
my $TEMP_DIR = "$SROOT_DIR/tmp";
my $UPLOAD_DIR = "$SROOT_DIR/upload";
my $CONF_DIR = "$SROOT_DIR/conf";
my $FILES_DIR = "$SROOT_DIR/logs";
# run-regression-tests.pl (generated from run-regression-tests.pl.in by
# ./configure) already has $HTTPD/$MODULES_DIR resolved for this platform.
# This script is a plain .pl (not autoconf-substituted), so it reads those
# two values out of the generated file rather than duplicating configure
# logic.
my ($HTTPD, $MODULES_DIR);
my $runner = "$SCRIPT_DIR/run-regression-tests.pl";
if (-e $runner) {
    open(my $rfh, "<", $runner) or die "Failed to open $runner: $!\n";
    local $/;
    my $content = <$rfh>;
    close $rfh;
    ($HTTPD) = $content =~ /^\s*my\s+\$HTTPD\s*=\s*q\(([^)]*)\)/m;
    ($MODULES_DIR) = $content =~ /^\s*my\s+\$MODULES_DIR\s*=\s*q\(([^)]*)\)/m;
}
$HTTPD ||= "/usr/local/apache2/bin/httpd";
$MODULES_DIR ||= "/usr/local/apache2/modules";

my $httpd_root = `$HTTPD -V 2>/dev/null`;
(my $server_root = $httpd_root) =~ s/.*-D HTTPD_ROOT="([^"]*)".*/$1/sm;

# Same %ENV keys as run-regression-tests.pl, so conf/request strings that
# interpolate $ENV{...} evaluate identically here and at real test-run time.
%ENV = (
    %ENV,
    SERVER_ROOT => $server_root,
    SERVER_PORT => 8088,
    SERVER_NAME => "localhost",
    TEST_SERVER_ROOT => $SROOT_DIR,
    DATA_DIR => $DATA_DIR,
    TEMP_DIR => $TEMP_DIR,
    UPLOAD_DIR => $UPLOAD_DIR,
    CONF_DIR => $CONF_DIR,
    MODULES_DIR => $MODULES_DIR,
    LOGS_DIR => $FILES_DIR,
    SCRIPT_DIR => $SCRIPT_DIR,
    REGRESSION_DIR => $REG_DIR,
    DIST_ROOT => File::Spec->rel2abs(dirname("$SCRIPT_DIR/../..")),
    AUDIT_LOG => "$FILES_DIR/modsec_audit.log",
    DEBUG_LOG => "$FILES_DIR/modsec_debug.log",
    ERROR_LOG => "$FILES_DIR/error.log",
    HTTPD_CONF => "$CONF_DIR/httpd.conf",
    HTDOCS => "$SROOT_DIR/htdocs",
    USER_AGENT => "ModSecurity Regression Tests/1.2.3",
    RUNASUSER => $ENV{USER} || $ENV{LOGNAME} || $ENV{USERNAME} || 'unknown',
);

my $PLATFORM = "apache";

# Same helper the .t files themselves call (e.g. inside conf=>sub{}); a
# faithful stand-in for run-regression-tests.pl's own copy.
sub normalize_raw_request_data {
    my $r = $_[0];
    $r =~ s/^[ \t]*\x0d?\x0a//s;
    my($indention) = ($r =~ m/^([ \t]*)/s);
    $r =~ s/^$indention//mg;
    $r =~ s/(\x0d?\x0a)[ \t]+$/$1/s;
    $r =~ s/^\x0a/\x0d\x0a/mg;
    $r =~ s/([^\x0d])\x0a/$1\x0d\x0a/mg;
    return $r;
}

# Minimal stand-in for run-regression-tests.pl's encode_chunked(), used by
# config/10-request-directives.t to build raw chunked-transfer-encoding
# request bodies. Kept here so `conf=>sub{}`/`request` strings that call it
# evaluate identically to the real harness.
sub encode_chunked {
    my ($data, $chunk_size) = @_;
    $chunk_size ||= length($data) || 1;
    my $out = "";
    while (length($data)) {
        my $chunk = substr($data, 0, $chunk_size, "");
        $out .= sprintf("%x\x0d\x0a%s\x0d\x0a", length($chunk), $chunk);
    }
    $out .= "0\x0d\x0a\x0d\x0a";
    return $out;
}

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

sub resolve_platform {
    my ($val) = @_;
    return $val unless ref($val) eq 'HASH';
    # Distinguish a real platform-dispatch hash ({apache=>..., nginx=>...})
    # from an ordinary nested hash (e.g. a per-key match spec) by checking
    # for known platform names.
    for my $p ($PLATFORM, qw(apache nginx iis)) {
        return $val->{$p} if exists $val->{$p};
    }
    return $val;
}

# Turns a Regexp ref OR a plain scalar into a serialized pattern. The Perl
# harness's match_log()/match_response() do `$buf =~ m/$re/`, and Perl
# matches a plain string the same way it matches a qr// there - some .t
# files rely on that and just write a literal string instead of qr/.../.
sub serialize_pattern {
    my ($val) = @_;
    if (ref($val) eq 'Regexp') {
        my ($pattern, $flags) = regexp_pattern($val);
        return { __regex__ => 1, pattern => $pattern, flags => $flags };
    }
    elsif (ref($val) eq 'CODE') {
        return { __unsupported_coderef__ => 1 };
    }
    else {
        return { __regex__ => 1, pattern => $val, flags => '' };
    }
}

# One match_log/match_response/match_file value, after platform-dispatch
# resolution: either a bare pattern (match_response), or [pattern, timeout,
# graph?] (match_log/match_file) where only the first element is a pattern.
sub serialize_match_entry {
    my ($val) = @_;
    if (ref($val) eq 'ARRAY') {
        my @out = @$val;
        $out[0] = serialize_pattern(resolve_platform($out[0]));
        return \@out;
    }
    else {
        return serialize_pattern($val);
    }
}

sub serialize_match_spec {
    my ($spec) = @_;
    return undef unless defined $spec;
    my %out;
    for my $key (keys %$spec) {
        $out{$key} = serialize_match_entry(resolve_platform($spec->{$key}));
    }
    return \%out;
}

sub serialize_request {
    my ($req) = @_;
    return undef unless defined $req;
    if (ref($req) eq 'CODE') {
        $req = eval { &$req };
        die "request coderef failed: $@" if $@;
    }
    if (ref($req) eq 'HTTP::Request') {
        my @headers;
        $req->headers->scan(sub { push @headers, [$_[0], $_[1]] });
        return {
            __type__ => 'http_request',
            method => $req->method,
            uri => $req->uri->as_string,
            headers => \@headers,
            content => encode_base64($req->content // '', ''),
        };
    }
    elsif (ref($req) eq '') {
        # Raw request text (do_raw_request in run-regression-tests.pl),
        # e.g. deliberately malformed or chunked-encoded requests.
        return { __type__ => 'raw', data => encode_base64($req, '') };
    }
    else {
        die "Unsupported request value of type " . ref($req);
    }
}

my @out;
for my $t (@C) {
    my %h = %{ $t || {} };
    my %entry;

    $entry{type} = $h{type} if exists $h{type};
    $entry{comment} = $h{comment} if exists $h{comment};

    if (exists $h{conf}) {
        my $conf = $h{conf};
        if (ref($conf) eq 'CODE') {
            $conf = eval { &$conf };
            die "conf coderef failed in $fn: $@" if $@;
        }
        $entry{conf} = $conf;
    }

    if (exists $h{request}) {
        $entry{request} = serialize_request($h{request});
    }

    for my $key (qw(match_log match_response match_file)) {
        next unless exists $h{$key};
        $entry{$key} = serialize_match_spec($h{$key});
    }

    for my $key (qw(test prerun)) {
        next unless exists $h{$key} and ref($h{$key}) eq 'CODE';
        $entry{unsupported} = $key;
        print STDERR "WARNING: $fn: dropping unsupported '$key' coderef "
            . "(comment: " . ($h{comment} // '?') . ") - needs manual follow-up\n";
    }

    push @out, \%entry;
}

print JSON::PP->new->canonical->utf8->encode(\@out);
