### Regression tests for issue #2807: preserve rejection, but return 400.
### YAJL is optional. Check the existing no-YAJL policy separately, without
### accepting its response codes for a build that actually parsed the request.
map {
    my $case = $_;
    my $request = delete $case->{request};
    my $response = delete $case->{match_response};
    my $logs = delete $case->{match_log};
    my $no_yajl_status = delete $case->{no_yajl_status};
    $case->{test} = sub {
        my $resp = do_request($request);
        return 1 unless $resp;
        if (defined match_log("error", qr/JSON support was not enabled/, 0)) {
            if ($resp->code != $no_yajl_status) {
                msg("No-YAJL response: expected $no_yajl_status, got " . $resp->code);
                return 1;
            }
            return 0;
        }
        unless (defined match_response("status", $resp, $response->{status})) {
            msg("Response status failed to match: " . $response->{status});
            vrb($resp);
            return 1;
        }
        for my $key (keys %$logs) {
            my ($neg, $name) = ($key =~ /^(-?)(.*)$/);
            my $match = match_log($name, @{$logs->{$key}});
            if (($neg && defined $match) || (!$neg && !defined $match)) {
                msg("Log expectation failed: $key " . $logs->{$key}[0]);
                return 1;
            }
        }
        return 0;
    };
    $case;
} (
{
	type => "rule",
	no_yajl_status => 400,
	comment => "json parser - issue #2807 - premature EOF is rejected before phase 2",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
		SecAction "id:200005,phase:2,pass,log,msg:'JSON phase 2 reached'"
	),
	match_log => {
		error => [ qr/JSON parser error:.*premature EOF/s, 1 ],
		-error => [ qr/\[id "20000[25]"\]/, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},

{
	type => "rule",
	no_yajl_status => 422,
	comment => "json parser - issue #2807 - incomplete object retains rejection before a custom error rule",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:422,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
		SecAction "id:200005,phase:2,pass,log,msg:'JSON phase 2 reached'"
	),
	match_log => {
		error => [ qr/JSON parser error:.*premature EOF/s, 1 ],
		-error => [ qr/\[id "20000[25]"\]/, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{ "id" : "123"',
	),
},

{
	type => "rule",
	no_yajl_status => 200,
	comment => "json parser - issue #2807 - premature EOF retains rejection in DetectionOnly",
	conf => qq(
		SecRuleEngine DetectionOnly
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
		SecAction "id:200005,phase:2,pass,log,msg:'JSON phase 2 reached'"
	),
	match_log => {
		error => [ qr/JSON parser error:.*premature EOF/s, 1 ],
		-error => [ qr/\[id "20000[25]"\]/, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},

{
	type => "rule",
	no_yajl_status => 200,
	comment => "json parser - issue #2807 - premature EOF retains rejection with a non-disruptive error rule",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,pass,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
		SecAction "id:200005,phase:2,pass,log,msg:'JSON phase 2 reached'"
	),
	match_log => {
		error => [ qr/JSON parser error:.*premature EOF/s, 1 ],
		-error => [ qr/\[id "20000[25]"\]/, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},

{
	type => "rule",
	no_yajl_status => 400,
	comment => "json parser - issue #2807 - valid empty object does not set REQBODY_ERROR",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		-error => [ qr/\[id "200002"\]/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{}',
	),
},

{
	type => "rule",
	no_yajl_status => 200,
	comment => "json parser - issue #2807 - empty body retains existing behavior",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		-error => [ qr/\[id "200002"\]/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'',
	),
},

{
	type => "rule",
	no_yajl_status => 400,
	comment => "json parser - issue #2807 - in-stream syntax errors still use the error rule",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		error => [ qr/Access denied with code 400.*\[id "200002"\]/s, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{ id : "123" }',
	),
},

{
	type => "rule",
	no_yajl_status => 400,
	comment => "json parser - issue #2807 - valid JSON arguments are still inspected",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
		SecRule ARGS:foo "\@streq bar" "id:200003,phase:2,t:none,log,deny,status:403"
	),
	match_log => {
		error => [ qr/Access denied with code 403.*\[id "200003"\]/s, 1 ],
	},
	match_response => {
		status => qr/^403$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{"foo":"bar"}',
	),
},

{
	type => "rule",
	no_yajl_status => 400,
	comment => "json parser - issue #2807 - chunked premature EOF is rejected before phase 2",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
		SecAction "id:200005,phase:2,pass,log,msg:'JSON phase 2 reached'"
	),
	match_log => {
		error => [ qr/JSON parser error:.*premature EOF/s, 1 ],
		-error => [ qr/\[id "20000[25]"\]/, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => normalize_raw_request_data(
		qq(
			POST /test.txt HTTP/1.1
			Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
			User-Agent: $ENV{USER_AGENT}
			Content-Type: application/json
			Transfer-Encoding: chunked

		),
	) . encode_chunked('{ "id" : "123"', 3),
},

{
	type => "rule",
	no_yajl_status => 200,
	comment => "json parser - issue #2807 - no error rule still rejects incomplete JSON",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecAction "id:200005,phase:2,pass,log,msg:'JSON phase 2 reached'"
	),
	match_log => {
		error => [ qr/JSON parser error:.*premature EOF/s, 1 ],
		-error => [ qr/\[id "20000[25]"\]/, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},

{
	type => "rule",
	no_yajl_status => 200,
	comment => "json parser - issue #2807 - excluded error rule still rejects incomplete JSON",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQUEST_URI "^/test[.]txt\$" \\
		    "id:200004,phase:1,pass,nolog,ctl:ruleRemoveById=200002"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error'"
		SecAction "id:200005,phase:2,pass,log,msg:'JSON phase 2 reached'"
	),
	match_log => {
		error => [ qr/JSON parser error:.*premature EOF/s, 1 ],
		-error => [ qr/\[id "20000[25]"\]/, 1 ],
	},
	match_response => {
		status => qr/^400$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},

{
	type => "rule",
	no_yajl_status => 200,
	comment => "json parser - issue #2807 - disabled engine retains existing behavior",
	conf => qq(
		SecRuleEngine Off
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
		SecAction "id:200005,phase:2,pass,log,msg:'JSON phase 2 reached'"
	),
	match_log => {
		-error => [ qr/JSON parser error:/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},

{
	type => "rule",
	no_yajl_status => 200,
	comment => "json parser - issue #2807 - disabled request body access retains existing behavior",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess Off
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:400,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
		SecAction "id:200005,phase:2,pass,log,msg:'JSON phase 2 reached'"
	),
	match_log => {
		-error => [ qr/JSON parser error:/, 1 ],
	},
	match_response => {
		status => qr/^200$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{',
	),
},

{
	type => "rule",
	no_yajl_status => 422,
	comment => "json parser - issue #2807 - in-stream syntax errors retain custom error status",
	conf => qq(
		SecRuleEngine On
		SecRequestBodyAccess On
		SecDebugLog $ENV{DEBUG_LOG}
		SecDebugLogLevel 9
		SecRule REQUEST_HEADERS:Content-Type "^application/json" \\
		    "id:200001,phase:1,t:none,pass,nolog,ctl:requestBodyProcessor=JSON"
		SecRule REQBODY_ERROR "!\@eq 0" \\
		    "id:200002,phase:2,t:none,log,deny,status:422,msg:'JSON body error',logdata:'%{REQBODY_ERROR_MSG}'"
	),
	match_log => {
		error => [ qr/Access denied with code 422.*\[id "200002"\]/s, 1 ],
	},
	match_response => {
		status => qr/^422$/,
	},
	request => new HTTP::Request(
		POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
		[ "Content-Type" => "application/json" ],
		'{ id : "123" }',
	),
},

)
