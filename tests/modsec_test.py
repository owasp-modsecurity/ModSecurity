import os
import re
import time
import subprocess
from pathlib import Path
from typing import Optional, Dict, Any, List, Union, Pattern
import requests
from dataclasses import dataclass


@dataclass
class TestResult:
    """Result of a ModSecurity test execution"""
    __test__ = False  # not a pytest test class despite the name

    success: bool
    message: str = ""
    response: Optional[requests.Response] = None
    log_matches: Optional[Dict[str, List[str]]] = None
    skipped: bool = False

    def __post_init__(self):
        if self.log_matches is None:
            self.log_matches = {}


def _split_negation(key: str) -> "tuple[bool, str]":
    """Mirrors run-regression-tests.pl's `($neg,$name) = ($key =~ m/^(-?)(.*)$/)`."""
    if key.startswith("-"):
        return True, key[1:]
    return False, key


def _split_match_value(value) -> "tuple[Pattern[bytes], float]":
    """match_response values are a bare compiled pattern; match_log/match_file
    values are `[pattern, timeout]` (match_file's timeout is unused by the
    Perl harness itself but harmless to honor the same way)."""
    if isinstance(value, list):
        pattern = value[0]
        timeout = value[1] if len(value) > 1 else 0
        return pattern, timeout
    return value, 0


def _force_multiline(pattern: Pattern[bytes]) -> Pattern[bytes]:
    """match_log()/match_file() in Perl always match with an extra `/m` on
    top of whatever flags the qr// itself carries."""
    if pattern.flags & re.MULTILINE:
        return pattern
    return re.compile(pattern.pattern, pattern.flags | re.MULTILINE)


def _check_match_spec(spec, get_match, describe) -> Optional[str]:
    """Shared logic for match_response/match_log/match_file: walk a
    {key: pattern_or_[pattern,timeout]} spec, respecting a leading "-" on the
    key for negation, and return a failure message for the first mismatch (or
    None if everything matched as expected).

    get_match(name, pattern, timeout) -> matched bytes or None.
    describe(name) -> message prefix, e.g. "response status" or "file <path>".
    """
    for key, value in (spec or {}).items():
        negate, name = _split_negation(key)
        pattern, timeout = _split_match_value(value)
        match = get_match(name, pattern, timeout)
        if (negate and match is not None) or (not negate and match is None):
            verb = "matched (expected no match)" if negate else "failed to match"
            return f"{describe(name)} {verb}: {pattern.pattern!r}"
    return None


POOL_DEBUG_RE = re.compile(rb"POOL DEBUG:(?=[^\n]+PALLOC)[^\n]+\n")
BUFSIZ = 32768


class _PersistentLog:
    """Ports match_log()/httpd_reset_fd() from run-regression-tests.pl: an
    accumulating buffer read from a fixed position in a growing log file
    (Apache appends to it while running), reset once per test."""

    def __init__(self, path: Path):
        self.path = path
        self._position = 0
        self.buf = b""

    def reset(self):
        self.path.touch(exist_ok=True)
        self._position = self.path.stat().st_size
        self.buf = b""

    def wait_for(self, pattern: Pattern[bytes], timeout: float = 0) -> Optional[bytes]:
        """Poll for `pattern` in the accumulated buffer for up to `timeout`
        seconds, growing the buffer from the file as new data appears.
        Always tries at least once, matching the Perl do{}while loop."""
        deadline = time.monotonic() + timeout
        while True:
            try:
                with open(self.path, "rb") as f:
                    f.seek(self._position)
                    chunk = f.read(BUFSIZ)
            except FileNotFoundError:
                # match_file targets (e.g. an uploaded file) may not exist
                # yet; keep polling until the deadline like any other miss.
                chunk = b""
            if chunk:
                self._position += len(chunk)
                self.buf += chunk
                self.buf = POOL_DEBUG_RE.sub(b"", self.buf)

            match = pattern.search(self.buf)
            if match:
                return match.group(0)

            if time.monotonic() >= deadline:
                return None
            if len(chunk) < BUFSIZ:
                time.sleep(0.1)


