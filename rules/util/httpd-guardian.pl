#!/usr/bin/perl -w
#
# httpd-guardian - detect DoS attacks by monitoring requests
# Apache Security, http://www.apachesecurity.net
# Copyright (C) 2005 Ivan Ristic <ivanr@webkreator.com>
#
# $Id: httpd-guardian,v 1.6 2005/12/04 11:30:35 ivanr Exp $
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 2.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#

# This script is designed to monitor all web server requests through
# the piped logging mechanism. It keeps track of the number of requests
# sent from each IP address. Request speed is calculated at 1 minute and
# 5 minute intervals. Once a threshold is reached, httpd-guardian can
# either emit a warning or execute a script to block the IP address.
#
# Error message will be sent to stderr, which means they will end up
# in the Apache error log.
#
# Usage (in httpd.conf)
# ---------------------
#
# Without mod_security, Apache 1.x:
#
#   LogFormat "%V %h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\" %{UNIQUE_ID}e \"-\" %T 0 \"%{modsec_message}i\" 0" guardian
#   CustomLog "|/path/to/httpd-guardian" guardian
#
# or without mod_security, Apache 2.x:
#
#   LogFormat "%V %h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\" %{UNIQUE_ID}e \"-\" %T %D \"%{modsec_message}i\" 0" guardian
#   CustomLog "|/path/to/httpd-guardian" guardian
#
# or with mod_security (better):
#
#   SecGuardianLog "|/path/to/httpd-guardian"
#
# NOTE: In order for this script to be effective it must be able to
#       see all requests coming to the web server. This will not happen
#       if you are using per-virtual host logging. In such cases either
#       use the ModSecurity 1.9 SecGuardianLog directive (which was designed
#       for this very purpose).
#
#
# Usage (with Spread)
# -------------------
#
# 1) First you need to make sure you have Spread running on the machine
#    where you intend to run httpd-guardian on.
#
# 2) Then uncomment line "use Spread;" in this script, and change
#    $USE_SPREAD to "1".
#
# 3) The default port for Spread is 3333. Change it if you want to
#    and then start httpd-guardian. We will be looking for messages
#    in the Spread group called "httpd-guardian".

# TODO Add support to ignore certain log entries based on a
#      regex applied script_name.
#
# TODO Warn about session hijacking.
#
# TODO Track ip addresses, sessions, and individual users.
#
# TODO Detect status code anomalies.
#
# TODO Track accesses to specific pages.
#
# TODO Open proxy detection.
#
# TODO Check IP addresses with blacklists (e.g.
#      http://www.spamhaus.org/XBL/).
#
# TODO Is there a point to keep per-vhost state?
#
# TODO Enhance the script to tail a log file - useful for test
#      runs, in preparation for deployment.
#
# TODO Can we track connections as Apache creates and destroys them?
#
# TODO Command-line option to support multiple log formats. E.g. common,
#      combined, vcombined, guardian.
#
# TODO Command-line option not to save state
#

use strict;
use Time::Local;
# SPREAD UNCOMMENT
# use Spread;


# -- Configuration----------------------------------------------------------

my $USE_SPREAD = 0;
my $SPREAD_CLIENT_NAME = "httpd-guardian";
my $SPREAD_DAEMON = "3333";
my $SPREAD_GROUP_NAME = "httpd-guardian";
my $SPREAD_TIMEOUT = 1;

# If defined, execute this command when a threshold is reached
# block the IP address for one hour.
# $PROTECT_EXEC = "/sbin/blacklist block %s 3600";
# $PROTECT_EXEC = "/sbin/samtool -block -ip %s -dur 3600 snortsam.example.com";
#my $PROTECT_EXEC;

# For testing only:
my $PROTECT_EXEC = "/usr/bin/logger Possible DoS Attack from %s";

# Max. speed allowed, in requests per
# second, measured over an 1-minute period
#my $THRESHOLD_1MIN = 2; # 120 requests in a minute

# For testing only:
my $THRESHOLD_1MIN = 0.01;

# Max. speed allowed, in requests per
# second, measured over a 5-minute period
my $THRESHOLD_5MIN = 1; # 360 requests in 5 minutes

# If defined, httpd-guardian will make a copy
# of the data it receives from Apache
# $COPY_LOG = "";
my $COPY_LOG;

# Remove IP address data after a 10-minute inactivity
my $STALE_INTERVAL = 400;

