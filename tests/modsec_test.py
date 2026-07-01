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
    log_matches: Dict[str, List[str]] = None
    
    def __post_init__(self):
        if self.log_matches is None:
            self.log_matches = {}


class LogMatcher:
    """Handles matching patterns in log files"""
    
    def __init__(self, error_log: Path, debug_log: Path, audit_log: Path):
        self.log_files = {
            'error': error_log,
            'debug': debug_log, 
            'audit': audit_log
        }
        self._last_positions = {name: 0 for name in self.log_files.keys()}
    
    def reset_positions(self):
        """Reset file read positions to current end"""
        for log_name, log_file in self.log_files.items():
            if log_file.exists():
                try:
                    with open(log_file, 'r') as f:
                        f.seek(0, 2)  # Seek to end
                        self._last_positions[log_name] = f.tell()
                except Exception:
                    self._last_positions[log_name] = 0
            else:
                self._last_positions[log_name] = 0
    
    def wait_for_pattern(self, log_name: str, pattern: Union[str, Pattern], 
                        timeout: int = 10) -> Optional[str]:
        """Wait for a pattern to appear in the specified log file"""
        if isinstance(pattern, str):
            regex = re.compile(pattern)
        else:
            regex = pattern
        
        log_file = self.log_files.get(log_name)
        if not log_file:
            return None
        
        start_time = time.time()
        last_position = self._last_positions.get(log_name, 0)
        
        while time.time() - start_time < timeout:
            if log_file.exists():
                try:
                    with open(log_file, 'r') as f:
                        f.seek(last_position)
                        new_content = f.read()
                        last_position = f.tell()
                        self._last_positions[log_name] = last_position
                        
                        for line in new_content.split('\n'):
                            match = regex.search(line)
                            if match:
                                return match.group()
                except Exception:
                    pass
            
            time.sleep(0.1)
        
        return None
    
    def check_patterns(self, patterns: Dict[str, List]) -> Dict[str, List[str]]:
        """Check multiple patterns across different log files
        
        Args:
            patterns: Dict with log names as keys and list of [pattern, count] as values
            
        Returns:
            Dict with log names as keys and list of matches as values
        """
        results = {}
        
        for log_name, pattern_list in patterns.items():
            results[log_name] = []
            
            for pattern_info in pattern_list:
                if isinstance(pattern_info, list) and len(pattern_info) >= 1:
                    pattern = pattern_info[0]
                    expected_count = pattern_info[1] if len(pattern_info) > 1 else 1
                else:
                    pattern = pattern_info
                    expected_count = 1
                
                matches = []
                for _ in range(expected_count):
                    match = self.wait_for_pattern(log_name, pattern)
                    if match:
                        matches.append(match)
                    else:
                        break
                
                results[log_name].extend(matches)
        
        return results
    
    def get_log_content(self, log_name: str, from_position: bool = True) -> str:
        """Get content from log file"""
        log_file = self.log_files.get(log_name)
        if not log_file or not log_file.exists():
            return ""
        
        try:
            with open(log_file, 'r') as f:
                if from_position:
                    f.seek(self._last_positions.get(log_name, 0))
                return f.read()
        except Exception:
            return ""


