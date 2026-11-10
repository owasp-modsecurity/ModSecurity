import pytest


@pytest.mark.unit
@pytest.mark.parametrize("param,input_data,expected_ret,test_name", [
    (b'0-255', b'', 0, 'Test case 1'),
    (b'', b'TestCase', 1, 'Test case 2'),
    (b'xxx', b'TestCase', 1, 'Test case 3'),
    (b'xxx', b'\x00', 0, 'Test case 4'),
    (b'0-255', b'abcdefghi', 0, 'Test case 5'),
    (b'97-105', b'abcdefghi', 0, 'Test case 6'),
    (b'97-105', b'abcdefghij', 1, 'Test case 7'),
])
def test_validatebyterange_operator(unit_test, param, input_data, expected_ret, test_name):
    """Test the @validateByteRange operator with various inputs"""

    result = unit_test.unit_runner.run_operator_test(
        name="validateByteRange",
        param=param,
        input_data=input_data,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
