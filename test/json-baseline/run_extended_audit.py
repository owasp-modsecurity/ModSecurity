#!/usr/bin/env python3
"""Separate Transaction.toJSON and public processContentOffset tests.

Fixtures are synthetic hex bytes, never parsed/reencoded before the C++ probe.
C cases characterize the original; R cases assert useful output requirements and
remain ordinary FAIL when the original cannot meet them. No production fixes.
Unusual connector header bytes are API tests, not HTTP-server acceptance claims.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys
from run_audit import Object, equal, expect, headers, mapping, reject_constant

A, B, C, D, E, F, G, H, I, J, K, Z = (2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096)
ALL = A | B | C | E | F | H
FIXTURES = {
    "all-controls": bytes(range(32)),
    "json-text": b'null true false 1.2300 ["quote\\\"","slash\\\\"]',
    "nul-middle": b'before\x00after',
    "unicode": "\u00a2\u20ac\U0001f600\u2028\u2029".encode("utf-8"),
    "long": b'key"\\' * 512,
    "empty": b'',
    "invalid-utf8": b'pre\x80\xc0\xaf\xed\xa0\x80\xf4\x90\x80\x80post',
    "clean": b'clean',
}
AUDIT_CASES = [
    ("AUDX-C01", "all-controls", ALL, "single", "all control bytes, exact request/response distinction"),
    ("AUDX-C02", "json-text", ALL, "single", "JSON-looking text remains a string"),
    ("AUDX-C03", "nul-middle", ALL, "single", "embedded NUL is length-preserved"),
    ("AUDX-C04", "unicode", ALL, "single", "Unicode header names and line-separator values"),
    ("AUDX-C05", "long", ALL, "single", "bounded long escaped values"),
    ("AUDX-C06", "empty", ALL, "single", "empty request versus prefixed response"),
    ("AUDX-P01", "nul-middle", A, "single", "A alone has mandatory metadata"),
    ("AUDX-P02", "nul-middle", B, "single", "B includes request headers only"),
    ("AUDX-P03", "nul-middle", C, "single", "C includes request body only"),
    ("AUDX-P04", "nul-middle", E, "single", "E includes response body only"),
    ("AUDX-P05", "nul-middle", F, "single", "F includes response headers only"),
    ("AUDX-P06", "nul-middle", H, "single", "H includes producer and empty messages"),
    ("AUDX-P07", "nul-middle", D | G | I | J | K | Z, "single", "other parts do not add JSON fields"),
    ("AUDX-L01", "nul-middle", ALL, "repeat", "three serializations preserve transaction state"),
    ("AUDX-L02", "nul-middle", ALL, "lifecycle", "new transaction has no previous body or headers"),
    ("AUDX-C07", "invalid-utf8", B | C, "single", "request binary data is ASCII hex-rendered"),
    ("AUDX-R01", "invalid-utf8", E | F, "single", "response writer output is valid UTF-8 JSON"),
]


def request_display(value):
    # Frozen source contract src/utils/string.h:106: byte-wise hex outside 32..126.
    return ''.join(chr(c) if 32 <= c <= 126 else f'\\x{c:02x}' for c in value)


def decode(raw):
    return json.loads(raw.decode("utf-8", "strict"), object_pairs_hook=Object,
                      parse_constant=reject_constant)


def check_audit(record, payload, mask, ident, invalid_response=False):
    root = mapping(record, "root")
    equal(set(root), {"transaction"}, "root members")
    tx = mapping(root["transaction"], "transaction")
    names = {"client_ip", "time_stamp", "server_id", "client_port", "host_ip", "host_port", "unique_id", "is_interrupted", "request", "response"}
    if mask & H:
        names |= {"producer", "messages"}
    equal(set(tx), names, "transaction members")
    equal(tx["client_ip"], "192.0.2.31", "client IP")
    equal(tx["host_ip"], "192.0.2.32", "host IP")
    equal(tx["client_port"], 10001, "client port number")
    equal(tx["host_port"], 8081, "host port number")
    equal(tx["is_interrupted"], False, "interruption boolean")
    equal(tx["unique_id"], ident, "transaction identity")
    for field in ("time_stamp", "server_id"):
        expect(type(tx[field]) is str and bool(tx[field]), f"{field}: nonempty string")
    request = mapping(tx["request"], "request")
    names = {"method", "http_version", "hostname", "uri"}
    if mask & B: names.add("headers")
    if mask & C: names.add("body")
    equal(set(request), names, "request part inclusion/exclusion")
    equal(request["method"], "POST", "request method")
    equal(request["http_version"], "1.1", "request HTTP version")
    equal(request["hostname"], "writer.invalid", "request hostname")
    equal(request["uri"], '/quote"back\\?q=writer', "request URI escapes")
    if mask & C: equal(request["body"], request_display(payload), "request body")
    if mask & B:
        headers(request["headers"], [("Content-Type", "application/octet-stream"),
            ("Content-Length", str(len(payload))), ("X-Trace", request_display(b"request:" + payload)),
            ("X-Trace", "request:second"), ('X-Name"\\', 'request:name'),
            (r'X-Uni\xc2\xa2', 'request:unicode-name')], "request exact duplicate headers")
    response = mapping(tx["response"], "response")
    names = {"http_code"}
    if mask & E: names.add("body")
    if mask & F: names.add("headers")
    equal(set(response), names, "response part inclusion/exclusion")
    equal(response["http_code"], 202, "response numeric status")
    if mask & E:
        if invalid_response:
            expect(type(response["body"]) is str, "invalid-byte response body is a JSON string")
        else:
            equal(response["body"], (b"response:" + payload).decode("utf-8", "strict"), "response body")
    if mask & F and invalid_response:
        # This requirement deliberately leaves the invalid-byte representation
        # open. A future repair may hex/base64/escape/reject by explicit policy;
        # this particular requirement asks for valid UTF-8 JSON on API success.
        value = response["headers"]
        expect(type(value) is Object, "response headers are an object")
        expect(all(type(k) is str and type(v) is str for k, v in value), "all response header members are strings")
        equal(sorted(k for k, v in value), sorted(["Content-Type", "X-Trace", "X-Trace", 'X-Name"\\', 'X-Uni\u00a2']), "response header multiplicity")
        fixed = [(k, v) for k, v in value if k != 'X-Trace']
        headers(Object(fixed), [("Content-Type", "text/plain"), ('X-Name"\\', 'response:name'), ('X-Uni\u00a2', 'response:unicode-name')], "fixed response headers")
        expect(sum(k == 'X-Trace' and v == 'response:second' for k, v in value) == 1, "unchanged duplicate header retained")
    elif mask & F:
        headers(response["headers"], [("Content-Type", "text/plain"),
            ("X-Trace", (b"response:" + payload).decode("utf-8", "strict")),
            ("X-Trace", "response:second"), ('X-Name"\\', 'response:name'),
            ('X-Uni\u00a2', 'response:unicode-name')], "response exact duplicate headers")
    if mask & H:
        producer = mapping(tx["producer"], "producer")
        equal(set(producer), {"modsecurity", "connector", "secrules_engine", "components"}, "producer members")
        expect(type(producer["modsecurity"]) is str and "ModSecurity" in producer["modsecurity"], "ModSecurity producer")
        equal(producer["connector"], "extended JSON writer probe", "connector")
        equal(producer["secrules_engine"], "Enabled", "engine state")
        equal(producer["components"], [], "components array")
        equal(tx["messages"], [], "empty message array")


class DependencyBlocked(Exception):
    """A required executable/runtime dependency was unavailable."""


def run(probe, args):
    try:
        child = subprocess.run([str(probe), *args], capture_output=True, timeout=10)
    except OSError as error:
        raise DependencyBlocked(str(error)) from error
    if child.returncode == 77 or (child.returncode == 127 and b'error while loading shared libraries' in child.stderr):
        raise DependencyBlocked(f"probe/runtime unavailable: exit={child.returncode}, stderr={child.stderr!r}")
    expect(child.returncode == 0, f"probe failure (not an intended rejection): exit={child.returncode}, stderr={child.stderr!r}")
    expect(child.stderr == b'', f"unexpected stderr {child.stderr!r}")
    return child.stdout


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("probe", type=Path)
    parser.add_argument("--report", type=Path, required=True)
    parser.add_argument("--suite", choices=("characterization", "requirements", "all"), default="all")
    args = parser.parse_args()
    probe = args.probe.resolve()
    results, first = [], None
    def selected(cid):
        requirement = '-R' in cid
        return args.suite == 'all' or requirement == (args.suite == 'requirements')

    for cid, fixture, mask, mode, purpose in AUDIT_CASES:
        if not selected(cid): continue
        payload = FIXTURES[fixture]
        result = {"id": cid, "layer": "Transaction audit writer", "contract": "desired requirement" if "-R" in cid else "current behavior", "purpose": purpose, "fixture": fixture, "input_length": len(payload), "input_hex": payload.hex(), "input_sha256": hashlib.sha256(payload).hexdigest(), "parts_mask": mask, "lifecycle": mode}
        raw = b''
        try:
            raw = run(probe, ["audit", payload.hex(), str(mask), mode])
            records = raw.splitlines()
            equal(len(records), 3 if mode == "repeat" else 2 if mode == "lifecycle" else 1, "record count")
            expect(raw.endswith(b'\n'), "record terminator missing")
            decoded = [decode(line) for line in records]
            for i, record in enumerate(decoded):
                is_second = mode == "lifecycle" and i == 1
                check_audit(record, FIXTURES["clean"] if is_second else payload, mask, "writer-second" if is_second else "writer-first", invalid_response=(cid == "AUDX-R01"))
            if mode == "repeat": equal(decoded, [decoded[0]] * 3, "repeat exact record stability")
            if cid == "AUDX-C01": first = decoded[0]
            result.update(status="PASS", records=len(records))
        except DependencyBlocked as error:
            result.update(status="BLOCKED", reason=str(error))
        except (AssertionError, ValueError, OSError, TypeError, KeyError, subprocess.TimeoutExpired) as error:
            result.update(status="FAIL", reason=str(error), output_hex_prefix=raw[:192].hex())
        results.append(result)
    # Public C++ API is a separate writer, not a request parser. Exact invalid
    # output is characterization only; the three R cases below require a repair.
    invalid_empty = b'{"match":[{"variable":{"highlight":[],"value":[{"value":""}],"operator":{}}]}]\n'
    offset_cases = [
        ("OFFSET-C01", "v1,3o1,1", "single offsets currently omitted", "raw", invalid_empty),
        ("OFFSET-C02", "v6,1v0,1", "start at byte length is rejected", "error", b"Offset is out of the content limits.\n"),
        ("OFFSET-C03", "v7,1v0,1", "start beyond byte length is rejected", "error", b"Offset is out of the content limits.\n"),
        ("OFFSET-R01", "", "empty match produces valid JSON (representation otherwise open)", "json", None),
        ("OFFSET-R02", "v1,3o1,1", "single variable range and operator range round-trip", "single", None),
        ("OFFSET-R03", "v0,6v0,6t:lowercase", "transformation output is valid structural JSON", "transform", None),
    ]
    for cid, match, purpose, kind, expected in offset_cases:
        if not selected(cid): continue
        result = {"id": cid, "layer": "public ModSecurity.processContentOffset writer", "contract": "desired requirement" if "-R" in cid else "current behavior; malformed output is not a compatibility requirement", "purpose": purpose, "input_hex": b"ABCDEF".hex(), "input_length": 6, "match_string": match}
        raw = b''
        try:
            raw = run(probe, ["offset", b"ABCDEF".hex(), match])
            rc, sep, content = raw.partition(b'\n')
            expect(sep == b'\n', "API status framing missing")
            equal(rc, b'-1' if kind == 'error' else b'0', "API return code")
            if kind in ('error', 'raw'): equal(content, expected, "fixed characterization output")
            else:
                root = mapping(decode(content), "offset root")
                if kind != 'json':
                    equal(set(root), {"match"}, "offset root fields")
                    expect(type(root['match']) is list and len(root['match']) == 1, "one match array item")
                    variable = mapping(mapping(root['match'][0], "match")['variable'], "variable")
                if kind == 'single':
                    equal(variable['highlight'], [Object([('startingAt', '1'), ('size', '3')])], "single range highlight")
                    equal(variable['value'], [Object([('value', 'BCD')])], "single range value")
                    equal(mapping(variable['operator'], 'operator')['value'], 'C', "operator value")
                if kind == 'transform':
                    equal(variable['value'], [Object([('value', 'ABCDEF')]), Object([('transformation', 't:lowercase'), ('value', 'abcdef')])], "transformation chain")
            result['status'] = 'PASS'
        except DependencyBlocked as error:
            result.update(status='BLOCKED', reason=str(error))
        except (AssertionError, ValueError, OSError, TypeError, KeyError, subprocess.TimeoutExpired) as error:
            result.update(status='FAIL', reason=str(error), output_hex_prefix=raw[:384].hex())
        results.append(result)
    if args.suite != 'requirements':
        # Controls mutate only synthetic records/expectations, never production.
        for cid in ('AUDX-M01', 'AUDX-M02'):
            result = {"id": cid, "layer": "assertion sensitivity", "contract": "harness control",
                      "purpose": "wrong expectation rejected" if cid.endswith('01') else "valid alternative invalid-byte encoding accepted"}
            if first is None:
                status = 'BLOCKED' if any(r['status'] == 'BLOCKED' for r in results) else 'FAIL'
                result.update(status=status, reason='positive fixture unavailable')
            elif cid.endswith('01'):
                try:
                    check_audit(copy.deepcopy(first), b'WRONG', ALL, 'writer-first')
                    result.update(status='FAIL', reason='wrong expectation accepted')
                except AssertionError:
                    result.update(status='PASS', reason='controlled wrong expectation rejected')
            else:
                try:
                    synthetic = copy.deepcopy(first)
                    tx = dict(synthetic)['transaction']
                    tx[:] = [(k, v) for k, v in tx if k not in ('producer', 'messages')]
                    request = dict(tx)['request']
                    request[:] = [(k, v) for k, v in request if k not in ('headers', 'body')]
                    response = dict(tx)['response']
                    # A hypothetical explicit replacement strategy is tested
                    # only as a harness control. Original bytes and observed
                    # production output are never repaired before assertions.
                    replacement = (b'response:' + FIXTURES['invalid-utf8']).decode('utf-8', 'replace')
                    response[:] = [(k, replacement if k == 'body' else v) for k, v in response]
                    response_headers = dict(response)['headers']
                    response_headers[:] = [(k, replacement if k == 'X-Trace' and v != 'response:second' else v) for k, v in response_headers]
                    check_audit(synthetic, FIXTURES['invalid-utf8'], E | F, 'writer-first', invalid_response=True)
                    result.update(status='PASS', reason='hypothetical valid replacement record accepted; encoding policy stays open')
                except (AssertionError, ValueError, TypeError, KeyError) as error:
                    result.update(status='FAIL', reason=str(error))
            results.append(result)
    counts = {s: sum(r['status'] == s for r in results) for s in ('PASS', 'FAIL', 'SKIP', 'BLOCKED', 'XFAIL')}
    contract_counts = {}
    for label, subset in (("requirements", [r for r in results if '-R' in r['id']]),
                          ("characterization", [r for r in results if '-R' not in r['id'] and '-M' not in r['id']]),
                          ("assertion_sensitivity", [r for r in results if '-M' in r['id']])):
        contract_counts[label] = {status: sum(r['status'] == status for r in subset)
                                  for status in ('PASS', 'FAIL', 'SKIP', 'BLOCKED', 'XFAIL')}
    report = {"schema": 1, "selected_suite": args.suite, "contract_counts": contract_counts, "production_base": "dd1d0f82672ecf7d0f014a0c3fc4fd0ab40b3bdd", "cases": results, "counts": counts}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, ensure_ascii=True, indent=2) + '\n')
    for r in results: print(r['status'], r['id'], r.get('reason', ''))
    print(json.dumps(counts, sort_keys=True))
    return 1 if counts['FAIL'] or counts['BLOCKED'] else 0

if __name__ == '__main__': sys.exit(main())