class ResponseMatcher:
    """Handles matching patterns in HTTP responses"""
    
    @staticmethod
    def match_status(response: requests.Response, pattern: Union[str, Pattern]) -> bool:
        """Match response status code against pattern"""
        if isinstance(pattern, str):
            regex = re.compile(pattern)
        else:
            regex = pattern
        
        return bool(regex.search(str(response.status_code)))
    
    @staticmethod
    def match_content(response: requests.Response, pattern: Union[str, Pattern]) -> bool:
        """Match response content against pattern"""
        if isinstance(pattern, str):
            regex = re.compile(pattern, re.MULTILINE)
        else:
            regex = pattern
        
        return bool(regex.search(response.text))
    
    @staticmethod 
    def match_headers(response: requests.Response, header_name: str, 
                     pattern: Union[str, Pattern]) -> bool:
        """Match response header against pattern"""
        if isinstance(pattern, str):
            regex = re.compile(pattern)
        else:
            regex = pattern
        
        header_value = response.headers.get(header_name, "")
        return bool(regex.search(header_value))
    
    @staticmethod
    def match_raw(response: requests.Response, pattern: Union[str, Pattern]) -> bool:
        """Match full response (headers + content) against pattern"""
        if isinstance(pattern, str):
            regex = re.compile(pattern, re.MULTILINE)
        else:
            regex = pattern
        
        # Reconstruct raw response
        raw_response = f"HTTP/{response.raw.version // 10}.{response.raw.version % 10} {response.status_code} {response.reason}\n"
        for name, value in response.headers.items():
            raw_response += f"{name}: {value}\n"
        raw_response += f"\n{response.text}"
        
        return bool(regex.search(raw_response))
    
    def check_response(self, response: requests.Response, 
                      match_criteria: Dict[str, Any]) -> Dict[str, bool]:
        """Check response against multiple criteria
        
        Args:
            response: HTTP response object
            match_criteria: Dict with criteria types as keys and patterns as values
            
        Returns:
            Dict with criteria types as keys and match results as values
        """
        results = {}
        
        for criteria_type, pattern in match_criteria.items():
            if criteria_type == "status":
                results[criteria_type] = self.match_status(response, pattern)
            elif criteria_type == "content":
                results[criteria_type] = self.match_content(response, pattern)
            elif criteria_type == "raw":
                results[criteria_type] = self.match_raw(response, pattern)
            elif criteria_type.startswith("header_"):
                header_name = criteria_type[7:]  # Remove "header_" prefix
                results[criteria_type] = self.match_headers(response, header_name, pattern)
            else:
                results[criteria_type] = False
        
        return results


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
    
    def run_regression_test(self, test_config: Dict[str, Any]) -> TestResult:
        """Run a complete regression test"""
        # Extract test configuration
        test_type = test_config.get("type", "unknown")
        comment = test_config.get("comment", "")
        conf = test_config.get("conf", "")
        request = test_config.get("request")
        match_response = test_config.get("match_response", {})
        match_log = test_config.get("match_log", {})
        prerun = test_config.get("prerun")
        
        try:
            # Reset log positions
            self.log_matcher.reset_positions()
            
            # Start Apache with test configuration
            if not self.apache_server.start(conf):
                return TestResult(
                    success=False,
                    message="Failed to start Apache server"
                )
            
            # Run prerun setup if provided
            if prerun and callable(prerun):
                prerun_result = prerun()
                if prerun_result != 0:
                    return TestResult(
                        success=False,
                        message=f"Prerun failed with code {prerun_result}"
                    )
            
            # Make HTTP request if provided
            response = None
            if request:
                response = self._make_request(request)
                if not response:
                    return TestResult(
                        success=False,
                        message="Failed to make HTTP request"
                    )
            
            # Check response criteria
            response_results = {}
            if match_response and response:
                response_results = self.response_matcher.check_response(
                    response, match_response
                )
                
                for criteria, result in response_results.items():
                    if not result:
                        return TestResult(
                            success=False,
                            message=f"Response {criteria} match failed",
                            response=response
                        )
            
            # Check log criteria  
            log_results = {}
            if match_log:
                log_results = self.log_matcher.check_patterns(match_log)
                
                for log_name, patterns in match_log.items():
                    expected_matches = len(patterns)
                    actual_matches = len(log_results.get(log_name, []))
                    
                    if actual_matches < expected_matches:
                        return TestResult(
                            success=False,
                            message=f"Log {log_name} match failed: expected {expected_matches}, got {actual_matches}",
                            response=response,
                            log_matches=log_results
                        )
            
            return TestResult(
                success=True,
                message=f"Test '{comment}' passed",
                response=response,
                log_matches=log_results
            )
            
        except Exception as e:
            return TestResult(
                success=False,
                message=f"Test failed with exception: {e}"
            )
        
        finally:
            # Always stop the server
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
    
    def _make_request(self, request_config) -> Optional[requests.Response]:
        """Make HTTP request based on configuration"""
        if isinstance(request_config, requests.Request):
            # Direct Request object
            return self.http_client.send(request_config)
        elif callable(request_config):
            # Function that returns request configuration
            try:
                request_config = request_config()
            except Exception:
                return None
        
        # Handle dictionary configuration
        if isinstance(request_config, dict):
            method = request_config.get("method", "GET")
            path = request_config.get("path", "/")
            return self.http_client.make_request(method, path, **request_config)
        
        # Handle raw request string
        if isinstance(request_config, str):
            return self._make_raw_request(request_config)
        
        return None
    
    def _make_raw_request(self, raw_request: str) -> Optional[requests.Response]:
        """Make raw HTTP request"""
        try:
            import socket
            
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((self.apache_server.server_name, self.apache_server.port))
            sock.send(raw_request.encode())
            sock.shutdown(socket.SHUT_WR)
            
            response_data = b""
            while True:
                data = sock.recv(4096)
                if not data:
                    break
                response_data += data
            
            sock.close()
            
            # Parse response
            response_text = response_data.decode('utf-8', errors='ignore')
            return self._parse_raw_response(response_text)
            
        except Exception:
            return None
    
    def _parse_raw_response(self, response_text: str) -> requests.Response:
        """Parse raw HTTP response into requests.Response object"""
        # This is a simplified parser - in practice you might want to use
        # a more robust HTTP parser
        lines = response_text.split('\n')
        
        # Parse status line
        status_line = lines[0]
        status_code = int(status_line.split()[1])
        
        # Parse headers
        headers = {}
        content_start = 0
        for i, line in enumerate(lines[1:], 1):
            if line.strip() == "":
                content_start = i + 1
                break
            if ":" in line:
                name, value = line.split(":", 1)
                headers[name.strip()] = value.strip()
        
        # Get content
        content = '\n'.join(lines[content_start:])
        
        # Create mock response
        response = requests.Response()
        response.status_code = status_code
        response.headers.update(headers)
        response._content = content.encode()
        
        return response 