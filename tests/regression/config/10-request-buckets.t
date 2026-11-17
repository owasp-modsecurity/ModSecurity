### Preserve bucket-level limits and phase-2 visibility while refactoring I/O.
do {
    my @tests;
    for my $policy (["On", "Reject"], ["On", "ProcessPartial"],
                    ["DetectionOnly", "Reject"], ["DetectionOnly", "ProcessPartial"]) {
        my ($engine, $action) = @$policy;
        for my $limit_kind ("body", "no-files") {
            for my $size (31, 32, 33) {
                my $body_limit = $limit_kind eq "body" ? 32 : 4096;
                my $no_files_limit = $limit_kind eq "no-files" ? 32 : 4096;
                my $content_type = $limit_kind eq "no-files"
                    ? "application/x-www-form-urlencoded" : "text/plain";
                my $status = 200;
                if ($size > 32 && ($limit_kind eq "no-files"
                    || ($engine eq "On" && $action eq "Reject"))) {
                    $status = 413;
                }
                push @tests, {
                    type => "config",
                    comment => "chunked $limit_kind limit: $engine/$action, $size bytes",
                    conf => qq(
                        SecRuleEngine $engine
                        SecRequestBodyAccess On
                        SecRequestBodyLimit $body_limit
                        SecRequestBodyNoFilesLimit $no_files_limit
                        SecRequestBodyLimitAction $action
                        SecAction "id:200010,phase:2,pass,log,msg:'BUCKET_PHASE2'"
                    ),
                    match_response => { status => qr/^$status\z/ },
                    match_log => {
                        ($status == 200 ? "error" : "-error") =>
                            [ qr/\[id "200010"\].*BUCKET_PHASE2/, 1 ],
                    },
                    request => normalize_raw_request_data(qq(
                        POST /test.txt HTTP/1.1
                        Host: $ENV{SERVER_NAME}:$ENV{SERVER_PORT}
                        User-Agent: $ENV{USER_AGENT}
                        Content-Type: $content_type
                        Transfer-Encoding: chunked

                    )) . encode_chunked("x" x $size, 7),
                };
            }
        }
    }
    @tests;
}
