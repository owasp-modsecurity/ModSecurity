import pytest
import os
import tempfile
import subprocess
import time
import shutil
from pathlib import Path
from typing import Dict, Any, Optional, List
import requests
from urllib.parse import urljoin
import re
import threading
import signal
from dataclasses import dataclass

from .apache_server import ApacheServer
from .modsec_test import ModSecurityTestCase, LogMatcher, ResponseMatcher


@dataclass
class TestEnvironment:
    """Environment configuration for tests"""
    server_root: Path
    server_port: int
    server_name: str
    data_dir: Path
    temp_dir: Path
    upload_dir: Path
    conf_dir: Path
    logs_dir: Path
    htdocs_dir: Path
    httpd_conf: Path
    audit_log: Path
    debug_log: Path
    error_log: Path


@pytest.fixture(scope="session")
def test_environment():
    """Create test environment directories and configuration"""
    script_dir = Path(__file__).parent
    reg_dir = script_dir / "regression"
    server_root = reg_dir / "server_root"
    
    # Create directories
    directories = {
        'server_root': server_root,
        'data_dir': server_root / "data",
        'temp_dir': server_root / "tmp", 
        'upload_dir': server_root / "upload",
        'conf_dir': server_root / "conf",
        'logs_dir': server_root / "logs",
        'htdocs_dir': server_root / "htdocs"
    }
    
    for dir_path in directories.values():
        dir_path.mkdir(parents=True, exist_ok=True)
    
    # Test environment configuration
    env = TestEnvironment(
        server_root=server_root,
        server_port=8088,
        server_name="localhost",
        data_dir=directories['data_dir'],
        temp_dir=directories['temp_dir'],
        upload_dir=directories['upload_dir'],
        conf_dir=directories['conf_dir'],
        logs_dir=directories['logs_dir'],
        htdocs_dir=directories['htdocs_dir'],
        httpd_conf=directories['conf_dir'] / "httpd.conf",
        audit_log=directories['logs_dir'] / "modsec_audit.log",
        debug_log=directories['logs_dir'] / "modsec_debug.log",
        error_log=directories['logs_dir'] / "error.log"
    )
    
    # Set environment variables for compatibility
    os.environ.update({
        'SERVER_ROOT': str(env.server_root),
        'SERVER_PORT': str(env.server_port),
        'SERVER_NAME': env.server_name,
        'DATA_DIR': str(env.data_dir),
        'TEMP_DIR': str(env.temp_dir),
        'UPLOAD_DIR': str(env.upload_dir),
        'CONF_DIR': str(env.conf_dir),
        'LOGS_DIR': str(env.logs_dir),
        'SCRIPT_DIR': str(script_dir),
        'REGRESSION_DIR': str(reg_dir),
        'AUDIT_LOG': str(env.audit_log),
        'DEBUG_LOG': str(env.debug_log),
        'ERROR_LOG': str(env.error_log),
        'HTTPD_CONF': str(env.httpd_conf),
        'HTDOCS': str(env.htdocs_dir),
        'USER_AGENT': "ModSecurity Regression Tests/2.0.0"
    })
    
    return env


@pytest.fixture(scope="session")
def apache_config():
    """Apache configuration settings"""
    # Try to detect Apache installation
    httpd_paths = [
        '/usr/local/apache2/bin/httpd',
        '/usr/sbin/httpd',
        '/usr/bin/httpd',
        '/opt/apache2/bin/httpd'
    ]
    
    httpd = None
    for path in httpd_paths:
        if os.path.exists(path):
            httpd = path
            break
    
    if not httpd:
        pytest.skip("Apache httpd not found. Please install Apache or set HTTPD_PATH environment variable.")
    
    # Get modules directory
    try:
        result = subprocess.run([httpd, '-V'], capture_output=True, text=True)
        modules_dir = "/usr/local/apache2/modules"  # Default fallback
        for line in result.stdout.split('\n'):
            if 'HTTPD_ROOT' in line:
                match = re.search(r'"([^"]*)"', line)
                if match:
                    apache_root = match.group(1)
                    modules_dir = f"{apache_root}/modules"
                    break
    except subprocess.SubprocessError:
        modules_dir = "/usr/local/apache2/modules"
    
    return {
        'httpd': httpd,
        'modules_dir': modules_dir
    }


@pytest.fixture(scope="function")
def apache_server(test_environment, apache_config):
    """Apache server instance for testing"""
    server = ApacheServer(
        httpd_path=apache_config['httpd'],
        server_root=test_environment.server_root,
        conf_dir=test_environment.conf_dir,
        logs_dir=test_environment.logs_dir,
        modules_dir=apache_config['modules_dir'],
        port=test_environment.server_port,
        server_name=test_environment.server_name
    )
    
    yield server
    
    # Cleanup - stop server if running
    if server.is_running():
        server.stop()


@pytest.fixture(scope="function")
def log_matcher(test_environment):
    """Log file matcher for validating log entries"""
    return LogMatcher(
        error_log=test_environment.error_log,
        debug_log=test_environment.debug_log,
        audit_log=test_environment.audit_log
    )


@pytest.fixture(scope="function")
def response_matcher():
    """HTTP response matcher for validating responses"""
    return ResponseMatcher()


@pytest.fixture(scope="function")
def http_client(test_environment):
    """HTTP client for making requests"""
    session = requests.Session()
    session.headers.update({
        'User-Agent': os.environ.get('USER_AGENT', 'ModSecurity Regression Tests/2.0.0')
    })
    
    def make_request(method='GET', path='/', **kwargs):
        url = f"http://{test_environment.server_name}:{test_environment.server_port}{path}"
        return session.request(method, url, **kwargs)
    
    session.make_request = make_request
    return session


@pytest.fixture(scope="function")
def modsec_test(apache_server, log_matcher, response_matcher, http_client):
    """Complete ModSecurity test environment"""
    return ModSecurityTestCase(
        apache_server=apache_server,
        log_matcher=log_matcher,
        response_matcher=response_matcher,
        http_client=http_client
    )


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


class ApacheServerManager:
    """Manages Apache server instances across test sessions"""
    
    _instance = None
    _server = None
    
    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance
    
    def get_server(self, test_environment, apache_config):
        if self._server is None:
            self._server = ApacheServer(
                httpd_path=apache_config['httpd'],
                server_root=test_environment.server_root,
                conf_dir=test_environment.conf_dir,
                logs_dir=test_environment.logs_dir,
                modules_dir=apache_config['modules_dir'],
                port=test_environment.server_port,
                server_name=test_environment.server_name
            )
        return self._server
    
    def cleanup(self):
        if self._server and self._server.is_running():
            self._server.stop()
        self._server = None


@pytest.fixture(scope="session", autouse=True)
def cleanup_apache():
    """Ensure Apache is cleaned up at the end of the session"""
    yield
    manager = ApacheServerManager()
    manager.cleanup() 