import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'', b'', 1, 'Test case 1'),
    (b'TestCase', b'', 0, 'Test case 2'),
    (b'', b'TestCase', 1, 'Test case 3'),
    (b'abc', b'abcdefghi', 0, 'Test case 4'),
    (b'def', b'abcdefghi', 0, 'Test case 5'),
    (b'ghi', b'abcdefghi', 1, 'Test case 6'),
    (b'ghi', b'abcdef\x00ghi', 1, 'Test case 7'),
])
def test_endswith_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @endsWith operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="endsWith",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
