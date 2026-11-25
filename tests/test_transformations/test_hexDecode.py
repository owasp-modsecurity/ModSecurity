import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 1, 'Test case 1'),
    (b'5465737443617365', b'TestCase', 1, 'Test case 2'),
    (b'546573740043617365', b'Test\x00Case', 1, 'Test case 3'),
    (b'01234567890a0z01234567890a', b'\x01#Eg\x89\n#\x01#Eg\x89\n', 1, 'Test case 4'),
    (b'01234567890az', b'\x01#Eg\x89\n', 1, 'Test case 5'),
    (b'01234567890a0', b'\x01#Eg\x89\n', 1, 'Test case 6'),
])
def test_hexdecode_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the hexDecode transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="hexDecode",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
