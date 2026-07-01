# ModSecurity Pytest Testing Framework

This directory contains the modernized pytest-based testing framework for ModSecurity, converted from the original Perl-based system.

## Overview

The testing framework provides comprehensive testing capabilities for ModSecurity:

- **Unit Tests**: Test individual operators, transformations, and actions using the `msc_test` C binary
- **Regression Tests**: Full integration tests with Apache server running ModSecurity
- **Parameterized Tests**: Efficient testing of multiple scenarios using pytest parametrization
- **Fixtures**: Reusable test components for Apache server management, log matching, and HTTP requests

## Installation

### Prerequisites

1. **Apache HTTP Server** with ModSecurity module installed
2. **Python 3.8+**
3. **ModSecurity** compiled with the `msc_test` binary

### Install Python Dependencies

```bash
cd tests
pip install -r requirements.txt
```

### Apache Configuration

Ensure Apache is installed and the ModSecurity module is available:

```bash
# Check Apache installation
httpd -V

# Verify ModSecurity module
httpd -M | grep security
```

## Test Structure

```
tests/
├── conftest.py                     # Pytest fixtures and configuration
├── apache_server.py                # Apache server management
├── modsec_test.py                  # Test utilities and helpers
├── test_operators/                 # Unit tests for operators
│   └── test_beginswith.py
├── test_transformations/           # Unit tests for transformations  
│   └── test_base64decode.py
├── test_actions/                   # Unit tests for actions
├── test_regression/                # Integration tests
│   └── test_disruptive_actions.py
├── requirements.txt                # Python dependencies
├── pytest.ini                     # Pytest configuration
└── README.md                      # This file
```

## Running Tests

### Run All Tests

```bash
cd tests
pytest
```

### Run Specific Test Categories

```bash
# Unit tests only
pytest -m unit

# Regression tests only  
pytest -m regression

# Apache-specific tests
pytest -m apache
```

### Run Specific Test Files

```bash
# Run operator tests
pytest test_operators/

# Run transformation tests  
pytest test_transformations/

# Run specific test file
pytest test_regression/test_disruptive_actions.py
```

### Run Individual Tests

```bash
# Run specific test method
pytest test_operators/test_beginswith.py::test_beginswith_operator

# Run specific test class
pytest test_regression/test_disruptive_actions.py::TestDisruptiveActions
```

### Parallel Test Execution

```bash
# Run tests in parallel (requires pytest-xdist)
pytest -n auto

# Run with specific number of workers
pytest -n 4
```

### Verbose Output

```bash
# Verbose output
pytest -v

# Extra verbose with stdout
pytest -v -s

# Show test durations
pytest --durations=10
```

## Test Configuration

### Environment Variables

The following environment variables are automatically set by the test framework:

- `SERVER_ROOT`: Apache server root directory
- `SERVER_PORT`: Apache server port (default: 8088)
- `SERVER_NAME`: Apache server name (default: localhost)
- `DEBUG_LOG`: ModSecurity debug log path
- `ERROR_LOG`: Apache error log path
- `AUDIT_LOG`: ModSecurity audit log path

### Pytest Markers

- `@pytest.mark.unit`: Unit tests using msc_test binary
- `@pytest.mark.regression`: Integration tests with Apache
- `@pytest.mark.apache`: Tests requiring Apache server
- `@pytest.mark.slow`: Long-running tests

## Writing Tests

### Unit Tests Example

```python
import pytest

@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret", [
    ("test", "testing", 1),
    ("test", "other", 0),
])
def test_operator(modsec_test, param, input_data, expected_ret):
    test_config = {
        "type": "op",
        "name": "beginsWith",
        "param": param,
        "input": input_data,
        "ret": expected_ret
    }
    
    result = modsec_test.run_unit_test(test_config)
    assert result.success
```

### Regression Tests Example

