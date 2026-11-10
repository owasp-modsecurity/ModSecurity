import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'', 0, 'Test case 1'),
    (b'TestCase', b'TestCase', 0, 'Test case 2'),
    (b'Test\x01Case', b'Test\x01Case', 0, 'Test case 3'),
    (b'\x00TestCase', b'TestCase', 1, 'Test case 4'),
    (b'Test\x00Case', b'TestCase', 1, 'Test case 5'),
    (b'Test\x00\x00Case', b'TestCase', 1, 'Test case 6'),
    (b'TestCase\x00', b'TestCase', 1, 'Test case 7'),
    (b'\x00Test\x00Case\x00', b'TestCase', 1, 'Test case 8'),
])
def test_removenulls_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the removeNulls transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="removeNulls",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