class LogMatcher:
    """Handles matching patterns in log files. Mirrors match_log()'s real
    semantics: a persistent accumulating buffer per log, and a *timeout in
    seconds* to wait for a pattern (not an occurrence count)."""

    def __init__(self, error_log: Path, debug_log: Path, audit_log: Path):
        self.log_files = {
            'error': error_log,
            'debug': debug_log,
            'audit': audit_log,
        }
        self._logs: Dict[str, _PersistentLog] = {
            name: _PersistentLog(path) for name, path in self.log_files.items()
        }
    
    def reset_positions(self):
        """Reset every log's read position to its current end - called once
        per test, right before starting Apache (matches httpd_reset_fd())."""
        for log in self._logs.values():
            log.reset()

    def get_reader(self, log_name: str, from_start: bool = False) -> "_PersistentLog":
        """Returns the persistent reader for a well-known log (error/debug/
        audit; positioned at end-of-file by reset_positions()), or lazily
        creates one for an arbitrary file (match_file; read from the start,
        like match_file()'s own O_RDONLY-from-position-0 open in Perl)."""
        if log_name in self._logs:
            return self._logs[log_name]
        reader = _PersistentLog(Path(log_name))
        if not from_start:
            reader.reset()
        self._logs[log_name] = reader
        return reader

    def wait_for_pattern(self, log_name: str, pattern: Pattern[bytes],
                        timeout: float = 0, from_start: bool = False) -> Optional[bytes]:
        """Wait up to `timeout` seconds for `pattern` (already compiled with
        the right flags, including MULTILINE where the Perl harness forces
        it - see _force_multiline()) to appear in the named log/file."""
        reader = self.get_reader(log_name, from_start=from_start)
        return reader.wait_for(pattern, timeout=timeout)


class ResponseMatcher:
    """Handles matching patterns in HTTP responses. Mirrors match_response()
    in run-regression-tests.pl: only status/content/raw are real match
    types - anything else (some upstream .t files reference match_response
    keys the real harness doesn't recognize, relying on them always
    "failing to match") always returns None, same as Perl's fallthrough."""

    @staticmethod
    def match(mtype: str, response: requests.Response, pattern: Pattern[bytes]) -> Optional[bytes]:
        if mtype == "status":
            m = pattern.search(str(response.status_code).encode())
        elif mtype == "content":
            m = pattern.search(response.content)
        elif mtype == "raw":
            m = pattern.search(ResponseMatcher._raw_response(response))
        else:
            return None
        return m.group(0) if m else None

    @staticmethod
    def _raw_response(response: requests.Response) -> bytes:
        """Matches HTTP::Response->as_string (LWP), which `match_response`'s
        "raw" case uses in the real Perl harness: LF-only line endings, no
        CRLF, unlike the actual wire format."""
        raw = response.raw
        version = getattr(raw, "version", 11)
        lines = [f"HTTP/{version // 10}.{version % 10} {response.status_code} {response.reason}"]
        lines += [f"{name}: {value}" for name, value in response.headers.items()]
        header_text = "\n".join(lines).encode() + b"\n\n"
        return header_text + response.content

def _to_bytes(data: Union[str, bytes]) -> bytes:
    """Normalize test data to bytes. Hand-written tests pass plain `str` (real
    Unicode text, e.g. "café"); generated tests pass `bytes` literals for exact
    round-tripping of arbitrary/invalid byte sequences (e.g. b"\\xe4")."""
    return data if isinstance(data, bytes) else data.encode("utf-8")


def _msc_test_escape(data: Union[str, bytes]) -> str:
    """Escape data using msc_test.c's convention (mirrors its own escape()/
    unescape_inplace() pair) so control/NUL bytes survive as a CLI argument.

    Bytes outside printable ASCII (0x20-0x7e) become a literal ``\\xHH`` escape;
    everything else passes through unchanged.
    """
    out = []
    for byte in _to_bytes(data):
        if 0x20 <= byte <= 0x7E:
            out.append(chr(byte))
        else:
            out.append(f"\\x{byte:02x}")
    return "".join(out)


