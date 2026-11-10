import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'', b'', 1, 'Test case 1'),
    (b'TestCase', b'', 0, 'Test case 2'),
    (b'', b'TestCase', 1, 'Test case 3'),
    (b'abc', b'abcdefghi', 1, 'Test case 4'),
    (b'def', b'abcdefghi', 1, 'Test case 5'),
    (b'ghi', b'abcdefghi', 1, 'Test case 6'),
    (b'ghij', b'abcdefghi', 0, 'Test case 7'),
    (b'x', b'x', 1, 'Test case 8'),
    (b'y', b'xyz', 1, 'Test case 9'),
    (b'hiding', b'hidinX<-not quite, but is later on->hiding', 1, 'Test case 10'),
])
def test_contains_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @contains operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="contains",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