# Where to save state (at this point only useful
# for monitoring what the script does)
my $SAVE_STATE_FILE = "/tmp/httpd-guardian.state";

# How often to save state (in seconds).
my $SAVE_STATE_INTERVAL = 10;

my $DEBUG = 0;


# -----------------------------------------------------------------

my %months = (
    "Jan" => 0,
    "Feb" => 1,
    "Mar" => 2,
    "Apr" => 3,
    "May" => 4,
    "Jun" => 5,
    "Jul" => 6,
    "Aug" => 7,
    "Sep" => 8, 
    "Oct" => 9,
    "Nov" => 10,
    "Dec" => 11
);

# -- log parsing regular expression


# 127.0.0.1 192.168.2.11 - - [05/Jul/2005:16:56:54 +0100]
# "GET /favicon.ico HTTP/1.1" 404 285 "-"
# "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.8) Gecko/20050511 Firefox/1.0.4"
# - "-" 0 0 "-" 0

my $logline_regex = "";

# hostname
$logline_regex .= "^(\\S+)";
# remote host, remote username, local username
$logline_regex .= "\\ (\\S+)\\ (\\S+)\\ (\\S+)";
# date, time, and gmt offset
$logline_regex .= "\\ \\[([^:]+):(\\d+:\\d+:\\d+)\\ ([^\\]]+)\\]";
# request method + request uri + protocol (as one field)
$logline_regex .= "\\ \"(.*)\"";
# status, bytes out
$logline_regex .= "\\ (\\d+)\\ (\\S+)";
# referer, user_agent
$logline_regex .= "\\ \"(.*)\"\\ \"(.*)\"";
# uniqueid, session, duration, duration_msec
$logline_regex .= "\\ (\\S+)\\ \"(.*)\"\\ (\\d+)\\ (\\d+)";
# modsec_message, modsec_rating
$logline_regex .= "\\ \"(.*)\"\\ (\\d+)";

# the rest (always keep this part of the regex)
$logline_regex .= "(.*)\$";

my $therequest_regex = "(\\S+)\\ (.*?)\\ (\\S+)";

# use strict
my %ipaddresses = ();
my %request;
my $current_time;
my $last_state_save;

sub parse_logline {
    $_ = shift;

    my %request = ();
    $request{"invalid"} = 0;

    my @parsed_logline = /$logline_regex/x;
    if (@parsed_logline == 0) {
        return (0,0);
    }

    (
        $request{"hostname"},
        $request{"remote_ip"},
        $request{"remote_username"},
        $request{"username"},
        $request{"date"},
        $request{"time"},
        $request{"gmt_offset"},
        $request{"the_request"},
        $request{"status"},
        $request{"bytes_out"},
        $request{"referer"},
        $request{"user_agent"},
        $request{"unique_id"},
        $request{"session_id"},
        $request{"duration"},
        $request{"duration_msec"},
        $request{"modsec_message"},
        $request{"modsec_rating"},
        $request{"the_rest"}
    ) = @parsed_logline;

    if ($DEBUG == 2) {
        print "\n";
        print "hostname = " . $request{"hostname"} . "\n";
        print "remote_ip = " . $request{"remote_ip"} . "\n";
        print "remote_username = " . $request{"remote_username"} . "\n";
        print "username = " . $request{"username"} . "\n";
        print "date = " . $request{"date"} . "\n";
        print "time = " . $request{"time"} . "\n";
        print "gmt_offset = " . $request{"gmt_offset"} . "\n";
        print "the_request = " . $request{"the_request"} . "\n";
        print "status = " . $request{"status"} . "\n";
        print "bytes_out = " . $request{"bytes_out"} . "\n";
        print "referer = " . $request{"referer"} . "\n";
        print "user_agent = " . $request{"user_agent"} . "\n";
        print "unique_id = " . $request{"unique_id"} . "\n";
        print "session_id = " . $request{"session_id"} . "\n";
        print "duration = " . $request{"duration"} . "\n";
        print "duration_msec = " . $request{"duration_msec"} . "\n";
        print "modsec_message = " . $request{"modsec_message"} . "\n";
        print "modsec_rating = " . $request{"modsec_rating"} . "\n";
        print "\n\n";
    }

    # parse the request line
    $_ = $request{"the_request"};
    my @parsed_therequest = /$therequest_regex/x;
    if (@parsed_therequest == 0) {
        $request{"invalid"} = "1";
        $request{"request_method"} = "";
        $request{"request_uri"} = "";
        $request{"protocol"} = "";
    } else {
        (
            $request{"request_method"},
            $request{"request_uri"},
            $request{"protocol"}
        ) = @parsed_therequest;
    }

    if ($request{"bytes_out"} eq "-") {
        $request{"bytes_out"} = 0;
    }

    # print "date=" . $request{"date"} . "\n";
    (
        $request{"time_mday"},
        $request{"time_mon"},
        $request{"time_year"}
    ) = ( $request{"date"} =~ m/^(\d+)\/(\S+)\/(\d+)/x );

    # print "time=" . $request{"time"} . "\n";
    (
        $request{"time_hour"},
        $request{"time_min"},
        $request{"time_sec"}
    ) = ( $request{"time"} =~ m/(\d+):(\d+):(\d+)/x );

    $request{"time_mon"} = $months{$request{"time_mon"}};

    $request{"time_epoch"} = timelocal(
        $request{"time_sec"},
        $request{"time_min"},
        $request{"time_hour"},
        $request{"time_mday"},
        $request{"time_mon"},
        $request{"time_year"}
    );

    # print %request;

    my $offset = index($request{"request_uri"}, "?");
    if ($offset != -1) {
        $request{"script_name"} = substr($request{"request_uri"}, 0, $offset);
        $request{"query_string"} = substr($request{"request_uri"}, $offset + 1);
    } else {
        $request{"script_name"} = $request{"request_uri"};
        $request{"query_string"} = "";
    }

    $request{"request_uri"} =~ s/\%([A-Fa-f0-9]{2})/pack('C', hex($1))/seg;
    $request{"query_string"} =~ s/\%([A-Fa-f0-9]{2})/pack('C', hex($1))/seg;

    return %request;
}

