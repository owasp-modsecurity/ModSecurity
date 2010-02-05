### Multipart parser tests

# Normal
{
    type => "misc",
    comment => "multipart parser (normal)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Added file part [0-9a-h]+ to the list: name "image" file name "image.jpg" \(offset 258, length 10\).*Adding request argument \(BODY\): name "name", value "Brian Rectanus".*Adding request argument \(BODY\): name "email", value "brian.rectanus\@breach.com"/s, 1 ],
        -debug => [ qr/Multipart.*(?i:error|warning)/, 1 ],
    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary=0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},

# Final CRLF or not, we should still work
{
    type => "misc",
    comment => "multipart parser (final CRLF)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Adding request argument \(BODY\): name "a", value "1".*Adding request argument \(BODY\): name "b", value "2"/s, 1 ],
        -debug => [ qr/Multipart error:/, 1 ],

    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="a"
                
                1
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="b"
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},

# No final CRLF
{
    type => "misc",
    comment => "multipart parser (no final CRLF)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Adding request argument \(BODY\): name "a", value "1".*Adding request argument \(BODY\): name "b", value "2"/s, 1 ],
        -debug => [ qr/Multipart error:/, 1 ],

    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="a"
                
                1
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="b"
                
                2
                -----------------------------69343412719991675451336310646--),
        ),
    ),
},

# Should work with a boundary of "boundary"
{
    type => "misc",
    comment => "multipart parser (boundary contains \"boundary\")",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Adding request argument \(BODY\): name "a", value "1".*Adding request argument \(BODY\): name "b", value "2"/s, 1 ],
        -debug => [ qr/Multipart error:/, 1 ],

    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=------------------------------------------------boundary",
        ],
        normalize_raw_request_data(
            q(
                --------------------------------------------------boundary
                Content-Disposition: form-data; name="a"
                
                1
                --------------------------------------------------boundary
                Content-Disposition: form-data; name="b"
                
                2
                --------------------------------------------------boundary--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (boundary contains \"bOuNdArY\")",
    note => q(
        KHTML Boundary
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Adding request argument \(BODY\): name "a", value "1".*Adding request argument \(BODY\): name "b", value "2"/s, 1 ],
        -debug => [ qr/Multipart error:/, 1 ],

    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=--------0xKhTmLbOuNdArY",
        ],
        normalize_raw_request_data(
            q(
                ----------0xKhTmLbOuNdArY
                Content-Disposition: form-data; name="a"
                
                1
                ----------0xKhTmLbOuNdArY
                Content-Disposition: form-data; name="b"
                
                2
                ----------0xKhTmLbOuNdArY--
            ),
        ),
    ),
},

# We should handle data starting with a "--"
{
    type => "misc",
    comment => "multipart parser (data contains \"--\")",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Adding request argument \(BODY\): name "a", value "--test".*Adding request argument \(BODY\): name "b", value "--"/s, 1 ],
        -debug => [ qr/Multipart error:/, 1 ],

    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="a"
                
                --test
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="b"
                
                --
                -----------------------------69343412719991675451336310646--),
        ),
    ),
},

# We should emit warnings for parsing errors
{
    type => "misc",
    comment => "multipart parser error (no final boundary)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecAuditLog "$ENV{AUDIT_LOG}"
        SecAuditEngine RelevantOnly
    ),
    match_log => {
        audit => [ qr/Final boundary missing/, 1 ],
        debug => [ qr/Final boundary missing/, 1 ],

    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="a"
                
                1
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="b"
                
                2
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser error (no disposition)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecAuditLog "$ENV{AUDIT_LOG}"
        SecAuditEngine RelevantOnly
    ),
    match_log => {
        -debug => [ qr/Multipart error:/, 1 ],
        audit => [ qr/Part missing Content-Disposition header/, 1 ],
        debug => [ qr/Part missing Content-Disposition header/, 1 ],

    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                
                1
                -----------------------------69343412719991675451336310646
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser error (bad disposition)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecAuditLog "$ENV{AUDIT_LOG}"
        SecAuditEngine RelevantOnly
    ),
    match_log => {
        audit => [ qr/Invalid Content-Disposition header/, 1 ],
        debug => [ qr/Invalid Content-Disposition header/, 1 ],

    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data name="a"
                
                1
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data name="b"
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser error (no disposition name)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecAuditLog "$ENV{AUDIT_LOG}"
        SecAuditEngine RelevantOnly
    ),
    match_log => {
        -debug => [ qr/Multipart error:/, 1 ],
        audit => [ qr/Content-Disposition header missing name field/, 1 ],
        debug => [ qr/Content-Disposition header missing name field/, 1 ],

    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data;
                
                1
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data;
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},

