# Threat model

This file is read by Anthropic's OSS Scanner before it analyses ModSecurity v2 (`mod_security2`,
branch `v2/master`). How to report: see the
[security policy](https://github.com/owasp-modsecurity/ModSecurity/blob/v3/master/SECURITY.md),
which covers both v2.9.x and v3.0.x.

## What this project does and where untrusted input enters

ModSecurity v2 is a Web Application Firewall implemented as an Apache HTTP Server module
(`mod_security2.so`, sources in `apache2/`). It parses the SecRules language (SecLang) at
configuration time and evaluates the rules, normally the OWASP CRS, against every request and
response passing through Apache, from input and output filters (`apache2/apache2_io.c`) and
request hooks (`apache2/mod_security2.c`). It runs inside the Apache worker processes, so a
crash or a memory corruption affects every site the server hosts.

**Every byte of the request and of the response is attacker-controlled**: the request line,
URI, query string, headers, cookies, the request body in every format we parse (urlencoded,
multipart/form-data, JSON, XML, raw), and the response headers and body (the backend can be
compromised or reflect attacker input). The attacker also controls how that input is split
across reads, encoded, repeated and ordered.

**Rules and configuration are trusted.** SecLang directives, operator arguments, file paths
(`@pmFromFile`, `@ipMatchFromFile`, `@inspectFile`, `@validateSchema`, `@validateDTD`,
`SecAuditLog`, `SecDataDir`, ...), Lua scripts (`SecRuleScript`, `exec`), `SecRemoteRules`
sources, the GeoIP and GSB databases and the `ctl:` actions in a rule are the operator's
choice. A finding that requires the attacker to write or alter rules, configuration or those
files is not a vulnerability. A finding where a *valid* rule or directive silently does less
than documented (a limit that is accepted but not enforced, a collection key that never
matches, a parse divergence) is in scope.

## Components that matter most / least

Most, in this order:

1. `apache2/msc_reqbody.c`, `msc_multipart.c`, `msc_json.c`, `msc_xml.c`: request body
   buffering and parsing. Parser disagreements with real backends (PHP, Java, Python, Node.js)
   are the classic WAF bypass. Every processor must honour `SecRequestBodyLimit`,
   `SecRequestBodyNoFilesLimit`, `SecRequestBodyJsonDepthLimit` and `SecArgumentsLimit`, and
   must set `REQBODY_ERROR` (or `MULTIPART_STRICT_ERROR`) when it cannot fully parse a body.
2. `apache2/apache2_io.c` (input and output filters, request and response body handling,
   `SecStreamInBodyInspection`), `msc_parsers.c` (argument and cookie parsing),
   `re_variables.c` (variable generation).
3. `apache2/re_tfns.c` (transformations), `re_operators.c` (operators), `msc_util.c` (the
   decoders behind them), `acmp.c` and `msc_tree.c` (the Aho-Corasick and IP trees behind
   `@pm` and `@ipMatch`). Each transformation must be a total function of its input; each
   operator must be bounded in time and memory for any input.
4. `apache2/msc_pcre.c`: the PCRE2 (and legacy PCRE) wrapper behind `@rx`, `@verifyCC` and
   the other regex users, built with or without JIT and study. It must apply
   `SecPcreMatchLimit` and `SecPcreMatchLimitRecursion` on every path, size and read the
   capture vector correctly (`capture` fills `TX:0`-`TX:9` from it), and report a hit
   limit so that `MSC_PCRE_LIMITS_EXCEEDED` is set instead of the match silently failing.
   Custom scanning loops elsewhere are not bounded by these limits.
5. `apache2/re.c`, `re_actions.c`, `modsecurity.c`: rule evaluation, chains, `skip`/
   `skipAfter`, phases, `ctl:`, `setvar`, macro expansion (`%{...}`) of attacker data.
6. `apache2/persist_dbm.c` (persistent collections, shared between processes),
   `msc_logging.c` (attacker bytes reach the audit and debug logs; format and CR/LF injection
   are in scope), `msc_unicode.c`, `msc_crypt.c`.
7. `apache2/apache2_config.c`: configuration is trusted, but silent misparses of valid
   directives are in scope.

Less: `mlogc/` (the audit log collector, a separate program fed by the server's own audit
log), `standalone/` and `iis/` (other builds, not built in this image), `alp2/`, `ext/`,
`tools/`, `tests/` (the harness itself), `build/`, `doc/`.

Out of scope: `apache2/others/libinjection` (third-party submodule; report to
github.com/libinjection/libinjection), the CRS's own detection logic
(github.com/coreruleset/coreruleset), Apache httpd itself, and ModSecurity v3 (the
`v3/master` branch, enrolled separately).

## How to exercise it

- `make test` (in the top-level directory) runs the unit tests: single operators and
  transformations from `tests/op/*.t` and `tests/tfn/*.t` through `tests/msc_test`. Run one file with `cd tests && ./run-unit-tests.pl op/eq.t`.
- `make test-regression` runs the regression tests in `tests/regression/*/*.t`: each case is
  a Perl hash with a configuration, an `HTTP::Request` and the expected status and log
  lines. The runner starts its own Apache on port 8088 with the module from
  `apache2/.libs/`, with `tests/regression/server_root` as server root. Run one file with
  `cd tests && perl run-regression-tests.pl -S . regression/<dir>/<file>.t`, and add `N` for
  one case. The tests in this image were run as the unprivileged user `builder` (who owns
  `/src/tests`), as in CI; use `runuser -u builder -- ...` to do the same.
- **A reproducer as a regression test case (`tests/regression/*/*.t` format) is what we
  need in a report**, or a unit test case for a single operator or transformation.
- `mod_security2.so` is also installed into the system Apache
  (`/usr/lib/apache2/modules/`), so a full server can be started with
  `modsecurity.conf-recommended` and the OWASP CRS v4 (the latest release when the image was built: `git -C /opt/coreruleset describe --tags` shows which one; name it in any report whose result depends on the CRS) from
  `/opt/coreruleset` (`crs-setup.conf.example`, then `rules/*.conf`).
  `modsecurity.conf-recommended` is the configuration we expect in production, including the
  request body backstop rules 200002 and 200003. Score against it, not against a bare
  `SecRuleEngine On`.
- The image is built without sanitizers. Every dependency is installed, so an ASan build
  works offline: `make distclean && ./configure --enable-assertions --with-lua
  CFLAGS="-g -O1 -fno-omit-frame-pointer -fsanitize=address,undefined"
  LDFLAGS="-fsanitize=address,undefined" && make -j$(nproc)`. Apache loads the module with
  `dlopen`, so start httpd with `LD_PRELOAD=$(gcc -print-file-name=libasan.so)`. `gdb` and
  `valgrind` are installed.

## How you rate severity

We score with CVSS 3.1 and re-score every report during triage.

- **Memory corruption** (buffer overflow, use-after-free, double free, out-of-bounds read
  or write) reachable from request or response data under `modsecurity.conf-recommended`
  plus the CRS: High at a minimum, Critical when it is a controlled write.
- **WAF bypass** (a payload that a rule should inspect never reaches it, or reaches it
  altered so the rule misses, while the backend receives it intact): Scope Changed,
  Integrity Low. The impact lands on the protected application, which must still be
  vulnerable to the payload. That gives Medium for most bypasses. High only when a default
  `modsecurity.conf-recommended` plus CRS deployment at paranoia level 1 misses an entire
  attack class. A bypass that needs a non-default but reasonable configuration is scored as
  if that configuration were present; say what the precondition is.
- **Denial of service**: CPU or memory that grows beyond what the configured limits bound,
  from a single request, is Medium; High when a small request reliably exhausts a worker.
- **Crash without memory corruption** (null dereference, assertion) reachable from request
  data: Medium.
- **Log injection** into the audit or debug log: Low to Medium, depending on whether it can
  forge or hide entries.
- Anything that requires control of rules, configuration, Lua scripts or files on the
  server is not a vulnerability.

## How we want reports and patches

- A reproducer as a regression or unit test case, the exact configuration, and the build
  options.
- Patches against `v2/master`, C99, in the style of the surrounding code, one fix per
  patch, with the test case in the same patch.
- Many v2 bugs also exist in v3 (libModSecurity) in a different form; if you notice that,
  say so, but report the v3 side against the v3 enrolment.

## Anything to leave alone

- Findings in `tests/`, `tools/` or the build scripts that cannot be triggered through the
  module.
- Missing hardening flags, compiler warnings without an exploitable consequence, and
  findings that only apply to the legacy PCRE build (`--with-pcre`).
