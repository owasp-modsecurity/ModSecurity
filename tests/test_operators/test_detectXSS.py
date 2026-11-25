import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'', b'this is not an XSS', 0, 'Test case 2'),
    (b'', b'<a href="javascript:alert(1)">)', 1, 'Test case 3'),
])
def test_detectxss_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @detectXSS operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="detectXSS",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