# Zero length part name should not crash
{
    type => "misc",
    comment => "multipart parser (zero length part name)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "!\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "!\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/name: a.*variable: 1.*Invalid part header \(header name missing\)/s, 1 ],
        -debug => [ qr/Adding request argument \(BODY\): name "b"/s, 1 ],
    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="a"
                
                1
                -----------------------------69343412719991675451336310646
                :
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="b"
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},

# Part header folding
{
    type => "misc",
    comment => "multipart parser (part header folding - space)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "!\@eq 1" "phase:2,deny,status:403"
        SecRule MULTIPART_HEADER_FOLDING "!\@eq 1" "phase:2,deny,status:403"
        SecRule MULTIPART_INVALID_HEADER_FOLDING "!\@eq 0" "phase:2,deny,status:403"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny,status:403"
    ),
    match_log => {
        debug => [ qr/name: a.*variable: 1.*name: b.*variable: 2/s, 1 ],
    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data;
                 name="a"
                
                1
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data;
                    name="b"
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (part header folding - tab)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "!\@eq 1" "phase:2,deny,status:403"
        SecRule MULTIPART_HEADER_FOLDING "!\@eq 1" "phase:2,deny,status:403"
        SecRule MULTIPART_INVALID_HEADER_FOLDING "!\@eq 0" "phase:2,deny,status:403"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny,status:403"
    ),
    match_log => {
        debug => [ qr/name: a.*variable: 1.*name: b.*variable: 2/s, 1 ],
    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data;
                	name="a"
                
                1
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data;
                		name="b"
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (part header folding - mixed)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "!\@eq 1" "phase:2,deny,status:403"
        SecRule MULTIPART_HEADER_FOLDING "!\@eq 1" "phase:2,deny,status:403"
        SecRule MULTIPART_INVALID_HEADER_FOLDING "!\@eq 0" "phase:2,deny,status:403"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny,status:403"
    ),
    match_log => {
        debug => [ qr/name: a.*variable: 1.*name: b.*variable: 2/s, 1 ],
    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="a"
                
                1
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data;
                	 	name="b"
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (part header folding - invalid)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "!\@eq 1" "phase:2,deny,status:403"
        SecRule MULTIPART_HEADER_FOLDING "!\@eq 1" "phase:2,deny,status:403"
        SecRule MULTIPART_INVALID_HEADER_FOLDING "!\@eq 1" "phase:2,deny,status:403"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny,status:403"
    ),
    match_log => {
        debug => [ qr/name: a.*variable: 1.*name: b.*variable: 2/s, 1 ],
    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="a"
                
                1
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data;
                name="b"
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (part header folding - mixed invalid)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "!\@eq 1" "phase:2,deny,status:403"
        SecRule MULTIPART_HEADER_FOLDING "!\@eq 1" "phase:2,deny,status:403"
        SecRule MULTIPART_INVALID_HEADER_FOLDING "!\@eq 1" "phase:2,deny,status:403"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny,status:403"
    ),
    match_log => {
        debug => [ qr/name: a.*variable: 1.*name: b.*variable: 2/s, 1 ],
    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="a"
                
                1
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data;
                 	name="b"
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},

# Data following final boundary should set flag
{
    type => "misc",
    comment => "multipart parser (data after final boundary)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_DATA_AFTER "\@eq 1" "phase:2,deny,status:403"
    ),
    match_log => {
        debug => [ qr/name: a.*variable: 1.*Ignoring data after last boundary/s, 1 ],
        -debug => [ qr/Adding request argument \(BODY\): name "b"/s, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="a"
                
                1
                -----------------------------69343412719991675451336310646--
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="b"
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},

# Single quoted data is invalid
{
    type => "misc",
    comment => "multipart parser (C-D uses single quotes)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "!\@eq 1" "phase:2,deny"
        SecRule MULTIPART_INVALID_QUOTING "!\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "chain,phase:2,deny"
    ),
    match_log => {
        debug => [ qr/name: a.*variable: 1.*Duplicate Content-Disposition name/s, 1 ],
        -debug => [ qr/Adding request argument \(BODY\): name "b/s, 1 ],
    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => "multipart/form-data; boundary=---------------------------69343412719991675451336310646",
        ],
        normalize_raw_request_data(
            q(
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name="a"
                
                1
                -----------------------------69343412719991675451336310646
                Content-Disposition: form-data; name=';filename="dummy';name=b;"
                
                2
                -----------------------------69343412719991675451336310646--
            ),
        ),
    ),
},

