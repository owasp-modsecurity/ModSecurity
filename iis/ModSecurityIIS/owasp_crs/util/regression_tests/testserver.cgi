#!/usr/bin/perl
use CGI qw/:standard/;
$response_status = http('Response-Status') || "200 OK";
$response_content = http('Response-Content');
$response_type = http('Response-Content-Type') || "text/html";
$response_new_header_name = http('Response-Header-Name');
$response_new_header_value = http('Response-Header-Value');

$response_new_header = defined($response_new_header_name) ? $response_new_header_name . ': ' . $response_new_header_value : undef;

if (defined($response_new_header)) {
  print header ($response_type, $response_status, undef, undef, undef, undef, undef, undef, undef,$response_new_header);
} else {
  print header ($response_type, $response_status);
}
print start_html('rule set tester');
print h1('rule set tester');
print $response_content;
