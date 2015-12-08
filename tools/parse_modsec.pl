#!/usr/bin/perl

use strict;
use warnings;

use DateTime::Format::Strptime;
use Getopt::Long qw(:config no_ignore_case bundling);
use JSON;
use List::MoreUtils qw(any);
use NetAddr::IP;
use Try::Tiny;

=pod

=head1 NAME

parse_modsec.pl

=head1 SYNOPSIS

Parse ModSecurity logs generated as JSON

=head1 USAGE

Usage: $0 [h] [Htsrfdbalspjv]
    -H|--host             Search rules based on the Host request header
    -t|--transaction-id   Search rules based on the unique transaction ID
    -s|--source-ip        Search rules based on the client IP address (can be presented as an address or CIDR block)
    -r|--rule-id          Search rules based on the rule ID
    -f|--filter           Define advanced filters to walk through JSON tree
    -d|--delim            Define a delimiter for advanced filters. Default is '.'
    -b|--before           Search rules before this timeframe
    -a|--after            Search rules after this timeframe
    -l|--logpath          Define a path to read JSON logs from. Default is '/var/log/modsec_audit.log'
    -S|--stdin            Read rules from stdin instead of an on-disk file
    -p|--partial-chains   Do not prune partial chain matches
    -j|--json             Print rule entries as a JSON blob, rather than nice formatting
    -v|--verbose          Be verbose about various details such as JSON parse failures and log data


=head2 FILTERS

ModSecurity JSON audit logs are written as a series of atomic JSON documents, as opposed to a single, monolithic structure. This program will read through all JSON documents provided, making certain assumptions about the structure of each document, and will print out relevent entries based on the parameters provided. Log entries can be filtered by key-value pairs; given a key at an arbitrary level in the document, test the value of the key against an expected expression. The best way to understand this is with examples (see EXAMPLES for further details).

Filter values are treated as regular expressions. Each match is anchored by '^' and'$', meaning that values that do not contain PCRE metacharacters will essentially match by string equality.

Filters can be used to search a specific key-pair value, or an array of values. Arrays containing sub-elements can also be traversed. Arrays are identified in a filter key expression through the use of the '%' metacharacter. See EXAMPLES for further discussion of filter key expression syntax.

Multiple filters can be provided, and are used in a logical AND manner (that is, an entry must match all given filters).


=head2 FILTER EXAMPLES

Examine the following entry:

	{
		"foo": "bar",
		"foo2": "bar2",
		"qux": {
			"quux": "corge",
			"grault": "garply",
			"wal.do": "fred"
		},
		"baz": [
			"bat",
			"bam",
			"bif"
		],
		"bal": [
			{ "hello": "world" },
			{ "how": "are" },
			{ "you": "doing" }
		]
	}

A search for the top level key "foo" containing the value "bar" would look like:

	-f foo=bar

However, the following will not result in the entry being matched:

	-f foo=bar2

This is because the value of "foo" in the JSON document does not match the regex "^bar2$"

Searching sub-keys is possible by providing the traversal path as the filter key, separated by a delimiter. By default the delimiter is '.'. For example, to search the value of the "grault" subkey within the "qux" key:

	-f qux.grault=<expression>

Search arrays is also possible with the use of the '%' metacharacter, which should be used in place of a key name in the filter expression. For example, to search through all the values in the "baz" top-level key:

	-f baz.%=<expression>

Searching for specific keys that are live in an array is also possible. For example, to search for the value of the "hello" key within the top-level key "bal" array:

	-f bal.%.hello=<expression>

If any key contains a period character (.), you can specify an alternative delimiter using the '-d' option. To search the "wal.do" key within "qux":

	-d @ quz@wal.do=<expression>


=head2 SHORTCUTS

Quick searches of on-disk log files likely will be performed using simple queries. Rather than forcing users to write a filter for common parameters, we provide a few shortcuts as options. These shortcuts can be combined with additional filters for complex searches. Provided shortcuts (and the matching filter key expression) are listed below:

	Host:             request.headers.Host
	Transaction ID:   transaction.transaction_id
	Rule ID:          matched_rules.%.rules.%.actionset.id

Additionally, the '--source-ip' argument allows for searching rule entries based on the remote IP address. This option searches based on CIDR blocks, instead of the filter searching described above.

=head2 TIMEFRAME

Log entries can further be narrowed by time range. The --before and --after flags can be used to return only entries that returned before or after (or both) a given date and time. Values for these options can be provided by the following syntax:

	^\d+[dDhHmM]?$

