#!/usr/bin/env python3
"""Structurally check Transaction.toJSON; keep this separate from JSON parsing.

Compile audit_probe.cc against the same source build as the request probe, then:
  LD_LIBRARY_PATH="$PWD/src/.libs:$LD_LIBRARY_PATH" \
    python3 test/json-baseline/run_audit.py /absolute/path/to/audit-probe \
      --report /absolute/path/outside/the/source/audit-results.json

Expected bytes and display strings are fixed below, never generated from output.
Synthetic control characters in header values exercise the public connector API,
not an HTTP server's header validation. Only variable timestamp/server ID values
are checked by type rather than compared verbatim. Header member ordering is not
an API guarantee; duplicate members are retained and compared as lists of pairs.
"""

import argparse
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys


class Object(list):
    """JSON object pairs, distinct from arrays and preserving duplicate names."""


FIXTURES = {
    "ascii": (b'quote"slash\\', 'quote"slash\\', 'quote"slash\\'),
    "controls": (b"\x00\x08\x09\x0a\x0c\x0d\x1f",
                 "\x00\b\t\n\f\r\x1f", r"\x00\x08\x09\x0a\x0c\x0d\x1f"),
    "unicode": (b"\xc2\xa2\xe2\x82\xac\xf0\x9f\x98\x80",
                "\u00a2\u20ac\U0001f600", r"\xc2\xa2\xe2\x82\xac\xf0\x9f\x98\x80"),
    "mixed": (b'quote"slash\\\x00\x08\x09\x0a\x0c\x0d\x1f\xc2\xa2\xe2\x82\xac\xf0\x9f\x98\x80',
              'quote"slash\\\x00\b\t\n\f\r\x1f\u00a2\u20ac\U0001f600',
              'quote"slash\\' + r"\x00\x08\x09\x0a\x0c\x0d\x1f\xc2\xa2\xe2\x82\xac\xf0\x9f\x98\x80"),
    "empty": (b"", "", ""),
}

# ID, fixture, selected parts, rule mode, header mode.
CASES = [
    ("AUDIT-01", "ascii", "all", "pass", "unique"),
    ("AUDIT-02", "controls", "all", "pass", "unique"),
    ("AUDIT-03", "unicode", "all", "pass", "unique"),
    ("AUDIT-04", "mixed", "all", "pass", "unique"),
    ("AUDIT-05", "empty", "all", "pass", "unique"),
    ("AUDIT-06", "ascii", "all", "pass", "duplicate"),
    ("AUDIT-07", "ascii", "none", "pass", "unique"),
    ("AUDIT-08", "ascii", "request", "pass", "unique"),
    ("AUDIT-09", "ascii", "all", "deny", "unique"),
    ("AUDIT-10", "ascii", "all", "detect", "unique"),
]


def expect(condition, message):
    if not condition:
        raise AssertionError(message)


def mapping(value, label):
    expect(type(value) is Object, f"{label}: expected object")
    result = dict(value)
    expect(len(result) == len(value), f"{label}: unexpected duplicate member")
    return result


def equal(actual, expected, label):
    expect(type(actual) is type(expected), f"{label}: type {type(actual).__name__}, expected {type(expected).__name__}")
    expect(actual == expected, f"{label}: {actual!r}, expected {expected!r}")


def headers(actual, expected, label):
    expect(type(actual) is Object, f"{label}: expected object")
    for name, value in actual:
        expect(type(name) is str and type(value) is str, f"{label}: expected string headers")
    equal(sorted(actual), sorted(expected), label)


