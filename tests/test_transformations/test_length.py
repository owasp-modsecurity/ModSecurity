import pytest


@pytest.mark.unit
@pytest.mark.parametrize("input_data,expected_output,expected_ret,test_name", [
    (b'', b'0', 1, 'Test case 1'),
    (b'0123456789abcdef', b'16', 1, 'Test case 2'),
    (b'0123456789\tabcdef', b'17', 1, 'Test case 3'),
    (b'Test\x00Case', b'9', 1, 'Test case 4'),
])
def test_length_transformation(unit_test, input_data, expected_output, expected_ret, test_name):
    """Test the length transformation with various inputs"""

    result = unit_test.unit_runner.run_transformation_test(
        name="length",
        input_data=input_data,
        expected_output=expected_output,
        expected_ret=expected_ret,
    )

    assert result.success, f"Test '{test_name}' failed: {result.message}"