class UnitTestRunner:
    """Runs unit tests using the msc_test binary"""

    def __init__(self, msc_test_path: Optional[str] = None):
        if msc_test_path is None:
            msc_test_path = os.environ.get("MSC_TEST_PATH")
        if msc_test_path is None:
            msc_test_path = str(Path(__file__).resolve().parent / "msc_test")
        self.msc_test_path = msc_test_path
        # msc_test resolves test-data files (e.g. op/pmFromFile-01.dat) relative
        # to the tests/ directory, regardless of the caller's cwd.
        self.cwd = Path(__file__).resolve().parent

    def run_operator_test(self, name: str, param: Union[str, bytes], input_data: Union[str, bytes],
                         expected_ret: int = 1) -> TestResult:
        """Run an operator test"""
        return self._run_test("op", name, param, input_data, expected_ret)

    def run_transformation_test(self, name: str, input_data: Union[str, bytes],
                               expected_output: Union[str, bytes], expected_ret: int = 1) -> TestResult:
        """Run a transformation test"""
        return self._run_test("tfn", name, expected_output, input_data, expected_ret)

    def run_action_test(self, name: str, param: Union[str, bytes], input_data: Union[str, bytes],
                       expected_ret: int = 1) -> TestResult:
        """Run an action test"""
        return self._run_test("action", name, param, input_data, expected_ret)

    def _run_test(self, test_type: str, name: str, param: Union[str, bytes],
                  input_data: Union[str, bytes], expected_ret: int) -> TestResult:
        """Run a test using msc_test binary"""
        if not Path(self.msc_test_path).exists():
            return TestResult(
                success=False,
                message=(
                    f"msc_test binary not found at '{self.msc_test_path}'. "
                    "Build it with 'make check' (or 'make -C tests msc_test') first, "
                    "or set MSC_TEST_PATH."
                ),
            )

        cmd = [
            self.msc_test_path,
            "-t", test_type,
            "-n", name,
            "-p", _msc_test_escape(param),
            "-r", str(expected_ret)
        ]

        try:
            process = subprocess.Popen(
                cmd,
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                cwd=self.cwd,
            )

            stdout, stderr = process.communicate(input=_to_bytes(input_data))

            success = process.returncode == 0
            message = (stdout if stdout else stderr).decode("utf-8", errors="replace")

            return TestResult(
                success=success,
                message=message.strip()
            )

        except Exception as e:
            return TestResult(
                success=False,
                message=f"Failed to run test: {e}"
            )