For example, to limit the search of entries to between one and 4 days ago:

	-a 4d -b 1d

You may provide one, both, or neither of these flags.


=head2 USAGE EXAMPLES

Print all log entries from the default log location:

	parse_modsec.pl

Print all log entries and show more detailed information, such as response data and matched rule details

	parse_modsec.pl -v

Print entries matching a specific source IP:

	parse_modsec.pl -s 1.2.3.4

Print entries matching a source IP in a given subnet:

	parse_modsec.pl -s 1.2.3.0/24

Print entries matching a given host and all its sub domains:

	parse_modsec.pl -H .*example.com

Print entries matching a specific rule ID, that occurred within the last 12 hours:

	parse_modsec.pl -r 123456 -a 12h

Print entries matching a given rule ID, even if that ID was present in a partial chain:

	parse_modsec.pl -r 123456 -p

Print entries that contain an HTTP status code 403

	parse_modsec.pl -f response.status=403

Print entries that contain an HTTP GET request with a 'Content-Length' header

	parse_modsec.pl -f request.headers.Content-Length=.* -f request.request_line=GET.*

=cut

sub usage {
	print <<"_EOF";
Usage: $0 [h] [Htsrfdbalspjv]
	-h|--help             Print this help
	-H|--host             Search rules based on the Host request header
	-t|--transaction-id   Search rules based on the unique transaction ID
	-s|--source-ip        Search rules based on the client IP address (can be presented as an address or CIDR block)
	-r|--rule-id          Search rules based on the rule ID
	-f|--filter           Define advanced filters to walk through JSON tree
	-d|--delim            Define a delimiter for advanced filters. Default is '.'
	-b|--before           Search rules before this timeframe
	-a|--after            Search rules after this timeframe
	-l|--logpath          Define a path to read JSON logs from. Default is '/var/log/modsec_audit.log'
	-S|--stdin            Read rules from stdin instead of an on-disk file
	-p|--partial-chains   Do not prune partial chain matches
	-j|--json             Print rule entries as a JSON blob, rather than nice formatting
	-v|--verbose          Be verbose about various details such as JSON parse failures and log data

	For detailed explanations of various options and example usages, see 'perldoc $0'

_EOF
	exit 1;
}

# figure the number of seconds based on the command-line option
sub parse_duration {
	my ($duration) = @_;

	if ($duration =~ /^(\d+)[dD]$/) {
		return $1 * 60 * 60 * 24;
	} elsif ($duration =~ /^(\d+)[hH]$/) {
		return $1 * 60 * 60;
	} elsif ($duration =~ /^(\d+)[mM]$/) {
		return $1 * 60;
	} elsif ($duration =~ /^(\d+)[sS]?$/) {
		return $1;
	} else {
		die "Couldn't parse duration $duration!\n";
	}
}

# build a DateTime representative of the past
sub build_datetime {
	my ($duration) = @_;

	return if !$duration;
	return DateTime->now()->subtract(seconds => parse_duration($duration));
}

# determine if the log entry occurred within the given timeframe
sub within_timeframe {
	my ($args)    = @_;
	my $entry     = $args->{entry};
	my $before    = $args->{before};
	my $after     = $args->{after};
	my $timestamp = parse_modsec_timestamp($entry->{transaction}->{time});

	return (defined $before ? $timestamp < $before : 1) &&
	       (defined $after  ? $timestamp > $after  : 1);
}

# sigh...
sub parse_modsec_timestamp {
	my ($input) = @_;

	my $format = '%d/%b/%Y:%H:%M:%S -%z';
	my $locale = 'en_US';

	my $strp = DateTime::Format::Strptime->new(
		pattern => $format,
		locale  => $locale,
	);

	return $strp->parse_datetime($input);
}

# figure out if we're reading from a file or stdin
# return a file handle representation of our data
sub get_input {
	my ($args)  = @_;
	my $logpath = $args->{logpath};
	my $stdin   = $args->{stdin};
	my $fh;

	$stdin ?
		$fh = *STDIN :
		open $fh, '<', $logpath or die $!;

	return $fh;
}

# figure if the target address/cidr contains the entry's remote address
sub cidr_match {
	my ($args)    = @_;
	my $entry     = $args->{entry};
	my $target    = $args->{target};
	my $client_ip = $entry->{transaction}->{remote_address};

	return $target ? $target->contains(NetAddr::IP->new($client_ip)) : 1;
}

