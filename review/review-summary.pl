#!/usr/bin/perl
use strict;
use XML::Simple;
#use Data::Dumper;

my $REVIEW = shift @ARGV;
my %ISSUES = ();

unless (defined $REVIEW) {
	print STDERR "Usage: $0 <review-xml-file> [ <src-file> ... ]\n";
	exit 1;
}

# XML => hashref
my $review = XMLin(
	$REVIEW,
	KeepRoot => 0,
	KeyAttr => { ReviewIssue => "+id" },
	ContentKey => "value",
	SuppressEmpty => undef,
);
#print Dumper($review);

# Reorg hashref to be only open issues by filename
for my $rec (values %{$review->{ReviewIssue} || {}}) {
	my $key = defined($rec->{File}->{value}) ? $rec->{File}->{value} : "";
	push @{$ISSUES{$key}}, $rec if ($rec->{Status} =~ m/\.open$/);
}


# Write report
for my $fn (@ARGV ? (@ARGV) : (keys %ISSUES)) {
	print "File: $fn\n";
  print "===================================================================\n";
	for my $r (sort { $a->{File}->{line} <=> $b->{File}->{line} || $a->{ReviewerId} cmp $b->{ReviewerId} } @{$ISSUES{$fn} || []}) {
		(my $type = $r->{Type}) =~ s/^.*\.([^\.]+)$/$1/;
		$type = ucfirst($type);
		(my $res = $r->{Resolution}) =~ s/^.*\.([^\.]+)$/$1/;
		my $line = ($r->{File}->{line} and $r->{File}->{line} > 1) ? " @ $r->{File}->{line}" : "";
		my $summary = $r->{Summary} ? "\n$r->{Summary}\n\n" : "";
		my $desc = $r->{Description} ? "$r->{Description}\n\n" : "";

		print << "EOT";
[$r->{ReviewerId}] $type$line
$summary$desc
EOT
	}
  print "\n";
}

