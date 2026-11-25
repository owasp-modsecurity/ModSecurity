import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'TestCase', b'', 0, 'Test case 2'),
    (b'', b'127.0.0.1', 0, 'Test case 3'),
    (b'', b'216.75.21.122', 0, 'Test case 4'),
    (b'', b'www.modsecurity.org', 0, 'Test case 5'),
])
def test_geolookup_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @geoLookup operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="geoLookup",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
