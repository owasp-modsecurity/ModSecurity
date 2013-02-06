#!/opt/local/bin/perl
#
# Copyright (C) 2006-2011 Trustwave All rights reserved.
#
# The OWASP ModSecurity Core Rule Set is distributed under 
# Apache Software License (ASL) version 2
# Please see the enclosed LICENCE file for full details.#
# For Internal Use only!
#
# Originally writtern by Ofer Shezaf
#

# !! todo:
# !! ~ request for URI command in conf file
# !! ~ Ensure headers terminators
# !! read rulesets config file for event mane, policy and patterns
# !! fuz patterns from config file
# !! %include directive

use strict;
#use warnings;
#use diagnostics;
use IO::File;
use IO::Socket;
use IO::Select;
use HTTP::Request;
use HTTP::Response;
use Safe;
use Storable qw(dclone);
use Getopt::Long;
use Pod::Usage;

# -- Add library
use FindBin qw($Bin $Script);
use lib "$Bin";

use Data::Dumper;
autoflush STDOUT;

# -- consts
our $SKELETON_REQUEST = <<END_SKEL
GET \$URI HTTP/1.0
Host: local
User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.0.7) Gecko/20060909 Firefox/1.5.0.7
Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5
Accept-Language: en-us,en;q=0.5
Accept-Encoding: gzip,deflate
Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7

END_SKEL
;


# -- get options
my $global_state = { 'timeout' => '2', 'fuzz' => 1, 'vars' => {}, 'port' => 80 };
$global_state->{'global'} = $global_state;

GetOptions
(
    $global_state,
    'server|s:s',
    'hostname:s',
    'port|p:s',
    'timeout|t:f',
    'mslog:s',
    'msdebug:s',
    'o:s',
    'i=s@',
    'run:s@',
    'from:s',
    'relevant|r!',
    'fuzz|f!',
    'clean!',
    'check!',
    'verbose|v!',
    'help|h|?',
    'man'
) || pod2usage (-exitstatus => 0, -verbose => 0);
 
pod2usage(-exitstatus => 1, -verbose => 1) if $global_state->{'help'};
pod2usage(-exitstatus => 1, -verbose => 2) if $global_state->{'man'};

push @{$global_state->{'input'}}, @ARGV;
pod2usage (2) if $#{$global_state->{'input'}} < 0;

# -- get list of test files
my $testfiles = [];
my ($progname) = ($Script =~ /(.*)\..*$/);
if (-e "$progname.conf") {
    push @$testfiles, "$progname.conf";
}
foreach my $arg (@{$global_state->{'input'}}) {
    push @$testfiles, glob $arg;
}

foreach my $file (@$testfiles) {
    if (!-e $file) {
	print STDERR "Error 101: test file $file not found\n";
	exit;
    }
}

my ($outfile, $outfilename);
if ($global_state->{'output'}) {
    $outfile = new IO::File ">$global_state->{'output'}";
    if (!$outfile) {
	print STDERR "Error 106: unable to create report file $global_state->{'output'}. $@\n";
	exit;
    }
    $outfilename = $global_state->{'output'};    
}
else {
    $outfile = *STDOUT;
    $outfilename = 'STDOUT';
}

report_header($outfile, $outfilename);

foreach my $filename (@$testfiles) {
    parse_test_file ($outfile, $filename, $global_state);
}
exit (0);

# -- read an input file and execute tests in it
sub parse_test_file
{
    my ($outfile, $filename, $parent_state) = @_;
    my $file_state = inherit_state ($parent_state);

    report_file_header($outfile, $filename);

    my $linenumber = 0;
    my $testfile = new IO::File "<$filename";
    if (!$testfile) {
	print STDERR "Error 105: unable to open tests file $filename. $@\n";
	print $outfile "unable to open file";
	return;
    }

    
    my $state = $file_state;
    while (defined(my $line=<$testfile>)) {     
        $linenumber++;
        $line = tchomp ($line);
        $state = parse_test_line ($line, $state, $testfile);
        if (!ref $state) {
            print STDERR "$state in file $filename at line $linenumber\n";
            print STDERR "line: $line\n" if $parent_state->{'check'};
            
            return;
        }
        while (my $test = shift @{$file_state->{'tests'}}) {
            run_test ($outfile, $test, $filename);
        }
    }
    run_test ($outfile, $state, $filename) if $state->{'name'};
}


