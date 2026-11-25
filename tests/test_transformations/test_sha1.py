import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'\xda9\xa3\xee^kK\r2U\xbf\xef\x95`\x18\x90\xaf\xd8\x07\t', 1, 'Test case 1'),
    (b'TestCase', b'\xa7\x0c\xe3\x83\x89\xe3\x18\xbd+\xe1\x8a\x01\x11\xc6\xdcv\xbd,\xd9\xed', 1, 'Test case 2'),
    (b'\x00\x01\x02\x03\x04\x05\x06\x07\x08', b'c\xbf`\xc7\x10Z\x07\xa2\xb1%\xbb\xf8\x9ea\xab\xda\xbcix\xc2', 1, 'Test case 3'),
])
def test_sha1_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the sha1 transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="sha1",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