```python
import pytest
import re

@pytest.mark.regression
@pytest.mark.apache
def test_deny_action(modsec_test):
    config = """
        SecRuleEngine On
        SecAction "phase:1,deny,id:1001"
    """
    
    test_config = {
        "type": "action",
        "conf": config,
        "match_log": {
            "error": [re.compile(r"Access denied")]
        },
        "match_response": {
            "status": re.compile(r"^403$")
        },
        "request": {
            "method": "GET",
            "path": "/test.txt"
        }
    }
    
    result = modsec_test.run_regression_test(test_config)
    assert result.success
```

## Fixtures

### Available Fixtures

- `test_environment`: Test environment configuration and directories
- `apache_server`: Apache server instance for testing
- `log_matcher`: Log file pattern matching utilities
- `response_matcher`: HTTP response pattern matching utilities
- `http_client`: HTTP client for making requests
- `modsec_test`: Complete test environment (combines all above)

### Using Fixtures

```python
def test_custom_scenario(apache_server, log_matcher, http_client):
    # Start Apache with custom config
    config = "SecRuleEngine On"
    apache_server.start(config)
    
    # Make HTTP request
    response = http_client.make_request("GET", "/test.txt")
    
    # Check logs
    match = log_matcher.wait_for_pattern("error", r"ModSecurity:")
    
    assert response.status_code == 200
    assert match is not None
```

## Debugging

### Test Failures

When tests fail, pytest provides detailed output including:

- Test configuration used
- Apache configuration generated
- HTTP request/response details  
- Log file contents
- Error messages

### Debug Mode

```bash
# Run with debug output
pytest -v -s --tb=long

# Keep Apache server running after test failure
pytest --pdb

# Show local variables in tracebacks
pytest --tb=long --showlocals
```

### Log Files

Test logs are written to `tests/regression/server_root/logs/`:

- `error.log`: Apache error log
- `modsec_debug.log`: ModSecurity debug log
- `modsec_audit.log`: ModSecurity audit log

## Migration from Perl Tests

### Key Differences

1. **Test Format**: Converted from Perl hash structures to Python dictionaries
2. **Parametrization**: Uses pytest parametrize instead of loops
3. **Assertions**: Uses pytest assertions instead of return codes
4. **Fixtures**: Replaces global variables with pytest fixtures
5. **Configuration**: Uses pytest.ini instead of command-line options

### Converting Perl Tests

To convert a Perl `.t` file:

1. Extract test data structures from Perl format
2. Convert to Python parametrized tests or test classes
3. Update regex patterns for Python `re` module
4. Replace Perl-specific logic with Python equivalents
5. Use appropriate pytest markers

### Example Conversion

Perl format:
```perl
{
    type => "op",
    name => "beginsWith",
    param => "test",
    input => "testing", 
    ret => 1,
},
```

Python format:
```python
@pytest.mark.parametrize("param,input_data,expected_ret", [
    ("test", "testing", 1),
])
def test_beginswith(modsec_test, param, input_data, expected_ret):
    test_config = {
        "type": "op",
        "name": "beginsWith",
        "param": param,
        "input": input_data,
        "ret": expected_ret
    }
    result = modsec_test.run_unit_test(test_config)
    assert result.success
```

## Performance

The pytest framework provides several performance benefits:

- **Parallel Execution**: Run tests across multiple processes
- **Smart Test Selection**: Only run tests that may be affected by changes
- **Efficient Fixtures**: Reuse expensive setup across tests
- **Caching**: Cache test results and data

## Continuous Integration

For CI environments:

```bash
# Skip slow tests
pytest -m "not slow"

# Generate JUnit XML for CI reporting
pytest --junitxml=test-results.xml

# Generate coverage reports
pytest --cov=modsecurity --cov-report=html
```

## Troubleshooting

### Common Issues

1. **Apache not found**: Set `HTTPD_PATH` environment variable
2. **Permission errors**: Ensure user can bind to test port (8088)
3. **ModSecurity module not loaded**: Check Apache configuration
4. **Port conflicts**: Change `SERVER_PORT` environment variable

### Getting Help

- Check pytest documentation: https://docs.pytest.org/
- Review test logs in `tests/regression/server_root/logs/`
- Use `pytest --collect-only` to see discovered tests
- Run individual tests to isolate issues 