class ModSecurityTestCase:
    """Main test case class that coordinates all testing components"""
    
    def __init__(self, apache_server=None, log_matcher: Optional[LogMatcher] = None,
                 response_matcher: Optional[ResponseMatcher] = None, http_client=None):
        self.apache_server = apache_server
        self.log_matcher = log_matcher
        self.response_matcher = response_matcher
        self.http_client = http_client
        self.unit_runner = UnitTestRunner()
    
    def run_regression_test(self, entry: Dict[str, Any]) -> TestResult:
        """Run one regression-test entry (as produced by
        regression_fixtures.load_fixture_file). Mirrors runfile() in
        run-regression-tests.pl: start Apache with the per-test conf, make
        the request, check match_response/match_log/match_file (each
        respecting a leading "-" for negation), stop Apache."""
        comment = entry.get("comment", "")

        if entry.get("unsupported"):
            return TestResult(
                success=False,
                skipped=True,
                message=(
                    f"'{entry['unsupported']}' is a Perl coderef this migration doesn't "
                    "execute - needs manual follow-up (see dump_regression_fixtures.pl)"
                ),
            )

        conf = entry.get("conf", "")
        request = entry.get("request")

        try:
            if not self.apache_server.start(conf):
                return TestResult(success=False, message=f"Failed to start Apache for '{comment}'")

            response = None
            if request:
                response = self._make_request(request)
                if response is None:
                    return TestResult(success=False, message="Failed to make HTTP request")

            def get_response_match(mtype, pattern, _timeout):
                return self.response_matcher.match(mtype, response, pattern) if response is not None else None

            failure = _check_match_spec(
                entry.get("match_response"), get_response_match, lambda mtype: f"response {mtype}"
            )
            if failure:
                return TestResult(success=False, message=failure, response=response)

            def get_log_match(log_name, pattern, timeout):
                return self.log_matcher.wait_for_pattern(log_name, _force_multiline(pattern), timeout=timeout)

            failure = _check_match_spec(entry.get("match_log"), get_log_match, lambda log_name: f"{log_name} log")
            if failure:
                return TestResult(success=False, message=failure, response=response)

            match_file = entry.get("match_file") or {}
            if match_file:
                time.sleep(1)  # matches runfile()'s own "make sure the file exists" delay

                def get_file_match(file_name, pattern, timeout):
                    return self.log_matcher.wait_for_pattern(
                        file_name, _force_multiline(pattern), timeout=timeout, from_start=True
                    )

                failure = _check_match_spec(match_file, get_file_match, lambda file_name: f"file {file_name}")
                if failure:
                    return TestResult(success=False, message=failure, response=response)

            return TestResult(success=True, message=f"Test '{comment}' passed", response=response)

        finally:
            self.apache_server.stop()
    
    def run_unit_test(self, test_config: Dict[str, Any]) -> TestResult:
        """Run a unit test"""
        test_type = test_config.get("type")
        name = test_config.get("name")
        
        if test_type == "op":
            return self.unit_runner.run_operator_test(
                name=name,
                param=test_config.get("param", ""),
                input_data=test_config.get("input", ""),
                expected_ret=test_config.get("ret", 1)
            )
        elif test_type == "tfn":
            return self.unit_runner.run_transformation_test(
                name=name,
                input_data=test_config.get("input", ""),
                expected_output=test_config.get("output", ""),
                expected_ret=test_config.get("ret", 1)
            )
        elif test_type == "action":
            return self.unit_runner.run_action_test(
                name=name,
                param=test_config.get("param", ""),
                input_data=test_config.get("input", ""),
                expected_ret=test_config.get("ret", 1)
            )
        else:
            return TestResult(
                success=False,
                message=f"Unknown test type: {test_type}"
            )
    
    def _make_request(self, request: Dict[str, Any]) -> Optional[requests.Response]:
        """Make an HTTP request from a fixture request dict (see
        regression_fixtures._decode_request): either a fully-formed
        HTTP::Request (method/uri/headers/content) or a raw byte string
        (do_raw_request() in Perl - deliberately malformed/chunked requests
        that must go over a raw socket instead of a conforming HTTP client)."""
        if request["__type__"] == "http_request":
            headers = dict(request["headers"])
            try:
                return self.http_client.request(
                    request["method"], request["uri"], headers=headers, data=request["content"],
                )
            except requests.exceptions.ConnectionError:
                # The `drop` action closes the connection with no HTTP
                # response at all. LWP::UserAgent (used by the real Perl
                # harness) synthesizes a response with a 5xx status for this
                # rather than raising, and .t files' match_response checks
                # rely on that - do the same instead of losing the response
                # entirely.
                return self._synthetic_response(500)
            except requests.RequestException:
                return None
        elif request["__type__"] == "raw":
            return self._make_raw_request(request["data"])
        else:
            raise ValueError(f"Unknown request type: {request['__type__']}")

    @staticmethod
    def _synthetic_response(status_code: int) -> requests.Response:
        response = requests.Response()
        response.status_code = status_code
        response._content = b""
        return response

    def _make_raw_request(self, raw_request: bytes) -> Optional[requests.Response]:
        """do_raw_request() in run-regression-tests.pl: write raw bytes
        straight to the socket and parse whatever comes back, bypassing
        `requests` entirely (it would "fix up" a malformed request)."""
        import socket

        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                sock.connect((self.apache_server.server_name, self.apache_server.port))
                sock.sendall(raw_request)
                sock.shutdown(socket.SHUT_WR)

                response_data = b""
                while True:
                    data = sock.recv(4096)
                    if not data:
                        break
                    response_data += data
        except OSError:
            return None

        return self._parse_raw_response(response_data)

    def _parse_raw_response(self, response_data: bytes) -> Optional[requests.Response]:
        """Parse a raw byte response into a requests.Response."""
        if b"\r\n\r\n" in response_data:
            head, _, body = response_data.partition(b"\r\n\r\n")
        else:
            head, body = response_data, b""

        lines = head.split(b"\r\n")
        if not lines or not lines[0]:
            return None
        status_parts = lines[0].split()
        if len(status_parts) < 2:
            return None
        status_code = int(status_parts[1])

        response = requests.Response()
        response.status_code = status_code
        for line in lines[1:]:
            if b":" in line:
                name, _, value = line.partition(b":")
                response.headers[name.strip().decode()] = value.strip().decode()
        response._content = body
        return response 