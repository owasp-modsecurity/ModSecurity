import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'', b'this is not isqli', 0, 'Test case 2'),
    (b'', b'ascii(substring(version() from 1 for 1))', 1, 'Test case 3'),
])
def test_detectsqli_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @detectSQLi operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="detectSQLi",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
