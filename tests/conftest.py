import pytest
from pathlib import Path
import requests

from .apache_server import ApacheServer
from .modsec_test import ModSecurityTestCase, LogMatcher, ResponseMatcher

REGRESSION_LOGS_DIR = Path(__file__).parent / "regression" / "server_root" / "logs"


@pytest.fixture(scope="function")
def log_matcher():
    """Log file matcher for validating log entries. Function-scoped: each
    regression test gets a fresh buffer/read-position, matching
    httpd_reset_fd() being called at the top of every runfile() iteration."""
    REGRESSION_LOGS_DIR.mkdir(parents=True, exist_ok=True)
    return LogMatcher(
        error_log=REGRESSION_LOGS_DIR / "error.log",
        debug_log=REGRESSION_LOGS_DIR / "modsec_debug.log",
        audit_log=REGRESSION_LOGS_DIR / "modsec_audit.log",
    )


@pytest.fixture(scope="function")
def apache_server(log_matcher):
    """One httpd instance for one regression test - started/stopped fresh
    per test, matching runfile()'s own restart-per-test-config model."""
    server = ApacheServer(log_matcher=log_matcher)

    yield server

    if server.is_running():
        server.stop()


@pytest.fixture(scope="function")
def response_matcher():
    """HTTP response matcher for validating responses"""
    return ResponseMatcher()


@pytest.fixture(scope="function")
def http_client():
    """HTTP client for making requests. User-Agent matches $UA_NAME in
    run-regression-tests.pl, since a few tests check for it verbatim."""
    session = requests.Session()
    session.headers.update({'User-Agent': 'ModSecurity Regression Tests/1.2.3'})
    return session


@pytest.fixture(scope="function")
def modsec_test(apache_server, log_matcher, response_matcher, http_client):
    """Complete ModSecurity test environment (regression tests, needs Apache)"""
    return ModSecurityTestCase(
        apache_server=apache_server,
        log_matcher=log_matcher,
        response_matcher=response_matcher,
        http_client=http_client
    )


@pytest.fixture(scope="function")
def unit_test():
    """Lightweight test environment for op/tfn unit tests. No Apache server
    is started - only the msc_test binary is used, so this fixture is cheap
    and has no dependency on apache_server/test_environment."""
    return ModSecurityTestCase()


def pytest_configure(config):
    """Pytest configuration"""
    # Add custom markers
    config.addinivalue_line("markers", "unit: Unit tests for operators, transformations, actions")
    config.addinivalue_line("markers", "regression: Integration tests with Apache server")
    config.addinivalue_line("markers", "slow: Tests that take a long time to run")
    config.addinivalue_line("markers", "apache: Tests that require Apache server")


def pytest_collection_modifyitems(config, items):
    """Modify test collection"""
    # Add markers based on test location
    for item in items:
        # Add apache marker for regression tests
        if "regression" in str(item.fspath):
            item.add_marker(pytest.mark.apache)
            item.add_marker(pytest.mark.regression)
        
        # Add unit marker for unit tests  
        if any(x in str(item.fspath) for x in ["test_operators", "test_transformations", "test_actions"]):
            item.add_marker(pytest.mark.unit)


