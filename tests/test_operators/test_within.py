import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'', b'', 1, 'Test case 1'),
    (b'TestCase', b'', 1, 'Test case 2'),
    (b'', b'TestCase', 0, 'Test case 3'),
    (b'abcdefghi', b'abc', 1, 'Test case 4'),
    (b'abcdefghi', b'def', 1, 'Test case 5'),
    (b'abcdefghi', b'ghi', 1, 'Test case 6'),
    (b'abcdefghi', b'ghij', 0, 'Test case 7'),
])
def test_within_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @within operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="within",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
