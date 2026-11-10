import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'\xd4\x1d\x8c\xd9\x8f\x00\xb2\x04\xe9\x80\t\x98\xec\xf8B~', 1, 'Test case 1'),
    (b'TestCase', b'\xc9\xab\xa2\xc3\xe6\x01&\x16\x9e\x80\xe9\xa2k\xa2s\xc1', 1, 'Test case 2'),
    (b'\x00\x01\x02\x03\x04\x05\x06\x07\x08', b'\xa6\xe7\xd3\xb4o\xdf\xaf\x0b\xde*\x1f\x83*\x00\xd2\xde', 1, 'Test case 3'),
])
def test_md5_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the md5 transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="md5",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