# Invalid boundary separators
{
    type => "misc",
    comment => "multipart parser (invalid C-T boundary separator - comma)",
    note => q(
        PHP 5.2.3 - no effect.
    ),
    conf => qq(
        SecRuleEngine On
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        error => [ qr/Invalid boundary in C-T \(malformed\)/, 1 ],

    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data, boundary=0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (invalid C-T boundary separator - space)",
    note => q(
        PHP 5.2.3 - no effect.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data boundary=0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},

# Invalid boundary parameter names
{
    type => "misc",
    comment => "multipart parser (invalid C-T boundary parameter name - case)",
    note => q(
        PHP 5.2.3 - does not recognise boundary.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        error => [ qr/Invalid boundary in C-T \(case sensitivity\)/, 1 ],

    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; bOundAry=0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (invalid C-T boundary parameter name - trailing chars)",
    note => q(
        PHP 5.2.3 - does not recognise boundary.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        error => [ qr/Invalid boundary in C-T \(parameter name\)/, 1 ],

    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary123=0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},

# Invalid boundaries
{
    type => "misc",
    comment => "multipart parser (multiple C-T boundaries - first quoted)",
    note => q(
        PHP 5.2.3 - uses first boundary.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Multiple boundary parameters in C-T/, 1 ],

    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary="0000"; boundary=1111),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (multiple C-T boundaries - comma separated)",
    note => q(
        PHP 5.2.3 - uses first boundary.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Multiple boundary parameters in C-T/, 1 ],

    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary=0000, boundary=1111),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (boundary whitespace in C-T - after name)",
    note => q(
        PHP 5.2.3 - no effect.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary =0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (boundary whitespace in C-T - before value)",
    note => q(
        PHP 5.2.3 - uses " 0000" as boundary.
        We should probably interpret it as "0000" and just flag a strict error.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/boundary whitespace in C-T header/, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary= 0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (boundary whitespace in C-T - after value)",
    note => q(
        PHP 5.2.3 - no effect.
        Apache just silently removes the trailing whitespace for us.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Added file part [0-9a-h]+ to the list: name "image" file name "image.jpg" \(offset 258, length 10\).*Adding request argument \(BODY\): name "name", value "Brian Rectanus".*Adding request argument \(BODY\): name "email", value "brian.rectanus\@breach.com"/s, 1 ],
        -debug => [ qr/Multipart.*(?i:error|warning)/, 1 ],
    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary=0000 ),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},

# Special chars
{
    type => "misc",
    comment => "multipart parser (boundary special char - trailing whitespace+token)",
    note => q(
        PHP 5.2.3 - uses "0000 1111" as boundary.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/No boundaries found in payload/, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary=0000 1111),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (boundary special char - trailing comma+token)",
    note => q(
        PHP 5.2.3 - uses "0000" as boundary.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Invalid boundary in C-T \(characters\)/, 1 ],

    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary=0000,1111),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},

# Quoting
{
    type => "misc",
    comment => "multipart parser (quoted boundary - normal)",
    note => q(
        PHP 5.2.3 - no effect.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/boundary was quoted/, 1 ],

    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary="0000"),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (quoted boundary value - whitespace before)",
    note => q(
        PHP 5.2.3 - uses " 0000" as boundary.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/boundary was quoted.*No boundaries found in payload/s, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary=" 0000"),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (quoted boundary value - whitespace after)",
    note => q(
        PHP 5.2.3 - uses "0000 " as boundary.
        Trailing whitespace violates RFC as well.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/boundary was quoted.*No boundaries found in payload/s, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary="0000 "),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (quoted boundary value - whitespace between)",
    note => q(
        PHP 5.2.3 - uses "0000 " as boundary.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/boundary was quoted/s, 1 ],
        -debug => [ qr/No boundaries found in payload/s, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary="0000 1111"),
        ],
        normalize_raw_request_data(
            q(
                --0000 1111
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000 1111
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000 1111
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000 1111--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (quoted boundary value - contained quote)",
    note => q(
        PHP 5.2.3 - uses "0000 " as boundary.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Invalid boundary in C-T \(characters\)/, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary="00\"00"),
        ],
        normalize_raw_request_data(
            q(
                --00"00
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --00"00
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --00"00
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --00"00--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (quoted boundary value - two quoted values)",
    note => q(
        PHP 5.2.3 - does not work, uses "00" as boundary.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Invalid boundary in C-T \(characters\)/, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary="00""00"),
        ],
        normalize_raw_request_data(
            q(
                --00"00
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --00"00
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --00"00
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --00"00--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (partial quoted boundary value - only start quote)",
    note => q(
        PHP 5.2.3 - breaks parsing.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Invalid boundary in C-T \(quote\)/, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary="0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (partial quoted boundary value - only end quote)",
    note => q(
        PHP 5.2.3 - breaks parsing.
    ),
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Invalid boundary in C-T \(quote\)/, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary=0000"),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image"; filename="image.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA
                --0000--
            ),
        ),
    ),
},

# Multipart mixed
{
    type => "misc",
    comment => "multipart parser (multipart mixed - normal)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Invalid Content-Disposition header/, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary=0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: attachment
                Content-Type: multipart/mixed; boundary=BbC04y

                --BbC04y
                Content-Disposition: file; filename="file1.txt"
                Content-Type: text/plain

                ... contents of file1.txt ...
                --BbC04y
                Content-Disposition: file; filename="file2.gif
                Content-Type: image/jpeg
                Content-Transfer-Encoding: binary

                ...contents of file2.gif...
                --0000--
            ),
        ),
    ),
},
{
    type => "misc",
    comment => "multipart parser (multipart mixed - missing disposition)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule MULTIPART_STRICT_ERROR "\@eq 1" "phase:2,deny"
        SecRule MULTIPART_UNMATCHED_BOUNDARY "\@eq 1" "phase:2,deny"
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Part missing Content-Disposition header/, 1 ],
    },
    match_response => {
        status => qr/^403$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary=0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Type: multipart/mixed; boundary=BbC04y

                --BbC04y
                Content-Disposition: file; filename="file1.txt"
                Content-Type: text/plain

                ... contents of file1.txt ...
                --BbC04y
                Content-Disposition: file; filename="file2.gif
                Content-Type: image/jpeg
                Content-Transfer-Encoding: binary

                ...contents of file2.gif...
                --0000--
            ),
        ),
    ),
},