# given a file handle, return an arrayref representing pertinent rule entries
sub grok_input {
	my ($args)    = @_;
	my $fh        = $args->{fh};
	my $filters   = $args->{filters};
	my $delim     = $args->{delim};
	my $source_ip = $args->{source_ip};
	my $before    = $args->{before};
	my $after     = $args->{after};
	my $partial   = $args->{partial};
	my $verbose   = $args->{verbose};

	my @ref;

	while (my $line = <$fh>) {
		my $entry;

		try {
			$entry = decode_json($line);
		} catch {
			warn "Could not decode as JSON:\n$line\n" if $verbose;
		};

		next if !$entry;

		skim_entry({
			entry   => $entry,
			partial => $partial,
		});

		next if !filter({
			filters => $filters,
			data    => $entry,
			delim   => $delim,
		});

		next if !cidr_match({
			entry  => $entry,
			target => $source_ip,
		});

		next if !within_timeframe({
			entry  => $entry,
			before => $before,
			after  => $after,
		});

		push @ref, $entry;
	}

	return \@ref;
}

# get rid of partial chains and other noise
sub skim_entry {
	my ($args)  = @_;
	my $entry   = $args->{entry};
	my $partial = $args->{partial};
	my $ctr     = 0;

	for my $matched_rule (@{$entry->{matched_rules}}) {
		splice @{$entry->{matched_rules}}, $ctr++, 1
			if $matched_rule->{chain} && !$matched_rule->{full_chain_match} && !$partial;
	}
}

# print entries after filtering and skimming
sub print_matches {
	my ($args)  = @_;
	my $ref     = $args->{ref};
	my $json    = $args->{json};
	my $verbose = $args->{verbose};

	for my $entry (@{$ref}) {
		if ($json) {
			print encode_json($entry) . "\n";
		} else {
			printf "\n%s\n", '=' x 80;

			my $transaction   = $entry->{transaction};
			my $request       = $entry->{request};
			my $response      = $entry->{response};
			my $audit_data    = $entry->{audit_data};
			my $matched_rules = $entry->{matched_rules};

			if ($transaction) {
				printf "%s\nTransaction ID: %s\nIP: %s\n\n",
					parse_modsec_timestamp($transaction->{time}),
					$transaction->{transaction_id},
					$transaction->{remote_address};
			}

			printf "%s\n", $request->{request_line}
				if $request->{request_line};

			if ($request->{headers}) {
				for my $header (sort keys %{$request->{headers}}) {
					printf "%s: %s\n", $header, $request->{headers}->{$header};
				}
			}

			if ($verbose) {
				print join ("\n", @{$request->{body}}) . "\n"
					if $request->{body};

				printf "\n%s %s\n", $response->{protocol}, $response->{status}
					if $response->{protocol} && $response->{status};

				for my $header (sort keys %{$response->{headers}}) {
					printf "%s: %s\n", $header, $response->{headers}->{$header};
				}

				printf "\n%s\n", $response->{body}
					if $response->{body};
			}

			for my $chain (@{$matched_rules}) {
				print "\n";
				my @extra_data;
				my $ctr = 0;

				for my $rule (@{$chain->{rules}}) {
					printf $rule->{is_matched} ? "%s%s\n" : "%s#%s\n", '  ' x $ctr++, $rule->{unparsed};
					push @extra_data, $rule->{actionset}->{msg} if $rule->{actionset}->{msg};
					push @extra_data, $rule->{actionset}->{logdata} if $rule->{actionset}->{logdata};
				}

				printf "\n-- %s\n", join "\n-- ", @extra_data
					if @extra_data && $verbose;
			}

			printf "\n-- %s\n\n", $audit_data->{action}->{message}
				if $audit_data->{action}->{message} && $verbose;

			printf "%s\n", '=' x 80;
		}
	}
}

# filter out rule entries based on given filter definitions
sub filter {
	my ($args)  = @_;
	my $filters = $args->{filters};
	my $data    = $args->{data};
	my $delim   = $args->{delim};

	my $valid_match = 1;

	for my $field (keys %{$filters}) {
		my $args  = {
			field => $field,
			match => $filters->{$field},
			delim => $delim,
			hash  => $data,
		};

		if (!match($args)) {
			$valid_match = 0;
			last;
		}
	}
	return $valid_match;
}

# match a hash element (may be an array of elements) against a given pattern
sub match {
	my ($args) = @_;
	my $delim  = $args->{delim};
	my $hash   = $args->{hash};
	my $match  = $args->{match};
	my $field  = $args->{field};

	my @matches = traverse($args);

	return any { $_ =~ m/^$match$/ } @matches;
}

