### JSON parser tests

# Normal
{
    type => "misc",
    comment => "JSON parser (normal)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny,id:500657"
    ),
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(application/json),
        ],
        normalize_raw_request_data(
            q(
                {
                 "key1": "value",
                 "key2": 10,
                 "key3": [1, 2],
                 "key4": {"key1": "value", "key2": 10, "key3": [1, 2]}
                }
            ),
        ),
    ),
},

# Empty key -> should still work
{
    type => "misc",
    comment => "JSON parser (Empty key)",
    conf => qq(
        SecRuleEngine On
        SecDebugLog $ENV{DEBUG_LOG}
        SecDebugLogLevel 9
        SecRequestBodyAccess On
        SecRule REQBODY_PROCESSOR_ERROR "\@eq 1" "phase:2,deny,id:500660"
    ),
    match_response => {
        status => qr/^200$/,
    },
    request => new HTTP::Request(
        POST => "http://$ENV{SERVER_NAME}:$ENV{SERVER_PORT}/test.txt",
        [
            "Content-Type" => q(application/json),
        ],
        normalize_raw_request_data(
            q(
                25
            ),
        ),
    ),
},
