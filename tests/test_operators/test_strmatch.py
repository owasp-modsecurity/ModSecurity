import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'TestCase', b'', 0, 'Test case 1'),
    (b'abc', b'abcdefghi', 1, 'Test case 2'),
    (b'def', b'abcdefghi', 1, 'Test case 3'),
    (b'ghi', b'abcdefghi', 1, 'Test case 4'),
    (b'ghij', b'abcdefghi', 0, 'Test case 5'),
])
def test_strmatch_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @strmatch operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="strmatch",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