# File limits
{
    type => "misc",
    comment => "multipart parser (normal)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecTmpDir "$ENV{TEMP_DIR}"
        SecUploadDir "$ENV{UPLOAD_DIR}"
        SecUploadKeepFiles On
        SecUploadFileLimit 2

        # These should be set
        SecRule MULTIPART_STRICT_ERROR "!\@eq 1" "phase:2,deny"
        SecRule MULTIPART_FILE_LIMIT_EXCEEDED "!\@eq 1" "phase:2,deny"

        # This should not be set
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny"

        # Theses should still be accurate
        SecRule &FILES "!\@eq 3" "phase:2,deny"
        SecRule &FILES_NAMES "!\@eq 3" "phase:2,deny"
        SecRule &FILES_SIZES "!\@eq 3" "phase:2,deny"
        SecRule FILES_SIZES:/^image/ "\@eq 0" "phase:2,deny"

        # This should be the SecUploadFileLimit
        SecRule &FILES_TMPNAMES "!\@eq 2" "phase:2,deny"
    ),
    match_log => {
        debug => [ qr/Multipart: Upload file limit exceeded.*name: test.*variable: This is test data/s, 1 ],
    },
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(multipart/form-data; boundary=0000),
        ],
        normalize_raw_request_data(
            q(
                --0000
                Content-Disposition: form-data; name="name"
                
                Brian Rectanus
                --0000
                Content-Disposition: form-data; name="email"
                
                brian.rectanus@breach.com
                --0000
                Content-Disposition: form-data; name="image1"; filename="image1.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA1
                --0000
                Content-Disposition: form-data; name="image2"; filename="image2.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA2
                --0000
                Content-Disposition: form-data; name="image3"; filename="image3.jpg"
                Content-Type: image/jpeg
                
                BINARYDATA3
                --0000
                Content-Disposition: form-data; name="test"
                
                This is test data.
                --0000--
            ),
        ),
    ),
},