# walk a JSON structure in search of a given key
# borrowed and butchered from view_signatures.pl
sub traverse {
	my ($args)   = @_;
	my $delim    = $args->{delim};
	my $hash     = $args->{hash};
	my $match    = $args->{match};
	my $field    = $args->{field};
	my @traverse = split /\Q$delim\E/, $field;

	my @values;

	while (my $level = shift @traverse) {
		if ($level eq '%') {
			# match() is called in a list context
			# so if we have a bad filter expression
			# we need to bail in a sensible way
			return () if ref $hash ne 'ARRAY';

			for my $subhash (@{$hash}) {
				my @match = traverse({
					hash  => $subhash,
					delim => $delim,
					match => $match,
					field => join $delim, @traverse,
				});
				push(@values, @match) if @match;
			}
		} elsif (ref $hash eq 'HASH' && defined $hash->{$level}) {
			$hash = $hash->{$level};
		} else {
			$hash = undef;
			last;
		}
	}

	push @values, $hash if defined $hash;
	return ref $hash eq 'ARRAY' ? @{$hash} : @values;
}

# merge any custom-defined filters with shortcut options
sub merge_filters {
	my ($args)  = @_;
	my $filters = $args->{filters};
	my $delim   = $args->{delim};

	my $lookup = {
		host           => [qw(request headers Host)],
		transaction_id => [qw(transaction transaction_id)],
		rule_id        => [qw(matched_rules % rules % actionset id)]
	};

	for my $field (keys %{$lookup}) {
		if (defined $args->{$field}) {
			my $key = build_filter_key({
				elements => $lookup->{$field},
				delim    => $delim,
			});

			$filters->{$key} = $args->{$field};
		}
	}
}

# stub sub to build a filter key
sub build_filter_key {
	my ($args)   = @_;
	my $elements = $args->{elements};
	my $delim    = $args->{delim};

	return join $delim, @{$elements};
}

sub main {
	my (
		$host, $transaction_id, # shortcuts
		$source_ip, $rule_id,   # shortcuts
		%filters, $delim,       # used by filters/match/traverse to grok the input
		$before, $after,        # timeframe
		$logpath, $stdin,       # input
		$partial_chains, $json, # output
		$verbose,               # output
		$fh, $parsed_ref,       # data structures
	);

	GetOptions(
		'h|help'             => sub { usage(); },
		'H|host=s'           => \$host,
		't|transaction-id=s' => \$transaction_id,
		's|source-ip=s'      => \$source_ip,
		'r|rule-id=i'        => \$rule_id,
		'f|filter=s'         => \%filters,
		'd|delim=s'          => \$delim,
		'b|before=s'         => \$before,
		'a|after=s'          => \$after,
		'l|logpath=s'        => \$logpath,
		'S|stdin'            => \$stdin,
		'p|partial-chains'   => \$partial_chains,
		'j|json'             => \$json,
		'v|verbose'          => \$verbose,
	) or usage();

	# sanity checks
	die "Cannot parse both a file and stdin\n"
		if defined $logpath && defined $stdin;

	if (defined $source_ip) {
		$source_ip = NetAddr::IP->new($source_ip);
		die "Invalid IP/CIDR provided for source IP argument\n"
			unless $source_ip;
	}

	# build_datetime will bail out if an invalid format was given
	$before = build_datetime($before);
	$after  = build_datetime($after);

	# figure where we're reading from
	$logpath ||= '/var/log/mod_sec/modsec_audit.log';
	$fh = get_input({
		logpath => $logpath,
		stdin   => $stdin,
	});

	die "Could not get a handle on your data\n"
		unless $fh;

	# build the filters by merging shortcut options with custom filter directives
	$delim ||= '.';
	merge_filters({
		filters        => \%filters,
		host           => $host,
		transaction_id => $transaction_id,
		source_ip      => $source_ip,
		rule_id        => $rule_id,
		delim          => $delim,
	});

	# walk through our input, getting an arrayref of valid entries based on filters and timeframe
	$parsed_ref = grok_input({
		fh        => $fh,
		filters   => \%filters,
		delim     => $delim,
		source_ip => $source_ip,
		before    => $before,
		after     => $after,
		partial   => $partial_chains,
		verbose   => $verbose,
	});

	close $fh || warn $!;

	# show me the money!
	print_matches({
		ref     => $parsed_ref,
		json    => $json,
		verbose => $verbose,
	});
}

main();