sub update_ip_address() {
    my $ipd = $ipaddresses{$request{"remote_ip"}};
    if (defined($$ipd{"counter"})) {
        $$ipd{"counter"} = $$ipd{"counter"} + 1;

        if ($DEBUG) {
            print STDERR "httpd-guardian: Incrementing counter for " . $request{"remote_ip"} . " (" . $$ipd{"counter"} . ")\n";
        }

        my($exec) = 0;

        # check the 1 min counter
        if ($current_time - $$ipd{"time_1min"} > 60) {
            # check the counters
            my $speed = ($$ipd{"counter"} - $$ipd{"counter_1min"}) / ($current_time - $$ipd{"time_1min"});
            if ($speed > $THRESHOLD_1MIN) {
                print STDERR "httpd-guardian: IP address " . $ipaddresses{$request{"remote_ip"}} . " reached the 1 min threshold (speed = $speed req/sec, threshold = $THRESHOLD_1MIN req/sec)\n";
                $exec = 1;
            }

            # reset the 1 min counter
            $$ipd{"time_1min"} = $current_time;
            $$ipd{"counter_1min"} = $$ipd{"counter"};
        }

        # check the 5 min counter
        if ($current_time - $$ipd{"time_5min"} > 360) {
            # check the counters
            my $speed = ($$ipd{"counter"} - $$ipd{"counter_5min"}) / ($current_time - $$ipd{"time_5min"});
            if ($speed > $THRESHOLD_5MIN) {
                print STDERR "httpd-guardian: IP address " . $request{"remote_ip"} . " reached the 5 min threshold (speed = $speed req/sec, threshold = $THRESHOLD_5MIN req/sec)\n";
                $exec = 1;
            }

            # reset the 5 min counter
            $$ipd{"time_5min"} = $current_time;
            $$ipd{"counter_5min"} = $$ipd{"counter"};
        }
    
        if (($exec == 1)&&(defined($PROTECT_EXEC))) {
            my $cmd = sprintf($PROTECT_EXEC, $request{"remote_ip"});
            print STDERR "httpd-guardian: Executing: $cmd\n";
            system($cmd);
        }

    } else {
        # start tracking this email address
        my %ipd = ();
        $ipd{"counter"} = 1;
        $ipd{"counter_1min"} = 1;
        $ipd{"time_1min"} = $current_time;
        $ipd{"counter_5min"} = 1;
        $ipd{"time_5min"} = $current_time;
        $ipaddresses{$request{"remote_ip"}} = \%ipd;
    }
}

sub process_log_line {
    update_ip_address();
}

