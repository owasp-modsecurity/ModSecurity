import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'', b'', 1, 'Test case 1'),
    (b'TestCase', b'', 0, 'Test case 2'),
    (b'', b'TestCase', 1, 'Test case 3'),
    (b'abc', b'abcdefghi', 0, 'Test case 4'),
    (b'def', b'abcdefghi', 0, 'Test case 5'),
    (b'ghi', b'abcdefghi', 0, 'Test case 6'),
    (b'abc', b'abc def ghi', 1, 'Test case 7'),
    (b'def', b'abc def ghi', 1, 'Test case 8'),
    (b'ghi', b'abc def ghi', 1, 'Test case 9'),
    (b'abc', b'abc\x00def ghi', 1, 'Test case 10'),
    (b'def', b'abc\x00def ghi', 1, 'Test case 11'),
    (b'x', b'x', 1, 'Test case 12'),
    (b'x', b' x ', 1, 'Test case 13'),
    (b'y', b'xyz', 0, 'Test case 14'),
    (b'hiding', b'hidingX<-not on word boundary, but is later on->hiding', 1, 'Test case 15'),
])
def test_containsword_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @containsWord operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="containsWord",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