# -- parse the next input line
sub parse_test_line
{
    my ($line, $state, $file) = @_;
    
    # -- Handle EOF
    return $state unless defined $line;
    
    # -- Hande multi line remarks
    if ($state->{'multi_line_cmd'} eq "remark") {
        undef $state->{'multi_line_cmd'} if ($line =~ /^\%endremark/i);
        return $state;
    }

    # -- Handle multi line directives
    if (my $incmd = $state->{'multi_line_cmd'}) {
        
        # -- Request parser
        if ($incmd =~ /^request$/i) {
            if (my ($len) = $line =~ /^Content-Length: (\d+)$/) {
                $state->{'request_len'} = $len;
            } elsif ($state->{'request_state'} eq 'headers' && $line =~ /^$/) {
                $state->{'request_state'} = 'body';
                $state->{'multi_line_value'} .= "$line\x0D\x0A";
                if (defined $state->{'request_len'}) {
                    my $result = read $file, my $buffer, $state->{'request_len'};
                    return "Error 110: Error reading file" if !defined $result;
                    return "Error 111: File terminated unexpectedly (read $result char of required $state->{'request_len'})" if $result != $state->{'request_len'};
                    #print "==>$state->{'multi_line_value'}<==\n$buffer\n----\n";
                    $state->{'multi_line_value'} .= $buffer;
                    return $state;
                    undef $state->{'request_len'};
                }
            }
        }

        # X-Real-Content-Length:
        # -- Append to value if not yet next directive
        if ($line !~ /^\%/) {
            $state->{'multi_line_value'} .= "$line\x0D\x0A";
            return $state;
        }
        
        # -- Otherwise use directive
        $state = use_test_directive ($state, $incmd, $state->{'multi_line_value'}, $state->{'multi_line_global'});
        return $state if (!ref $state);
        undef $state->{'multi_line_cmd'};
        undef $state->{'multi_line_value'};
        undef $state->{'request_len'};
    }
    
    # -- Handle empty lines and single line remarks
    return $state if $line =~  /^\s*(\#|$)/;
    
    # -- Parse directive
    my ($global);
    $line =~ /^\%(\w+)\s*(.*)?$/;
    my ($cmd, $operand) = ($1,$2);
    if ($cmd =~ /^global$/i) {
        $global = 1;
        ($cmd, $operand) = ($operand =~ /^\s*(\w+)\s*(.*)?$/);
    }
    if (!$operand) {
        $operand = 1;
        if ($cmd =~ /^no(.*)$/) {
            $cmd = $1;
            $operand = 0;
        }
    }
    $cmd = lc $cmd;
    
    # -- Start multi line directives
    if ($cmd =~ /^(?:request|remark)$/i) {
        $state->{'multi_line_cmd'} = $cmd;
        $state->{'multi_line_global'} = $global;
        return $state;
    }
    
    return use_test_directive ($state, $cmd, $operand, $global);
}

sub use_test_directive
{
    my ($state, $cmd, $operand, $global) = @_;

    # -- Simple directives
    if ($cmd =~ /^(?:server|port|hostname|timeout|verbose|relevant|mslog|msdebug|request|uri|request|fuzz|clean|pause)$/i) {
        if ($global) {
            $state->{'global'}->{$cmd} = $operand;
        }
        $state->{$cmd} = $operand;
        $state->{'request_state'} = 'headers';
    }
    
    # -- List directives
    elsif ($cmd =~ /^(?:status|remote_event|event|audit|output)$/i) {
        push_state ($state, $state->{'global'}, $cmd, $global, $operand);
    }

    # -- Variable assignment
    elsif ($cmd =~ /^(?:var)$/i) {
        my ($var, $values) = ($operand =~ /\s*(\w+)\s*=\s*?(.*)/);
        my @values = split /\s*,\s*/, $values;
        push_state ($state->{'vars'}, $state->{'global'}->{'vars'}, $var, $global, @values);
    }

    # -- End test (return to file context)
    elsif ($cmd =~ /endtest/i) {
        if ($state->{'name'}) {
            push @{$state->{'parent'}->{'tests'}}, $state;
        }
        else {
            return "Error 107: %endtest directive without a preceding %test directive";
        }
        $state = $state->{'parent'};
    }
    
    # -- New test (end test and start a new one)
    elsif ($cmd =~ /test/i) {
        if ($state->{'name'}) {
            push @{$state->{'parent'}->{'tests'}}, $state;
            $state = inherit_state ($state->{'parent'});
        }
        else {
            $state = inherit_state ($state);
        }
        $state->{'name'} = $operand;
    }
    
    # -- error
    else {
        return "Error 102: syntax error";
    }
    
    return $state;
}


sub reconfigure
{
    my ($state) = @_;
    
    my ($restart) = 0;
    if ($state->{'clean'}) {
        unlink $state->{'mslog'} if $state->{'mslog'};
        unlink $state->{'msdebug'} if $state->{'msdebug'};
        $restart = 1;
        global_clear ($state, 'clean');
    }
    if ($restart) {
        print "## Restarting apache\n";
        print STDERR `/usr/local/apache/bin/apachectl restart`;
        sleep (1);
    }
}

sub inherit_state
{
    my ($state) = @_;
    my $clone = dclone $state;
    $clone->{'parent'} = $state;
    $clone->{'global'} = $state->{'global'};
    delete $clone->{'tests'};
    return $clone;
}

# -- Add values to key in state taking into about both overriding and global
sub push_state
{
    my ($hash, $global_hash, $key, $global, @values) = @_;
    if ($global) {
        push @{$global_hash->{$key}}, @values;
    }
    elsif (!$hash->{"_OVERRIDE_$key"}) {
        $hash->{$key} = [];
    }
    $hash->{"_OVERRIDE_$key"} = 1;
    push @{$hash->{$key}}, @values;
}

sub global_clear
{
    my ($state, $key) = @_;
    while ($state) {
        undef $state->{$key};
        $state = $state->{'parent'};
    }
}

sub run_test
{
    my ($outfile, $state, $file) = @_;

    return if $state->{'check'};
    if ($state->{'from'}) {
        return if $state->{'name'} !~ /$state->{'from'}/;
    }
    global_clear ($state, 'from');
    
    my $do_test = $#{$state->{'run'}} < 0;
    foreach my $select (@{$state->{'run'}}) {
        $do_test ||= ($state->{'name'} =~ /$select/);
    }
    return if !$do_test;
    
    if ($state->{'request'} && $state->{'uri'}) {
        print STDERR "Error 103: cannot use both %request and %uri in test $state->{'name'} in file $file\n";
	exit;
    }
    reconfigure($state);
    if ($state->{'uri'}) {
	$state->{'request'} = $SKELETON_REQUEST;
	$state->{'request'} =~ s/\$URI/$state->{'uri'}/;
    }
    my $requests = $state->{'fuzz'} ?
        generate_vectors ($state->{'request'}, $state->{'vars'}, $state->{'verbose'}) :
        {'' => $state->{'request'}};
    VECTOR: while (my ($vars, $request) = each %$requests)
    {
        my $test = inherit_state ($state);
        $test->{'request'} = $request;
        if ($test->{'mslog'}) {
    	    my $output = `wc $test->{'mslog'}`;
            $output =~ /\s*(\d+)/;
	    $test->{'mslog_start'} = $1;
        }
	if ($test->{'msdebug'}) {
    	    my $output = `wc $test->{'msdebug'}`;
            $output =~ /\s*(\d+)/;
	    $test->{'msdebug_start'} = $1;
	}
	my ($server, $port) = ($test->{'server'}, $test->{'port'});
        if (!$port && ($server =~ /^(.+)\:(\d+)$/)) {
	    $server = $1;
	    $port = $2;
	}
        if ($test->{'hostname'}) {
            my $hostname = ($test->{'hostname'});
        }
        my $sock = IO::Socket::INET->new(PeerAddr => $server, PeerPort => $port);
        if (!$sock) {
            print STDERR "Error 104: error connecting to server $server. $@\n";
            exit;
        }
     
        print $sock $request;
        my $line;
        do {
            my @ready;
            @ready = IO::Select->new($sock)->can_read($test->{'timeout'});
            if ($#ready < 0) {
                $test->{'response'} = $test->{'response_status'} = "N/A";
                report_test ($outfile, 'TIMEOUT', $test, $request, $vars);
                next VECTOR;
            }
        
            if (defined($line = <$sock>)) {
                $test->{'response'} .= $line;
                if (!$test->{'response_status'}) {
                    if ($line =~ /^HTTP\S*\s+(\d+)/) {
                        $test->{'response_status'} = $1;
                    }
                    elsif ($line =~ /<title>400 Bad Request<\/title>/) {
                        $test->{'response_status'} = 400;
                    }
                }
            }
        } while (defined($line));
	
        if ($test->{'mslog'}) {
            my $output = `wc $test->{'mslog'}`;
            $output =~ /\s*(\d+)/;
            my $lines = $1 - $test->{'mslog_start'};
	    $test->{'mslog'} = `tail -n $lines $test->{'mslog'}`;
	}

        if ($test->{'msdebug'}) {
            my $output = `wc $test->{'msdebug'}`;
            $output =~ /\s*(\d+)/;
            my $lines = $1 - $test->{'msdebug_start'};
	    $test->{'msdebug'} = `tail -n $lines $test->{'msdebug'}`;
	}

	$test->{'match_status'} = check_match ($test->{'response_status'}, $test->{'status'});	
	$test->{'match_output'} = check_match ($test->{'response'}, $test->{'output'});
	$test->{'match_audit'} = !$test->{'mslog'} || check_match ($test->{'mslog'}, $test->{'audit'});
        my $test_events;
        foreach my $event (@{$test->{'event'}}) {
            if ($event =~ /^\!(.*)$/) {
                push @$test_events, "!\\[id \\\"$1\\\"\\]"
            }
            else {
                push @$test_events, "\\[id \\\"$event\\\"\\]"
            }
        }
	$test->{'match_events'} = !$test->{'mslog'} || check_match ($test->{'mslog'}, $test_events);
        my $result =
            ($test->{'match_status'}
            && $test->{'match_output'}
            && $test->{'match_audit'}
            && $test->{'match_events'}) ? "OK" : "FAIL" ;
	report_test ($outfile, $result, $test, $request, $vars);
        sleep $test->{'pause'} if $test->{'pause'};
    }
}

sub check_match
{
    my ($text, $patterns) = @_;
    my $match = 1;
    foreach my $pattern (@$patterns) {
        if ($pattern =~ /^\!(.*)$/) {
            return 0 if $text =~ /$1/sm;
        }
        else {
            return 0 if $text !~ /$pattern/sm;
        }
    }
    return $match;
}

sub report_header
{
    my ($outfile, $outfilename) = @_;
    print $outfile "\nModSecurity rules test report generated to $outfilename on " . localtime() . "\n";
    print $outfile "Produced by rulestest.pl, (c) Trustwave Holdings Inc, 2012\n";
}

sub report_file_header
{
    my ($outfile, $filename) = @_;
    print $outfile "\n## reading tests file $filename\n";
}

sub report_test
{
    my ($outfile, $result, $test, $request, $vars) = @_;

    
    print $outfile "\n" if $result ne "OK";
    print $outfile "$result: ";
    print $outfile "$test->{'name'}";
    print $outfile " ($vars)" if $vars;
    print $outfile ", status = $test->{'response_status'}";
    #print $outfile ", X-WAF-Event Match" if ($test->{'match_output'});
    my (@events) = ($test->{'mslog'} =~ /\[id \"(\d+)\"\]/gim);
    print $outfile $#events < 0 ? ", no events received" : ", event(s) = " . (join ",", @events) ;
    if ($result eq "FAIL") {
	print $outfile "\n";
	if (!$test->{'match_status'}) {
	    print $outfile "Expected status code(s): " . (join ",", @{$test->{'status'}}) . "\n";
	}
	if (!$test->{'match_events'}) {
	    print $outfile "Expected event(s): " . (join ",", @{$test->{'event'}}) . "\n";
	}
	if (!$test->{'match_audit'}) {
	    print $outfile "Audit does not match\n";
	}
	if (!$test->{'match_output'}) {
	    print $outfile "Output does not match\n";
	}
	#$test->{'match_events'} && print "Events: $test->{'response_status'} and not " . (join ",", $test->{'status'}) . "\n";
	print_details ($test) if $test->{'verbose'} || $test->{'relevant'};
    }
     print $outfile "\n";
     print_details ($test) if $test->{'verbose'};
}
    
    
sub print_details
{
    my ($test) = @_;
    print $outfile "---------\nRequest:\n$test->{'request'}\n";
    print $outfile "---------\nResponse:\n$test->{'response'}\n";
    print $outfile "---------\nLog:\n$test->{'mslog'}\n" if ($test->{'mslog'});
    print $outfile "---------\nDebug:\n$test->{'msdebug'}\n" if ($test->{'msdebug'});
}    

sub generate_vectors
{
    my ($script, $vars, $verbose) = @_;
    my $test_requests = [];

    my $vectors  = [ {} ];
    while (my ($var, $values) = each %$vars) {
        next if $var =~ /^_OVERRIDE_/;
        next if $script !~ /\$$var\b/;
        foreach my $vector (@$vectors) {
            $vector->{$var} = $values->[0];
        }
        if ($#$values > 0) {
            my $collect_vectors = [];
            shift @$values;
            foreach my $value (@$values) {
                my $new_vectors = dclone $vectors;
                foreach my $vector (@$new_vectors) {
                    $vector->{$var} = $value;
                }
                push @$collect_vectors, @$new_vectors;
            };
            push @$vectors, @$collect_vectors;
        }
    }

    $script =~ s/\$([a-zA-Z_]+)/\$vector->{$1}/g;
    #print "SCRIPT=>$script\n";
    my $results;
    foreach our $vector (@$vectors) {
        my $var = join ",", map { "$_=$vector->{$_}" } keys %$vector;
        $vector->{'CONTENT_LENGTH'} = '$CONTENT_LENGTH';
        my $result;
        if (!defined($result = eval_expression ($script, $vector, $verbose))) {
            print STDERR "Error 109: unable to fuzz request. Not fuzzing test.\n";
            return ({'' => $script});
        }
        #my $req = HTTP::Request->parse($result);
        my ($content) = $result =~ /.*?\x0D\x0A\x0D\x0A(.*)/sm;
        $vector->{'CONTENT_LENGTH'} = length $1;
        $result = eval_expression ($script, $vector, $verbose);
        $results->{$var} = $result;
    }
    return $results;
}


sub eval_expression
{
    my ($script, $vector, $verbose) = @_;
    
    $script =~ s/([\"\@\%])/\\$1/g;
    my $result;
    my $warn;
    local $SIG{__WARN__} = sub { $warn = $_[0] };
    eval {
        my $safe = new Safe;
        $safe->share ('$vector');    
        $result = $safe->reval ("return \"$script\"");
    };
    if ((my $error = $@) || $warn) {
        print STDERR "Error 108: unable to evaluate expression\n";
        print STDERR "SCRIPT: $script\n" if $verbose;
        print STDERR "EVAL ERROR: $error\n" if $error && $verbose;
        print STDERR "EVAL WARNING: $warn\n" if $warn && $verbose; 
        return undef;
    }
    return $result;
}

sub tchomp {
    my ($text) = @_;
    $text =~ s/^(.*?)(?:\x0D\x0A|\x0A|\x0D|\x0C|\x{2028}|\x{2029})/$1/s;
    return $text;
}



__END__

=head1 NAME

rulestest.pl

=head1 SYNOPSIS

rulestest.pl [options] [test files ...]

This program reads and executed tests in input test file(s) agains a
ModSecurity protected web application.

use -help for options.
use -man for detailed usage information.

=head1 OPTIONS

the following options can be used either on the command line or (using the
long version) as directives (prefixed by %) in test files.

    -s or -server <address>[:<port>]
	address of server to send. Mandatory before any test, but can appear
        in the test files themselves

    -p or -port <port>
	port to send tests to, defaults to 80 

    -t or -timeout <time>
        time in seconds, possibly fractional, to wait for server response.
        If the server does not respond within this period the test fails.
        the default is 10 seconds.
        
        Timeout should be small for synthetic tests, such as those
        generated from capture files as the server would respond fast.
        The timeout may need to be longer for real world servers.

    -f or -fuzz
        Whether to use fuzzing or not. You may not want to use fuzzing in
        case the requests where generated automatically and may includes
        syntax that will be considered by rulestest as substitutable
        variables.

    -mslog <file name>
	ModSecurity log file to search for events in. If not specified
	events are not (useful if tests are not run locally).

    -msdebug <file name>
	ModSecurity debug file to extract debug information
	to test report. If not specified, debug information is not
	add to the report.

    -o <file name>
	name of output file. Defaults to STDOUT. Not relevant as directive
        in test files.

    -i <file name>
	Names of input files. can also appear as parameters on the command
        line. Not relevant as directive in test files.
        
    -check
        Does not run test but only parses the input file

    -run <regular expression>
        a regular expresion to select tests to perfrom. Only tests whose
        name match the regular expression are executed. The option
        (or directive) can be used multiple times, so a test matching
        any of the regular expressions will be executed.
        
    -from <regular expression>
        a regular expression selecting the first test to perform.

    -r or -relevant
	Detailed information in the test report in case
	of a test failure.

    -v or -verbose
	Detailed information for all tests. Verbose will also cause specific
        errors to include print more information.
        
    -c or -clean
        deletes log and debug files and restart apache (using apachctl).
        Significantly enhance performance of the tests and can be used as
        many times as needed in test files.
        
        Clean is executed once, when starting the 1st test after it is
        defined regardless of the scope it is defined at. Specifically
        it will remove the log and debug files as defined when the test
        start: this enables the use of -clean on the command line even
        though file locations are defined only later on, for example in
        rulestest.conf.
        

=head1 INSTALLATION & CONFIGURATION

Test should be run from the same host ModSecurity runs on, or a computer that 
has file system access to ModSecurity audit log to. This allows rulestest to
examine ModSecurity audit log for events and extract information from
ModSecurity debug log to the test report.

In order to test for events, ensure that ModSecurity is set use serial logging.

=head2 Local and Global Settings:

When used in a file, directives are local to the file, and when used whithin
a test they are local to a test. To specify global settings preced the directive
wiht the keyword global:

%global server 127.0.0.1:80

if a file with the name rulestest.conf exists in the same directory as the
script, it will be read. I can contain any directive valid in a test file.
It can be used to set default

=head2 Binary Attrbiutes:

Directives that except a yes/no value can be set in varios ways. Providing the
value 0 or 1 will set them to no and yes respectively. The directive without
any values is eqvivalent to setting it to 1, and the directive preceded by "no"
is eqvivalent to 0, for example:

%noverbose

will set the current scope to not report verbosely.

=head2 Default Settings:

The file rulestest.conf is automatically read by rulestest.pl before any
tests file and may contain global setup directives. You may especially want to
set there settings such as %server, %mslog and %msdebug as well as reporting
level using %verbose and %relevant.

=head1 WRITING TESTS

To write a test use the following directives:

=head2 defining the test request

    %test <name> -
        starts a test and set is name as shown in the report
        
    %endtest -
        used to terminate a test. Ususally there is no need to use this
        directive as the next %test directive implicitly defines the end of
        a test. You may want to use it if you want to set additional file
        level settings for the remaining tests.
        
    %remark -
        Ignore all lines (including directives) until a matching %endremark
        directive. use # at the beginning of a line to add a remark line to
        the file, if not in the middle of a multi-line directive such as
        %request.

    %request -
        multiple lines of the request should appear on the lines follwing
	the directive terminated by the next =directive (a line starting
	with "%"). Do not forget to leave an empty line as required by
	HTTP.
        
        You can use the special variable $CONTENT_LENGTH to have
	rulestest set the correct content length for the request.     
        $CONTENT_LENGTH can save counting, but its main use is to enable
        fuzzing of requests with variables in the post data.

    %uri -
        a uri to send to the server. it would be embedded in a
	standard request
        
    %pause -
        define a delay in seconds after the test and before the next test.
        Useful if the feature tested involves timeouts.

either a %uri or a %request directive must appeat in a test. A %request or a
%uri can include variables using perl notation ($varname). this would be
replaced when testing with a value set by the %var directive.

Empty lines are skipped if not in the middle of multi-line directives such
as %request.

=head2 defining expected output

    %status <regexp> -
        The expected response status code(s).

    %event <regexp> -
        A regexp that should match event ids generated by the test in
	the audit log.

    %audit <regexp> -
        A regexp that should match in the audit log of the test.

    %output <regexp> -
        A regexp that should match in set a string to search in the HTTP
	response. You can use multiple directives to define many required
	patterns.

for %event, %audit and %output you can use multiple directives to define
many required patterns. All of them must match for the rule to match. Use the
regular expresion or (|) option to check for at least one option from a group
of patterns.

Each regular expression can be preceded by a "!" mark to negate the test. the
regular expression following must not appear in the test result. 

=head1 REPORTING

By default rulestest will provide brief message describing if the test succeded
in any of the checks done: status code, events generated, pattern in audit log
and pattern in response.

the following directives allow control on the level of details of the report:

    %verbose -
        from the test for which the directive appears onward, output request,
	reply and new ModSecurity audit log lines for each test. set to 0 to
	stop (1 is implicit on set).

    %relevant -
	from the test for which the directive appears onward, output verbose
	output for tests that failed any check. set to 0 to stop (1 is
	implicit on set).

In most cases, you will only be interested in the failed tests. In that case,
you can use awk with the following command:
gawk '$1=="OK:" {printme=0}; $1=="FAIL:" {printme=1}; $1=="##" {printme=1}; printme==1 {print}'

=head1 VARIABLE SUBSTITUTION (FUZZING)

The directive "%var variable=value[, value[, valueM-^E..]] sets values for a
variable which are embedded in the request sent. The test would be repeated
using every value. Values are set only for the current test. Use the
%globalvar directive to set global variables.

Multiple %var directives for the same variable add values to the list and do 
not replace values, so:

%var variable=value1

%var variable=value2

Would test with both value1 and value2.

If multiple variables are used in the same test, than the test is carried for 
each combination of values of the variables:

%var var1=v1, v2

%var var3=v3, v4

The test would be repeated 4 times with the test vectors (v1, v3), (v1, v4), 
(v2, v3), (v2, v4).

As noted before, the special variable $CONTENT_LENGTH can be used to
automatically calculate the content length based on the actually generated
request after variable substitution.

=head1 TESTING RESPONSES

In order for outbound tests the script testserver.cgi has to be installed in
the web server's /cgi-bin directory.

To force response content in request, use /cgi-bin/testserver.cgi as the target 
URL and add one or more of the following headers to the reuqest:  

Response-Status: - Force a response status line. Defaults to "200 OK".

Response-Content: - Adds the string to the response. Note that this would not be
the entire response.

Response-Content-Type: - sets the value of the content type header, defaults to 
"text/html"

Response-Header-Name: - Adds a header to the response. This defined the new
header's name. Response-Header-Value defines the header's value.

Response-Header-Value: - The value of the new header defined by the request
header Response-Header-Name. Note: If Response-Header-Name is empty, then this
parameter will be ignored.  

=head1 ERRORS

    Error 101:
	test file <file> not found. Check that all options are valid and no
	option was considered a test file.

    Error 102:
	syntax error in file <file> on line <line>. a line which is not
	a remark, not a directive and not in any multiline section (request
	and multi line remark) was found at specified line and file.

    Error 103:
	cannot use both %request and %uri. Only one of these directive can
	be specified in each test.

    Error 104:
	error connecting to server. The specific error is also displayed.
	This error usually implies a communication problem or specificaiton
	of a wrong server or port.

    Error 105:
	Error occured when trying to open a tests file. Tests will continue
	with next tests file.
	
    Error 106:
	Error occured when trying to create report file.
        
    Error 107:
        %endtest directive without a preceding %test directive

    Error 108:
        The expression evulator (using Perl eval function) failed. The
        expression probably includes some Perl syntax. use -verbose to
        print the actual error returned. 
        
    Error 109:
        Fuzzing the request failed. This probably implies that the test
        request includes some Perl syntax. You may want to use the nofuzz
        option to overcome the problem.


=cut