sub remove_stale_data {
    while(my($key, $value) = each(%ipaddresses)) {
        if ($current_time - $$value{"time_1min"} > $STALE_INTERVAL) {
            if ($DEBUG) {
                print STDERR "httpd-guardian: Removing key $key\n";
            }
            delete($ipaddresses{$key});
        }
    }
}

sub save_state {
    if (!defined($SAVE_STATE_FILE)) {
        return;
    }

    if (!defined($last_state_save)) {
        $last_state_save = 0;
    }

    if ($current_time - $last_state_save > $SAVE_STATE_INTERVAL) {
        open(FILE, ">$SAVE_STATE_FILE") || die("Failed to save state to $SAVE_STATE_FILE");
        print FILE "# $current_time\n";
        print FILE "# IP Address\x09Counter\x09\x091min (time)\x095min (time)\n";
        while(my($key, $value) = each(%ipaddresses)) {
            print FILE ("$key" . "\x09" . $$value{"counter"} . "\x09\x09" . $$value{"counter_1min"} . " (" . $$value{"time_1min"} . ")\x09" . $$value{"counter_5min"} . " (" . $$value{"time_5min"} . ")\n");
        }
        close(FILE);
        $last_state_save = $current_time;
    }
}

# load state from $SAVE_STATE_FILE, store the data into $ipaddresses
sub load_state {
    return unless ( defined $SAVE_STATE_FILE );
    return unless ( -e $SAVE_STATE_FILE && -r $SAVE_STATE_FILE );
    open my $fd, "<", $SAVE_STATE_FILE
        or die "cannot open state file for reading : $SAVE_STATE_FILE : $!";
    while (<$fd>) {
        s/^\s+//;
        next if /^#/;
        #--------------------------------------------------
        # # 1133599679
        # # IP Address  Counter     1min (time)         5min (time)
        # 211.19.48.12  396         396 (1133599679)    395 (1133599379)
        #-------------------------------------------------- 
        my ($addr, $counter, $time1, $time5) = split /\t+/, $_; # TAB
        my ($counter_1min, $time_1min) = split /\s+/, $time1;
        my ($counter_5min, $time_5min) = split /\s+/, $time5;
        $ipaddresses{$addr} = {
            counter         => $counter,
            counter_1min    => $counter_1min,
            time_1min       => chop_brace($time_1min),
            counter_5min    => $counter_5min,
            time_5min       => chop_brace($time_5min),
        }
    }
    close $fd;
}

# return strings between braces
sub chop_brace {
	my $str = shift;
	$str =~ /\((.*)\)/;
	return $1;
}
sub process_line {
    my $line = shift(@_);

    if (defined($COPY_LOG)) {
        print COPY_LOG_FD $line;
    }

    if ($DEBUG) {
        print STDERR "httpd-guardian: Received: $line";
    }

    %request = parse_logline($line);
    if (!defined($request{0})) {
        # TODO verify IP address is in correct format

        # extract the time from the log line, to allow the
        # script to be used for batch processing too
        $current_time = $request{"time_epoch"};

        remove_stale_data();
        process_log_line();
        save_state();
    } else {
        print STDERR "Failed to parse line: " . $line;
    }
}

# -----------------------------------

load_state();
if (defined($COPY_LOG)) {
    open(COPY_LOG_FD, ">>$COPY_LOG") || die("Failed to open $COPY_LOG for writing");
    # enable autoflush on the file descriptor
    $| = 1, select $_ for select COPY_LOG_FD;  
}

if ($USE_SPREAD) {
    my($sperrno);
    my %args;

    $args{"spread_name"} = $SPREAD_DAEMON;
    $args{"private_name"} = $SPREAD_CLIENT_NAME;

    my($mbox, $privategroup) = Spread::connect(\%args);
    if (!defined($mbox)) {
        die "Failed to connect to Spread daemon: $sperrno\n";
    }

    Spread::join($mbox, $SPREAD_GROUP_NAME);

    for(;;) {
        my($st, $s, $g, $mt, $e, $msg);
        while(($st, $s, $g, $mt, $e, $msg) = Spread::receive($mbox, $SPREAD_TIMEOUT)) {
            if ((defined($st))&&($st == 2)&&(defined($msg))) {
                process_line($msg . "\n");
            }
        }
    }

} else {
    while(<STDIN>) {
        process_line($_);    
    }
}

if (defined($COPY_LOG)) {
    close(COPY_LOG_FD);
}