def check(record, case, response_override=None):
    _, fixture_name, parts, mode, header_mode = case
    fixture_bytes, response_value, request_value = FIXTURES[fixture_name]
    if response_override is not None:
        response_value = response_override
    top = mapping(record, "root")
    equal(set(top), {"transaction"}, "root fields")
    tx = mapping(top["transaction"], "transaction")
    expected_fields = {"client_ip", "time_stamp", "server_id", "client_port",
                       "host_ip", "host_port", "unique_id", "is_interrupted",
                       "request", "response"}
    if parts == "all":
        expected_fields |= {"producer", "messages"}
    equal(set(tx), expected_fields, "transaction fields")
    for name in ("time_stamp", "server_id"):
        expect(type(tx[name]) is str and bool(tx[name]), f"{name}: expected nonempty string")
    equal(tx["client_ip"], "192.0.2.10", "client_ip")
    equal(tx["host_ip"], "192.0.2.20", "host_ip")
    equal(tx["client_port"], 12345, "client_port")
    equal(tx["host_port"], 8080, "host_port")
    equal(tx["unique_id"], "audit-probe-id", "unique_id")
    equal(tx["is_interrupted"], mode == "deny", "is_interrupted")

    request = mapping(tx["request"], "request")
    expected_request = {"method", "http_version", "hostname", "uri"}
    if parts != "none":
        expected_request |= {"headers", "body"}
    equal(set(request), expected_request, "request fields")
    equal(request["method"], "POST", "method")
    equal(request["http_version"], "1.1", "http_version")
    equal(request["hostname"], "json-baseline.invalid", "hostname")
    equal(request["uri"], "/audit?source=fixture", "uri")
    if parts != "none":
        equal(request["body"], request_value, "request body")
        expected_headers = [("Content-Type", "application/octet-stream"),
                            ("Content-Length", str(len(fixture_bytes))),
                            ("X-Fixture", request_value), ('X-quote"slash\\', "name")]
        if header_mode == "duplicate":
            expected_headers.append(("X-Fixture", "second"))
        headers(request["headers"], expected_headers, "request headers")

    response = mapping(tx["response"], "response")
    equal(set(response), {"body", "http_code", "headers"} if parts == "all" else {"http_code"}, "response fields")
    equal(response["http_code"], 418 if mode == "deny" else 201, "http_code")
    if parts == "all":
        equal(response["body"], response_value, "response body")
        expected_headers = [("Content-Type", "text/plain"),
                            ("X-Fixture", response_value), ('X-quote"slash\\', "name")]
        if header_mode == "duplicate":
            expected_headers.append(("X-Fixture", "second"))
        headers(response["headers"], expected_headers, "response headers")
        producer = mapping(tx["producer"], "producer")
        equal(set(producer), {"modsecurity", "connector", "secrules_engine", "components"}, "producer fields")
        expect(type(producer["modsecurity"]) is str and "ModSecurity" in producer["modsecurity"], "producer modsecurity")
        equal(producer["connector"], "json-baseline audit probe", "connector")
        equal(producer["secrules_engine"], "DetectionOnly" if mode == "detect" else "Enabled", "secrules_engine")
        equal(producer["components"], [], "components")
        expect(type(tx["messages"]) is list and len(tx["messages"]) == 1, "exactly one message")
        message = mapping(tx["messages"][0], "message")
        equal(set(message), {"message", "details"}, "message fields")
        equal(message["message"], "audit writer", "message text")
        details = mapping(message["details"], "details")
        equal(set(details), {"match", "reference", "ruleId", "file", "lineNumber", "data", "severity", "ver", "rev", "tags", "maturity", "accuracy"}, "details fields")
        for name in set(details) - {"tags"}:
            expect(type(details[name]) is str, f"details.{name}: expected string")
        equal(details["ruleId"], "990100", "ruleId string")
        equal(details["file"], "audit-probe.conf", "rule file")
        equal(details["lineNumber"], "6", "rule lineNumber string")
        equal(details["tags"], ["fixture"], "tags")


def reject_constant(value):
    raise ValueError(f"non-JSON constant: {value}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("probe", type=Path)
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    results = []
    first_record = None
    for case in CASES:
        case_id, fixture_name, *_ = case
        payload = FIXTURES[fixture_name][0]
        result = {"id": case_id, "layer": "Transaction audit writer",
                  "fixture": fixture_name, "input_length": len(payload),
                  "input_hex": payload.hex(), "input_sha256": hashlib.sha256(payload).hexdigest(),
                  "parts": case[2], "mode": case[3], "headers": case[4]}
        try:
            completed = subprocess.run([str(args.probe.resolve()), *case[1:]],
                                       stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                       check=False, timeout=10)
            expect(completed.returncode == 0, f"probe exit {completed.returncode}: {completed.stderr.decode('utf-8', 'replace')}")
            expect(not completed.stderr, f"unexpected stderr: {completed.stderr!r}")
            raw = completed.stdout
            expect(raw.endswith(b"\n"), "writer record lacks terminating newline")
            record = json.loads(raw.decode("utf-8", "strict"),
                                object_pairs_hook=Object, parse_constant=reject_constant)
            check(record, case)
            if case_id == "AUDIT-01":
                first_record = record
            result["output_length"] = len(raw)
            result["status"] = "PASS"
        except subprocess.TimeoutExpired:
            result.update(status="FAIL", reason="probe timeout (not a parser rejection)")
        except (AssertionError, OSError, ValueError, KeyError, TypeError) as error:
            result.update(status="FAIL", reason=str(error))
        results.append(result)
        print(f"{result['status']} {case_id}" + (f": {result['reason']}" if "reason" in result else ""))

    # Deliberately incorrect expectations must fail; the actual record is kept.
    for case_id, mutation in (("AUDIT-M01", "wrong response body expectation"),
                              ("AUDIT-M02", "numeric field changed to string")):
        status, reason = "FAIL", "positive fixture unavailable"
        if first_record is not None:
            try:
                if case_id == "AUDIT-M01":
                    check(first_record, CASES[0], response_override="WRONG")
                else:
                    changed = copy.deepcopy(first_record)
                    tx = dict(changed)["transaction"]
                    tx[:] = [(key, str(value) if key == "client_port" else value)
                             for key, value in tx]
                    check(changed, CASES[0])
                reason = "controlled mutation was accepted"
            except AssertionError:
                status, reason = "PASS", "controlled mutation rejected"
        results.append({"id": case_id, "layer": "assertion sensitivity",
                        "purpose": mutation, "status": status, "reason": reason})
        print(f"{status} {case_id}: {reason}")
    counts = {status: sum(result["status"] == status for result in results)
              for status in ("PASS", "FAIL", "SKIP", "BLOCKED", "XFAIL")}
    report = {"schema": 1, "cases": results, "counts": counts}
    if args.report:
        args.report.write_text(json.dumps(report, ensure_ascii=True, indent=2) + "\n")
    print(json.dumps(counts, sort_keys=True))
    return 1 if counts["FAIL"] or counts["BLOCKED"] else 0


if __name__ == "__main__":
    sys.exit(main())
